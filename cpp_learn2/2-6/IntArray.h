#ifndef __INTARRAY_H_INCLUDED__
#define __INTARRAY_H_INCLUDED__

class CIntArray
{
    // メンバ変数
private:
    int* m_pnum;   // 動的配列
    int  m_nNumOf; // 配列の要素数

    // コンストラクタ・デストラクタ
public:
    CIntArray(const int nNumOf);
    ~CIntArray();

    // メンバへのアクセス関数
public:
    int Get(const int index);
    void Set(const int index, const int value);

    // インデックスのチェック
private:
    void CheckIndex(const int index);
};

#endif

