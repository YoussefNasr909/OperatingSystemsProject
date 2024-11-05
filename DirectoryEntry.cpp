#include "DirectoryEntry.h"
#include <cstring>
#include <algorithm>

Directory_Entry::Directory_Entry(string name, char attr, int firstCluster)
    : dir_attr(attr), dir_firstCluster(firstCluster), dir_fileSize(0)
{
    assignDir_Name(name);
    memset(dir_empty, 0, sizeof(dir_empty));
    /*
    دالة في مكتبة cstring 
    بتستخدم لتحديد او تعبئة مكان معين في الذاكرة بقيمة محددة هنا استخدمناها
    عشان نعبي الاراي بتاع داير ايمبتي بصفر كده بنضمن ان القيمة المبدئية 
    جواها بتساوي صفر عشان نضمن ان الاراي فاضية
    sizeof هو عدد البايتات الي هنملاها بالقيمة صفر
    */
    
}

string Directory_Entry::cleanTheName(string s)
{
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
    /*
    ترانسفورم تبع مكتبة algorithm 
    هنا بتحول حروف اسم الملف لحروف كبيرة uppercase
    toupper الدالة الي بتحول الحرف الواحد لحرف كبير
    */
}

void Directory_Entry::assignFileName(string name, string extension)
{
    name = cleanTheName(name).substr(0, 8);
    extension = cleanTheName(extension).substr(0, 3);
    string fullName = name + "." + extension;
    strncpy(dir_name, fullName.c_str(), sizeof(dir_name) - 1);
    dir_name[sizeof(dir_name) - 1] = '\0';
    /*
    strncpy تبع مكتبة cstring
    بتستخدم لنسخ جزء من النص او عدد معين من الحروف من مكان لمكان تاني في الذاكرة 
    dir nameهنا بنستخدمها عشان ننسخ اسم الملف ل
    sizeof هنا بيضمن اننا مش هننسخ اكتر من الحد الاقصي للأراي 
    */
}

void Directory_Entry::assignDir_Name(string name)
{
    name = cleanTheName(name).substr(0, 11);
    strncpy(dir_name, name.c_str(), sizeof(dir_name) - 1);
    dir_name[sizeof(dir_name) - 1] = '\0';
}
