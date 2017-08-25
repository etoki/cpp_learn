#include <iostream>
#include "IntArray.h"

using namespace std;

void Viss(int num){
    cout << "Viss : No." << num << endl;
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

    return 0;
}
