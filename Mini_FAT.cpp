#include "Mini_FAT.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

int Mini_FAT::FAT[1024];
void Mini_FAT::initialize_FAT()
{
    fill_n(FAT, 1024, -1);
}
vector<char> Mini_FAT::createSuperBlock()
{
    vector<char> superBlock(1024 * sizeof(int));
    memcpy(superBlock.data(), FAT, 1024 * sizeof(int));
    return superBlock;
}
void Mini_FAT::writeFAT()
{
    ofstream outFile("FAT.bin", ios::binary);
    if (outFile)
    {
        outFile.write(reinterpret_cast<char*>(FAT), 1024 * sizeof(int));
        outFile.close();
    }
    else
    {
        cout << "Error writing to FAT file." << endl;
    }
}
void Mini_FAT::readFAT()
{
    ifstream inFile("FAT.bin", ios::binary);
    if (inFile)
    {
        inFile.read(reinterpret_cast<char*>(FAT), 1024 * sizeof(int));
        inFile.close();
    }
    else
    {
        cout << "Error reading FAT file." << endl;
    }
}
void Mini_FAT::printFAT()
{
    for (int i = 0; i < 1024; i++)
    {
        cout << "Cluster " << i << ": " << FAT[i] << endl;
    }
}
void Mini_FAT::setFAT(int fat_arr[1024])
{
    memcpy(FAT, fat_arr, 1024 * sizeof(int));
}
void Mini_FAT::initialize_Or_Open_FileSystem(string name)
{
    ifstream file(name, ios::binary);
    if (file)
    {
        readFAT();
        cout << "FAT system opened from file." << endl;
    }
    else
    {
        initialize_FAT();
        writeFAT();
        cout << "FAT system initialized." << endl;
    }
}
void Mini_FAT::getAvilableClusters()
{
    int availableClusters = 0;
    for (int i = 0; i < 1024; i++)
    {
        if (FAT[i] == -1)
            availableClusters++;
    }
    cout << "Available clusters: " << availableClusters << endl;
}
void Mini_FAT::setClusterPointer(int clusterIndex, int pointer)
{
    if (clusterIndex >= 0 && clusterIndex < 1024)
        FAT[clusterIndex] = pointer;
    else
        cout << "Cluster index out of bounds." << endl;
}
void Mini_FAT::getClusterPointer(int clusterIndex)
{
    if (clusterIndex >= 0 && clusterIndex < 1024)
        cout << "Pointer at cluster " << clusterIndex << ": " << FAT[clusterIndex] << endl;
    else
        cout << "Cluster index out of bounds." << endl;
}
void Mini_FAT::getFreeSize()
{
    int freeClusters = 0;
    for (int i = 0; i < 1024; i++)
    {
        if (FAT[i] == -1)
            freeClusters++;
    }
    cout << "Free size in clusters: " << freeClusters << endl;
}
