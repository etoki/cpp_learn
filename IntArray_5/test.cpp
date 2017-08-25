#include <iostream>
#include "IntArray.h"

using namespace std;

void DispArray(const CIntArray& array)
{
    int i;

    cout << "要素数 : " << array.NumOf() << endl;
    cout << "要素 : ";
    for(i = 0; i < array.NumOf(); i++)
        cout << array[i] << ' ';
    cout << endl << endl;
}

int main()
{
    CIntArray a1(1), a2(1), a3(10);
    int       i;

    for(i = 0; i < a3.NumOf(); i++)
        a3[i] = i;

    a1 = a2 = a3;  // 例の代入

    DispArray(a1);
    DispArray(a2);
    DispArray(a3);

    return 0;
}
