#pragma once
#include"DirectoryEntry.h"

class Converter
{
public:
    static vector<char> intToByte(int n);

    static int byteToInt(vector<char> bytes);

    static vector<char> intArrayToByteArray(int* ints, int size);

    static void byteArrayToIntArray(int* ints, vector<char> bytes);

    static vector<vector<char>> splitBytes(vector<char> bytes);

    static Directory_Entry BytesToDirectory_Entity(vector<char> bytes);

    static vector<char> Directory_EntityToBytes(Directory_Entry d);
};