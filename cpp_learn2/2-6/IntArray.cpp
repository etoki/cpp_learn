#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include "IntArray.h"

using namespace std;

// コンストラクタ
CIntArray::CIntArray(const int nNumOf)
{
    m_pnum = new int[nNumOf]; //nNumOf個数分のメモリ確保
    if(m_pnum == NULL)
       m_nNumOf = 0;
    else
    {
        m_nNumOf = nNumOf;
//        cout << sizeof *m_pnum << endl; //4
//        cout << sizeof m_pnum << endl; //8
        memset(m_pnum, 0, nNumOf * sizeof *m_pnum);
        cout << m_pnum << endl //ランダムな値
             << sizeof *m_pnum << endl; //4
    }

    cout << "コンストラクタが呼ばれました。"
         << "要素数は " << m_nNumOf << " です。" << endl;
}

// デストラクタ
CIntArray::~CIntArray()
{
    if(m_pnum != NULL)
        delete [] m_pnum;

    cout << "デストラクタが呼ばれました。"
         << "要素数は " << m_nNumOf << " でした。" << endl;
}

// メンバへのアクセス関数
int CIntArray::Get(const int index)
{
    cout << "get start" << endl;
    CheckIndex(index);
    return m_pnum[index];
}

void CIntArray::Set(const int index, const int value)
{
    cout << "set start" << endl;
    CheckIndex(index);
    m_pnum[index] = value;
}

// インデックスのチェック
void CIntArray::CheckIndex(const int index)
{
    if((unsigned int)index < (unsigned int)m_nNumOf)
        return;

    cout << "インデックスが不正です！" << endl
         << "値 : " << index << endl;
    exit(1);
}
