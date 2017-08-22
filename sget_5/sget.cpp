#include <iostream>
#include <memory.h>
#include <stdlib.h>

#define ELEM(array)   (sizeof (array) / sizeof *(array))

using namespace std;

class CIntArray
{
public:
    int m_anum[100];

    CIntArray();

    int Get(const int index);
    void Set(const int index, const int value);
    void CheckIndex(const int index);

};

// コンストラクタ
CIntArray::CIntArray()
{
    cout << "construct start" << endl;
    // memset はバイト単位でデータを埋める関数です
    memset(m_anum, 0, sizeof m_anum);
}

// メンバへのアクセス関数
int CIntArray::Get(const int index)
{
    cout << "get start" << endl;
    CheckIndex(index);
    return m_anum[index];
}

void CIntArray::Set(const int index, const int value)
{
    cout << "set start" << endl;
    CheckIndex(index);
    m_anum[index] = value;
}

// インデックスのチェック
void CIntArray::CheckIndex(const int index)
{
    cout << "checkindex start" << endl
         << ELEM(m_anum) << endl
         << (unsigned int)index << endl;

    if((unsigned int)index < ELEM(m_anum)){
        cout << "test" << endl;
        return;
    }
    else{
        cout << "インデックスが不正です！" << endl
             << "値 : " << index << endl;
    }
    exit(1);
}

int main(){
    int n;
    cout << "n = ";
    cin >> n;
    CIntArray a;
    a.CheckIndex(n);
    return 0;
}
