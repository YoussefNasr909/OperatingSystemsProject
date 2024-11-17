#include "Mini_FAT.h"
#include "Converter.h"
#include <vector>
#include <iostream>

using namespace std;

int Mini_FAT::FAT[1024]; // Definition of the static FAT array

void Mini_FAT::initialize_FAT()
{
    for (int i = 0; i < 1024; i++)
    {
        if (i == 0 || i == 4)
        {
            FAT[i] = -1;
        }
        else if (i > 0 && i <= 3)
        {
            FAT[i] = i + 1;
        }
        else
        {
            FAT[i] = 0;
        }
    }
}

vector<char> Mini_FAT::createSuperBlock()
{
    vector<char> superBlock(1024, 0);
    return superBlock;
}

void Mini_FAT::writeFAT()
{
    vector<char> FATBYTES = Converter::intArrayToByteArray(FAT, 1024);
    vector<vector<char>> ls = Converter::splitBytes(FATBYTES);
    for (int i = 0; i < ls.size(); i++)
    {
        virtualDisk::writeDataCluster(ls[i], i + 1);
    }
}

void Mini_FAT::readFAT()
{
    vector<char> ls;
    for (int i = 1; i <= 4; i++)
    {
        vector<char> b = virtualDisk::readDataCluster(i);
        ls.insert(ls.end(), b.begin(), b.end());
    }
    Converter::byteArrayToIntArray(FAT, ls);
}

void Mini_FAT::printFAT()
{
    cout << "FAT has the following: ";
    for (int i = 0; i < 1024; i++)
        cout << "FAT[" << i << "] = " << FAT[i] << endl;
}

void Mini_FAT::setFAT(int fat_arr[1024])
{
    for (int i = 0; i < 1024; i++)
    {
        FAT[i] = fat_arr[i];
    }
}

void Mini_FAT::initialize_Or_Open_FileSystem(string name)
{
    virtualDisk::createOrOpenDisk(name);
    if (virtualDisk::isNew())
    {
        vector<char> superBlock = createSuperBlock();
        virtualDisk::writeDataCluster(superBlock, 0);
        initialize_FAT();
        writeFAT();
    }
    else
    {
        readFAT();
    }
}

void Mini_FAT::getAvilableClusters()
{
    int count = 0;
    for (int i = 0; i < 1024; i++)
    {
        if (FAT[i] == 0)
            count++;
    }
    cout << "Available clusters: " << count << endl;
}

void Mini_FAT::setClusterPointer(int clusterIndex, int pointer)
{
    if (clusterIndex >= 0 && clusterIndex < 1024 && pointer >= 0 && pointer < 1024)
    {
        FAT[clusterIndex] = pointer;
    }
}

void Mini_FAT::getClusterPointer(int clusterIndex)
{
    if (clusterIndex >= 0 && clusterIndex < 1024)
    {
        cout << "Cluster pointer at index " << clusterIndex << ": " << FAT[clusterIndex] << endl;
    }
    else
    {
        cout << "Invalid cluster index." << endl;
    }
}

void Mini_FAT::getFreeSize()
{
    int freeClusters = 0;
    for (int i = 0; i < 1024; i++)
    {
        if (FAT[i] == 0)
            freeClusters++;
    }
    cout << "Free size: " << (freeClusters * 1024) << " bytes" << endl;
}
