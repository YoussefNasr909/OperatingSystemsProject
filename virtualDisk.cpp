#include "virtualDisk.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class virtualDisk {
public:
    static fstream Disk;

    static void createOrOpenDisk(const string& path) {
        Disk.open(path, ios::in | ios::out | ios::binary);
        if (!Disk.is_open()) {
            Disk.clear();
            Disk.open(path, ios::out | ios::binary);
            Disk.close();
            Disk.open(path, ios::in | ios::out | ios::binary);
        }
    }

    static void writeDataCluster(const vector<char>& cluster, int clusterIndex) {
        Disk.seekp(clusterIndex * 1024, ios::beg);
        Disk.write(cluster.data(), 1024);
        Disk.flush();
    }

    static vector<char> readDataCluster(int clusterIndex) {
        Disk.seekg(clusterIndex * 1024, ios::beg);
        vector<char> bytes(1024);
        Disk.read(bytes.data(), 1024);
        return bytes;
    }

    static int getFreeSpace() {
        Disk.seekg(0, ios::end);
        return (1024 * 1024) - static_cast<int>(Disk.tellg());
    }

    static bool isNew() {
        Disk.seekg(0, ios::end);
        int size = static_cast<int>(Disk.tellg());
        return (size == 0);
    }
};

fstream virtualDisk::Disk;

int main() {
    string path = "virtual_disk.bin";

    virtualDisk::createOrOpenDisk(path);

    if (virtualDisk::isNew()) {
        cout << "Disk is new and needs initialization." << endl;
    }
    else {
        cout << "Disk already initialized." << endl;
    }

    vector<char> dataToWrite(1024, 'A');
    virtualDisk::writeDataCluster(dataToWrite, 5);

    vector<char> dataRead = virtualDisk::readDataCluster(5);
    cout << "Data read from cluster 5: ";
    for (int i = 0; i < 10; ++i) {
        cout << dataRead[i];
    }
    cout << endl;
    int freeSpace = virtualDisk::getFreeSpace();
    cout << "Free space on virtual disk: " << freeSpace << " bytes" << endl;

    return 0;
}
