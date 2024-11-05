#include "DirectoryEntry.h"
#include <cstring>
#include <algorithm>

Directory_Entry::Directory_Entry(string name, char attr, int firstCluster)
    : dir_attr(attr), dir_firstCluster(firstCluster), dir_fileSize(0)
{
    assignDir_Name(name);
    memset(dir_empty, 0, sizeof(dir_empty));
}

string Directory_Entry::cleanTheName(string s)
{
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

void Directory_Entry::assignFileName(string name, string extension)
{
    name = cleanTheName(name).substr(0, 8);
    extension = cleanTheName(extension).substr(0, 3);
    string fullName = name + "." + extension;
    strncpy(dir_name, fullName.c_str(), sizeof(dir_name) - 1);
    dir_name[sizeof(dir_name) - 1] = '\0';
}

void Directory_Entry::assignDir_Name(string name)
{
    name = cleanTheName(name).substr(0, 11);
    strncpy(dir_name, name.c_str(), sizeof(dir_name) - 1);
    dir_name[sizeof(dir_name) - 1] = '\0';
}
