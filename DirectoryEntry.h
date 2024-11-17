#pragma once
#include "Mini_FAT.h"
#include <string>
using namespace std;

class Directory_Entry
{
public:
	char dir_name[11];
	char dir_attr;
	char dir_empty[12];
	int dir_firstCluster;
	int dir_filesize;

	Directory_Entry(string name, char dir_attr, int dir_firstCluster);
	string cleanTheName(string s);
	void assignFileName(string name, string extension);
	void assignDir_Name(string name);
};
