#include <iostream>

using namespace std;

#define ELEM(array)  (sizeof (array) / sizeof *(array))

struct SStudent{
    char szName[16];
    int  nJapanese;
    int  nMath;
    int  nEnglish;
};

void Disp(const SStudent* pstudent){
    cout << "name: "      << (*pstudent).szName    << endl
         << "Japanese: "  << (*pstudent).nJapanese << " points, "
            "Math: "      << (*pstudent).nMath     << " points, "
            "English: "   << (*pstudent).nEnglish  << " points" << endl;
}

void Disp2(const SStudent* pstudent)
{
    cout << "name: "     << pstudent->szName    << endl
         << "Japanese: " << pstudent->nJapanese << " points, "
            "Math: "     << pstudent->nMath     << " points, "
            "English: "  << pstudent->nEnglish  << " points" << endl;
}

int main(){
    SStudent students[] = {
        { "taro",   73, 98, 86, },
        { "tanaka", 64, 45, 40, },
        { "kiyoshi",   76, 78, 69, },
    };
    int      i;
    const SStudent* pstudent;

    // printf("%d\n", sizeof (students)); //84
    // printf("%d\n", sizeof *(students)); //28
    // printf("%d\n", ELEM(students)); //3
    // printf("%d\n", &students[0]); //6422220, start address
    // printf("%d\n", &students[0].nJapanese); //6422236, address
    // printf("%d\n", students[0].nJapanese); //73, value

    printf("%d\n", (*pstudent[0]).nJapanese); //1077735557, value

    // for(i = 0; i < ELEM(students); i++)
    //     Disp(&students[i]);

    return 0;
}
