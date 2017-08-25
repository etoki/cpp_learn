// SMember5.cpp
#include <iostream>

using namespace std;

// たまには TYPE 以外の名前にしてみました
template <typename SOMETHING>
class CSomething
{
private:
    static int something;
public:
    static void Disp();
};

// テンプレートの静的メンバ変数も
// こんな感じで実体定義したのでいいのかな？
// とやってみます
template <typename SOMETHING>
int CSomething <SOMETHING> ::something = 0;

// something のアドレスの表示
template <typename SOMETHING>
void CSomething <SOMETHING> ::Disp()
{
    cout << "アドレス : " << &something << endl;
}

int main()
{
    // 実体が一度も使われませんという警告が出ますが
    // ここでは無視して構いません
    CSomething <int>   st1;
    CSomething <char>  st2;
    CSomething <void*> st3;
    CSomething <int>   st4;
    CSomething < CSomething <int> > st5;

    // 静的メンバ関数は
    // 別に実体を使って呼んでもいいのでしたね
    st1.Disp();
    st2.Disp();
    st3.Disp();
    st4.Disp();
    st5.Disp();

    return 0;
}

