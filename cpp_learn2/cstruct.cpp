#include <iostream>

using namespace std;

class CInt
{
public:
    int m_num;

    CInt();
    CInt(int num);
    void Disp();
};

// コンストラクタ
// オーバーロードしています（第１部第６４章参照）
CInt::CInt()       { m_num = 0; }
CInt::CInt(int num){ m_num = num; }

// メンバ変数の表示
void CInt::Disp(){ cout << m_num << endl; }

int main()
{
    // 宣言と同時に初期値の設定
    CInt a, b(100), c(40);

    a.Disp();
    b.Disp();
    c.Disp();

    return 0;
}
