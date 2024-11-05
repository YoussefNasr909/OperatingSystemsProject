#include "Converter.h"
#include <cstring>
vector<char> Converter::intToByte(int n)
{
    vector<char> bytes(sizeof(int));
    memcpy(bytes.data(), &n, sizeof(int));
    return bytes;
}
int Converter::byteToInt(vector<char> bytes)
{
    int n = 0;
    memcpy(&n, bytes.data(), sizeof(int));
    return n;
}
vector<char> Converter::intArrayToByteArray(int* ints, int size)
{
    vector<char> bytes(size * sizeof(int));
    memcpy(bytes.data(), ints, size * sizeof(int));
    return bytes;
}
void Converter::byteArrayToIntArray(int* ints, vector<char> bytes)
{
    memcpy(ints, bytes.data(), bytes.size());
}
vector<vector<char>> Converter::splitBytes(vector<char> bytes)
{
    vector<vector<char>> result;
    int chunkSize = sizeof(Directory_Entry); // Size of each chunk

    for (size_t i = 0; i < bytes.size(); i += chunkSize)
    {
        vector<char> chunk(bytes.begin() + i, bytes.begin() + i + chunkSize);
        result.push_back(chunk);
    }

    return result;
}
Directory_Entry Converter::BytesToDirectory_Entity(vector<char> bytes)
{
    Directory_Entry dirEntry("", 0, 0);
    memcpy(&dirEntry, bytes.data(), sizeof(Directory_Entry));
    return dirEntry;
}
vector<char> Converter::Directory_EntityToBytes(Directory_Entry d)
{
    vector<char> bytes(sizeof(Directory_Entry));
    memcpy(bytes.data(), &d, sizeof(Directory_Entry));
    return bytes;
}
