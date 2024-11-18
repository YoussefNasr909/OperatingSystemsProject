#include "Directory.h"
#include "Mini_FAT.h"
#include "Converter.h"

namespace OperatingSystemsProject {

    Directory::Directory(string name, char dir_attr, int dir_firstCluster, Directory* par)
        : Directory_Entry(name, dir_attr, dir_firstCluster), parent(par) {}

    Directory_Entry Directory::GetDirectory_Entry() {
        Directory_Entry entry(dir_name, dir_attr, dir_firstCluster);
        entry.dir_filesize = getMySizeOnDisk();
        return entry;
    }

    int Directory::getMySizeOnDisk() {
        int totalEntries = DirFiles.size();
        int sizeInBytes = totalEntries * 32;
        return ((sizeInBytes + 1023) / 1024) * 1024;
    }

    bool Directory::canAddEntry(Directory_Entry d) {
        int totalEntries = DirFiles.size();
        int maxEntriesPerCluster = 1024 / 32;
        int clustersRequired = (totalEntries + 1 + maxEntriesPerCluster - 1) / maxEntriesPerCluster;

        int availableClusters = 0;
        for (int i = 0; i < 1024; i++) {
            if (Mini_FAT::FAT[i] == 0) {
                availableClusters++;
            }
        }

        return clustersRequired <= availableClusters;
    }

    void Directory::emptyMyClusters() {
        int currentCluster = dir_firstCluster;

        while (currentCluster != -1) {
            int nextCluster = Mini_FAT::FAT[currentCluster];
            Mini_FAT::FAT[currentCluster] = 0;
            currentCluster = nextCluster;
        }

        dir_firstCluster = 0;
    }
}
