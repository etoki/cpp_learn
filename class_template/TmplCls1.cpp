// TmplCls1.cpp
#include <stdio.h>
#include "Array.h"

using namespace std;

int main()
{
    static const char str1[] = "Ｓｃｈｒｏｅｄｉｎｇｅｒの猫";
    CArray <char> str2(sizeof str1);
    CArray <char> str3(0);
    int i;

    for(i = 0; i < str2.NumOf(); i++)
        str2[i] = str1[i];
    cout << str2 << endl;

    str3 = str2;
    str3[sizeof str1 - 3] = (char)('犬' >> 8);
    str3[sizeof str1 - 2] = (char)('犬' & 0xFF);
    printf("%s\n", (const char*)str3);

    return 0;
}
