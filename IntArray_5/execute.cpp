#include <iostream>
#include "IntArray.h"

using namespace std;

void Viss(int num){
    cout << "Viss : No." << num << endl;
}

void DispArray(const CIntArray& array)
{
    int i;

    cout << "要素数 : " << array.NumOf() << endl;
    cout << "要素 : ";
    for(i = 0; i < array.NumOf(); i++)
        cout << array[i] << ' ';
    cout << endl << endl;
}

CIntArray a(100);

int main()
{
    CIntArray b(20);
    Viss(1);

    CIntArray c(30);
    Viss(2);

    {
        CIntArray d(40);
        Viss(3);
// デストラクタ要素数40。dの寿命が先に来るため。
    }
    Viss(4);
// デストラクタ要素数30
// デストラクタ要素数20
// デストラクタ要素数10

    CIntArray a1(1), a2(1), a3(10);
    int       i;

    for(i = 0; i < a3.NumOf(); i++)
        a3[i] = i;

    a1 = a2 = a3;  // 例の代入
//    cout << a1 << endl
//         << a2 << endl
//         << a3 << endl;

    DispArray(a1);
    DispArray(a2);
    DispArray(a3);

    return 0;
}
