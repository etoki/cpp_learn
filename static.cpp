// Static1.cpp
#include <iostream>
#include <memory.h>   // memset を使うためのヘッダファイルです

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// アドレスが常に一定かのチェック
// 前章で行った方法と同じですね

void AddressCheck_Sub()
{
    static int a;
    int        b;
    cout << "&a = " << &a << endl;
    cout << "&b = " << &b << endl;
}

void AddressCheck_Sub2()
{
    cout << "in Sub2" << endl;
    AddressCheck_Sub();
}

void AddressCheck()
{
    cout << "アドレスが常に一定かのチェック" << endl;

    AddressCheck_Sub();
    AddressCheck_Sub2();
    AddressCheck_Sub();
}

////////////////////////////////////////////////////////////////////////////////
// 初期化が１回しかされないかのチェック
// 第９章でやったのと同じ方法ですね

void InitOnceCheck_Sub()
{
    static int a = 3;
    int        b = 3;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    a++;
    b++;
}

void InitOnceCheck()
{
    cout << endl << "初期化が１回しかされないかのチェック" << endl;

    InitOnceCheck_Sub();
    InitOnceCheck_Sub();
}

////////////////////////////////////////////////////////////////////////////////
// ０で初期化されるかのチェック
// 偶然０だったときのために、４個くらい変数を作って確かめます
// 自動変数も一緒に確認してみます

void ZeroInitCheck_Sub()
{
    static int a, b, c, d;
    cout << a << ',' << b << ',' << c << ',' << d << endl;
}

void ZeroInitCheck_Sub2()
{
    int a, b, c, d;
    cout << a << ',' << b << ',' << c << ',' << d << endl;
}

void ZeroInitCheck()
{
    cout << endl << "０で初期化されるかのチェック" << endl;

    ZeroInitCheck_Sub();
    ZeroInitCheck_Sub2();
}

////////////////////////////////////////////////////////////////////////////////
// 値が保証されるかのチェック

// int*& は、int* への参照です
void HoldValueCheck_Sub(int*& pa, int*& pb)
{
    static int a = 9;
    int        b = 9;
    pa = &a;
    pb = &b;
}

// 大きな変数を作って、それを０で埋めます
// memset はメモリを指定した数値で埋める関数です
void HoldValueCheck_Sub2()
{
    int dummy[100];
    memset(dummy, 0, sizeof dummy);
}

void HoldValueCheck()
{
    cout << endl << "値が保証されるかのチェック" << endl;

    int *pa, *pb;
    HoldValueCheck_Sub(pa, pb);
    HoldValueCheck_Sub2();       // この関数を呼んでも値は保証されているかな？
    cout << "*pa = " << *pa << endl;
    cout << "*pb = " << *pb << endl;
}

////////////////////////////////////////////////////////////////////////////////
// main 関数

int main()
{
    AddressCheck();
    InitOnceCheck();
    ZeroInitCheck();
    HoldValueCheck();

    return 0;
}
