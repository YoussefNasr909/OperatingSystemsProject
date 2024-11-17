#include "Converter.h"

vector<char> Converter::intToByte(int n)
{
    vector<char> result;
    result.push_back(((n >> 24) & 0xFF));
    result.push_back(((n >> 16) & 0xFF));
    result.push_back(((n >> 8) & 0xFF));
    result.push_back((n & 0xFF));
    return result;
}


int Converter::byteToInt(vector<char> bytes)
{
    int n = 0;
    for (int i = 0; i < bytes.size(); ++i)
    {
        n = (n << 8) | (bytes[i] & 0xFF);
    }
    return n;
}

vector<char> Converter::intArrayToByteArray(int* ints, int size)
{
    vector<char> bytes;
    for (int i = 0; i < size; i++)
    {
        vector<char> b = Converter::intToByte(ints[i]);
        bytes.insert(bytes.end(), b.begin(), b.end());
    }
    return bytes;
}


void Converter::byteArrayToIntArray(int* ints, vector<char> bytes)
{
    for (int i = 0, j = 0; i < bytes.size() && j < bytes.size() / sizeof(int); j++, i += 4)
    {
        vector<char> b;
        for (int k = i; k < (i + 4); k++)
        {
            b.push_back(bytes[k]);
        }
        ints[j] = Converter::byteToInt(b);
    }
}


vector<vector<char>> Converter::splitBytes(vector<char> bytes)
{
    vector<vector<char>> ls;
    if (bytes.size() > 0)
    {
        int number_of_arrays = bytes.size() / 1024;
        int rem = bytes.size() % 1024;
        for (int i = 0; i < number_of_arrays; i++)
        {
            vector<char> b;
            for (int j = i * 1024, k = 0; k < 1024; j++, k++)
            {
                b.push_back(bytes[j]);
            }
            ls.push_back(b);
        }
        if (rem > 0)
        {
            vector<char> b1(1024);
            for (int i = number_of_arrays * 1024, k = 0; k < rem; i++, k++)
            {
                b1.push_back(bytes[i]);
            }
            for (int i = rem; i < 1024; i++)
            {
                b1.push_back(0);
            }
            ls.push_back(b1);
        }
    }
    else
    {
        vector<char> b1(1024);
        for (int i = 0; i < 1024; i++)
        {
            b1.push_back(0);
        }
        ls.push_back(b1);
    }
    return ls;
}

vector<char> Converter::Directory_EntryToBytes(Directory_Entry d)
{
    vector<char> bytes(32);
    for (int j = 0; j < 11; j++)
    {
        bytes.push_back(d.dir_name[j]);
    }
    bytes.push_back(d.dir_attr);
    for (int i = 0; i < 12; i++)
    {
        bytes.push_back(d.dir_empty[i]);
    }
    vector<char> fc = Converter::intToByte(d.dir_firstCluster);
    for (int i = 0; i < fc.size(); i++)
    {
        bytes.push_back(fc[i]);
    }
    vector<char> sz = Converter::intToByte(d.dir_filesize);
    for (int i = 0; i < sz.size(); i++)
    {
        bytes.push_back(sz[i]);
    }
    return bytes;
}


Directory_Entry Converter::BytesToDirectory_Entry(vector<char> bytes)
{
    string name = "";
    for (int i = 0; i < 11; i++)
    {
        name += bytes[i];
    }
    char attr = bytes[11];
    char empty[12];
    int j = 12;
    for (int i = 0; i < 12; i++)
    {
        empty[i] = bytes[j];
        j++;
    }
    vector<char> fc(4);
    for (int i = 0; i < fc.size(); i++)
    {
        fc.push_back(bytes[j]);
        j++;
    }
    int firstcluster = Converter::byteToInt(fc);
    vector<char> sz(4);
    for (int i = 0; i < sz.size(); i++)
    {
        sz.push_back(bytes[j]);
        j++;
    }
    int filesize = Converter::byteToInt(sz);
    Directory_Entry d(name, attr, firstcluster);
    for (int i = 0; i < 12; i++)
    {
        d.dir_empty[i] = empty[i];
    }
    d.dir_filesize = filesize;
    return d;
}


vector<Directory_Entry> Converter::BytesToDirectory_Entries(vector<char> bytes)
{
    vector<Directory_Entry> DirsFiles;
    for (int i = 0; i < bytes.size(); i += 32)
    {
        vector<char> b(bytes.begin() + i, bytes.begin() + i + 32);
        if (b[0] == 0)
            break;
        DirsFiles.push_back(Converter::BytesToDirectory_Entry(b));
    }
    return DirsFiles;
}

vector<char> Converter::Directory_EntriesToBytes(vector<Directory_Entry> d)
{
    vector<char> bytes;
    for (int i = 0; i < d.size(); i++)
    {
        vector<char> b = Converter::Directory_EntryToBytes(d[i]);
        bytes.insert(bytes.end(), b.begin(), b.end());
    }
    return bytes;
}
