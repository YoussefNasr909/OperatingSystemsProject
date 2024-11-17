#pragma once
#include "DirectoryEntry.h"
#include <vector>
#include <string>
using namespace std;

class Converter
{
public:
    static vector<char> intToByte(int n);
    static int byteToInt(vector<char> bytes);
    static vector<char> intArrayToByteArray(int* ints, int size);
    static void byteArrayToIntArray(int* ints, vector<char> bytes);
    static vector<vector<char>> splitBytes(vector<char> bytes);
    static Directory_Entry BytesToDirectory_Entry(vector<char> bytes);
    static vector<char> Directory_EntryToBytes(Directory_Entry d);

    // New methods
    static vector<Directory_Entry> BytesToDirectory_Entries(vector<char> bytes);
    static vector<char> Directory_EntriesToBytes(vector<Directory_Entry> d);
};
