#include "Directory.h"
#include "virtualDisk.h"
#include "Mini_FAT.h"
#include <algorithm>
#include <iostream>
#include "Converter.h"
#include "DirectoryEntry.h"

// Constructor initializes a Directory object with its name, attributes, first cluster, and parent directory
// Constructor: ده الكونستركتور اللي بيعمل إنشاء للدليل (Directory) وبيستلم:
// - الاسم (name)
// - النوع (attributes) زي فايل أو فولدر
// - أول كلاستر بيبدأ منه (firstCluster)
// - والأب (Parent directory)
namespace OperatingSystemsProject {

    Directory::Directory(string name, char dir_attr, int dir_firstCluster, Directory* pa)
        : Directory_Entry(name, dir_attr, dir_firstCluster), parent(pa) {
    }

// Returns the Directory_Entry object of this directory
// GetDirectory_Entry: دي دالة بتجهز نسخة من الدليل ده كـ Directory_Entry عشان تستخدمه
Directory_Entry Directory::GetDirectory_Entry() {
    Directory_Entry entry(dir_name, dir_attr, dir_firstCluster);
    entry.dir_filesize = getMySizeOnDisk();
    return entry;
}

// getMySizeOnDisk: بتحسب الحجم اللي مستخدمه الدليل على القرص
int Directory::getMySizeOnDisk() {
    int size = 0;
    int totalEntries = DirOrFiles.size();
    int sizeInBytes = totalEntries * 32; // Each entry is 32 bytes
    return ((sizeInBytes + 1023) / 1024) * 1024; // Convert to clusters
}

// Checks if a new entry can be added to the directory without exceeding the 1MB disk limit
// canAddEntry: بتشوف لو في مساحة كفاية لإضافة مدخل جديد في الدليل.
bool Directory::canAddEntry(Directory_Entry d) {
    int totalEntries = DirOrFiles.size(); // عدد المدخلات الحالي
    int maxEntriesPerCluster = 1024 / 32; // أقصى عدد مدخلات في كلاستر
    int clustersRequired = (totalEntries + 1 + maxEntriesPerCluster - 1) / maxEntriesPerCluster;

    int availableClusters = Mini_FAT::getAvilableClusters(); // حساب الكلاسترز الفاضية
    return clustersRequired <= availableClusters; // لو في مساحة كفاية، نرجع true
}

// Clears all clusters associated with this directory by releasing them in the Mini_FAT
// emptyMyClusters: بتفضي كل الكلاسترز المرتبطة بالدليل
void Directory::emptyMyClusters() {
    int currentCluster = dir_firstCluster; // نبدأ من أول كلاستر

    while (currentCluster != -1) {
        int nextCluster = Mini_FAT::FAT[currentCluster];
        Mini_FAT::FAT[currentCluster] = 0;
        currentCluster = nextCluster;
    }

    dir_firstCluster = 0;
}

// writeDirectory: اهم فنكشن
void Directory::writeDirectory() {
    vector<char> directory_data; // بيانات الدليل هتتخزن هنا

    // Convert all Directory_Entry objects files or dir to byte arrays
    for (const auto& entry : DirOrFiles) {
        vector<char> entry_bytes = Converter::Directory_EntryToBytes(entry);
        directory_data.insert(directory_data.end(), entry_bytes.begin(), entry_bytes.end());
    }

    size_t cluster_size = 1024; // Each cluster is 1024 bytes
    size_t total_clusters_needed = (directory_data.size() + cluster_size - 1) / cluster_size; // نحسب عدد الكلاسترز المطلوبة

    vector<int> current_clusters;
    int cluster_index = dir_firstCluster;

    // نجيب الكلاسترز اللي الدليل بيستخدمها
    while (cluster_index != -1 && cluster_index != 0) {
        current_clusters.push_back(cluster_index);
        cluster_index = Mini_FAT::getClusterPointer(cluster_index);
    }

    // لو عدد الكلاسترز الحالية مش كفاية، نطلب كلاسترز جديدة
    if (total_clusters_needed > current_clusters.size()) {
        int last_cluster = (current_clusters.size() > 0) ? current_clusters.back() : -1;
        for (size_t i = 0; i < total_clusters_needed - current_clusters.size(); ++i) {
            int new_cluster = Mini_FAT::getAvilableClusters();
            if (new_cluster == -1) {
                cout << "Error: No free clusters available" << endl;
                return;
            }
            if (last_cluster != -1) {
                Mini_FAT::setClusterPointer(last_cluster, new_cluster);
            }
            else {
                dir_firstCluster = new_cluster;
            }
            last_cluster = new_cluster;
            current_clusters.push_back(new_cluster);
        }
        Mini_FAT::setClusterPointer(last_cluster, -1); // Mark end of cluster chain
    }
    else if (total_clusters_needed < current_clusters.size()) {
        for (size_t i = total_clusters_needed; i < current_clusters.size(); ++i) {
            Mini_FAT::setClusterPointer(current_clusters[i], 0);
        }
        Mini_FAT::setClusterPointer(current_clusters[total_clusters_needed - 1], -1);
    }

    // نكتب البيانات على الكلاسترز
    size_t data_offset = 0;
    for (auto cluster : current_clusters) {
        vector<char> cluster_data(cluster_size, 0); // Empty cluster
        size_t to_copy = min(directory_data.size() - data_offset, cluster_size);
        copy(directory_data.begin() + data_offset, directory_data.begin() + data_offset + to_copy, cluster_data.begin());
        virtualDisk::writeDataCluster(cluster_data, cluster); // Write data to cluster
        data_offset += to_copy;
        if (data_offset >= directory_data.size()) break;
    }

    Mini_FAT::writeFAT(); // Update the FAT
}

// Reads the directory data from the virtual disk
void Directory::readDirectory() {
    DirOrFiles.clear(); // Clear existing entries

    int cluster_index = dir_firstCluster;
    vector<char> directory_data;

    // Read all clusters of this directory and concatenate their data
    while (cluster_index != -1 && cluster_index != 0) {
        vector<char> cluster_data = virtualDisk::readDataCluster(cluster_index);
        directory_data.insert(directory_data.end(), cluster_data.begin(), cluster_data.end());
        cluster_index = Mini_FAT::getClusterPointer(cluster_index);
    }

    size_t entry_size = 32; // Each directory entry occupies 32 bytes
    for (size_t i = 0; i + entry_size <= directory_data.size(); i += entry_size) {
        vector<char> entry_bytes(directory_data.begin() + i, directory_data.begin() + i + entry_size);
        Directory_Entry entry = Converter::BytesToDirectory_Entry(entry_bytes);
        if (entry.dir_name[0] != '\0') {
            DirOrFiles.push_back(entry);
        }
    }
}

// Adds a new directory entry, writes changes to disk if there's enough space
void Directory::addEntry(Directory_Entry d) {
    if (canAddEntry(d)) {
        DirOrFiles.push_back(d);
        writeDirectory();
    }
    else {
        cout << "Error: Not enough space to add entry." << endl;
    }
}

// Removes an entry matching the specified directory entry and updates the disk
void Directory::removeEntry(Directory_Entry d) {
    auto it = remove_if(DirOrFiles.begin(), DirOrFiles.end(),
        [&d](const Directory_Entry& entry) {
            return strcmp(entry.dir_name, d.dir_name) == 0;
        });
    if (it != DirOrFiles.end()) {
        DirOrFiles.erase(it, DirOrFiles.end());
        writeDirectory();
    }
}

// Deletes the directory by releasing its clusters and clearing its contents
void Directory::deleteDirectory() {
    emptyMyClusters();
    writeDirectory();
}

// Updates an existing entry with new content and writes changes to disk
void Directory::updateContent(Directory_Entry OLD, Directory_Entry NEW) {
    for (auto& entry : DirOrFiles) {
        if (strcmp(entry.dir_name, OLD.dir_name) == 0) {
            entry = NEW;
            break;
        }
    }
    writeDirectory();
}

// Searches the directory for an entry by name and returns its index, or -1 if not found
int Directory::searchDirectory(string name) {
    string cleanedName = Directory_Entry::cleanTheName(name);
    for (size_t i = 0; i < DirOrFiles.size(); ++i) {
        if (strcmp(DirOrFiles[i].dir_name, cleanedName.c_str()) == 0) {
            return i;
        }
    }
    return -1;
}
