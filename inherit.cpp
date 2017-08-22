// Inherit1.cpp
//派生クラスのコンストラクタを呼ぶ前に基底クラスのコンストラクタが呼ばれ、
//派生クラスのデストラクタが呼ばれた後に基底クラスのデストラクタが呼ばれる
//このとき、呼ばれるコンストラクタはデフォルトコンストラクタ

#include <iostream>

using namespace std;

class CA
{
public:
    CA()     { cout << "CA" << endl; }
    CA(int a){ cout << "CA:" << a << endl; }
    ~CA()    { cout << "~CA" << endl; }
};

class CB : public CA
{
public:
    CB() { cout << "CB" << endl; }
    ~CB(){ cout << "~CB" << endl; }
};

int main()
{
    CB b;

    return 0;
}
