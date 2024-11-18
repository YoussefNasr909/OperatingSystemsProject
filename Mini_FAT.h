#pragma once
#include"VirtualDisk.h"

class Mini_FAT
{
public:
	static int FAT[1024];
	static void initialize_FAT();
	static vector<char> createSuperBlock();
	static void writeFAT();
	static void readFAT();
	static void printFAT();
	static void setFAT(int fat_arr[1024]);
	static void initialize_Or_Open_FileSystem(string name);
	static int getAvilableClusters();
	static void setClusterPointer(int clusterIndex, int pointer);
	static int getClusterPointer(int clusterIndex);
	static void getFreeSize();

private:


};