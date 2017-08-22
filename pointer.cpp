#include <iostream>

using namespace std;

void ChangeToShohwa(int* p)
{
    if(1926 < *p && *p < 1989)
        *p -= 1925;
    else
        *p = 0;
    // 参照するのでreturnして値を保存しなおす必要なし
}

void Shohwa()
{
    int nYear;

    cout << "西暦を入力して下さい > ";
    cin >> nYear;

    ChangeToShohwa(&nYear);
    if(nYear)
        cout << "その年は昭和 " << nYear
             << " 年です。" << endl;
    else
        cout << "その年は昭和ではありません。" << endl;
}

int main()
{
    int a;
    int* p;
    int* q;
    int* r;

    p = &a;
    q = p;
    r = q;
    cout << " p = " << (unsigned int) p << endl
         << " q = " << (unsigned int) q << endl
         << " r = " << (unsigned int) r << endl
         << "&a = " << (unsigned int)&a << endl;

    a = 0;
    cout << " a = " <<  a << endl
         << "*p = " << *p << endl; // same as "a"

    *p = 5;
    cout << " a = " <<  a << endl
         << "*p = " << *p << endl;

    Shohwa();
    Shohwa();

    return 0;
}
