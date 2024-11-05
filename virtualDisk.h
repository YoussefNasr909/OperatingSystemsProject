#pragma once
#include <fstream>
#include<iostream>
#include <string>
#include<vector>
using namespace std;
class virtualDisk
{
public:

    static void createOrOpenDisk(const string& path);
    static void writeDataCluster(const vector<char>& cluster, int clusterIndex);
    static vector<char> readDataCluster(int clusterIndex);
    static bool isNew();

private:
    static fstream Disk;


};