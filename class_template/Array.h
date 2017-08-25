// Array.h
#ifndef __ARRAY_H__INCLUDED__
#define __ARRAY_H__INCLUDED__

#include <iostream>
#include <stdlib.h>

using namespace std;

template <typename TYPE>
class CArray
{
    // メンバ変数
private:
    TYPE* m_pnum;    // 動的配列
    int   m_nNumOf;  // 配列の要素数

    // コンストラクタ・デストラクタ
public:
    explicit CArray(const int nNumOf);
    CArray(const CArray <TYPE> & rother);  // コピーコンストラクタ
    virtual ~CArray();

    // メンバへのアクセス関数
public:
    TYPE Get(const int index) const;
    void Set(const int index, const TYPE value);

    // メンバの参照
    TYPE& operator [](unsigned int index);
    TYPE& operator [](int index);
    TYPE operator [](unsigned int index) const;
    TYPE operator [](int index) const;

    operator const TYPE*() const;  // 配列の直接参照

    // インデックスのチェック
private:
    void CheckIndex(const int index) const;

    // その他の情報の取得
public:
    bool IsValid() const;  // m_pnum の値が有効かどうか
    int NumOf() const;     // 配列の要素数
    int SizeOf() const;    // 配列のサイズ

    // コピー
public:
    bool Copy(const CArray <TYPE> & rother);  // 配列のコピー
    CArray <TYPE> & operator =(const CArray <TYPE> & rother);
                                              // = 演算子のオーバーロード

    // 諸関数
private:
    void Init();     // メンバの初期化
    void Release();  // メモリの解放
};

// m_pnum の値が有効かどうか
// inline はやはり template の後に書きます
template <typename TYPE>
inline bool CArray <TYPE> ::IsValid() const
{
    return m_pnum != NULL;
}

// 配列の要素数
template <typename TYPE>
inline int CArray <TYPE> ::NumOf() const
{
    return m_nNumOf;
}

// 配列のサイズ
template <typename TYPE>
inline int CArray <TYPE> ::SizeOf() const
{
    // sizeof を使っていれば、
    // どんな型が来てもきちんとした値が得られます
    return m_nNumOf * sizeof *m_pnum;
}

// 配列の直接参照
// TYPE 型への配列を返すので、
// もちろんキャスト演算子は (const TYPE*) となります
template <typename TYPE>
inline CArray <TYPE> ::operator const TYPE*() const
{
    return m_pnum;
}

// コンストラクタ
template <typename TYPE>
CArray <TYPE> ::CArray(const int nNumOf)
{
    // TYPE 型で new します
    // クラスの配列を作るときでもコンストラクタが呼ばれます
    m_pnum = new TYPE[nNumOf];
    if(m_pnum == NULL)
        m_nNumOf = 0;
    else
        m_nNumOf = nNumOf;

    // 0 での初期化は行わないことにします
    // クラスの配列を扱う際に問題になるからです
}

// コピーコンストラクタ
template <typename TYPE>
CArray <TYPE> ::CArray(const CArray <TYPE> & rother)
{
    Init();
    Copy(rother);
}

// デストラクタ
template <typename TYPE>
CArray <TYPE> ::~CArray()
{
    Release();
}

// メンバへのアクセス関数
template <typename TYPE>
TYPE CArray <TYPE> ::Get(const int index) const
{
    CheckIndex(index);
    return m_pnum[index];
}

template <typename TYPE>
void CArray <TYPE> ::Set(const int index, const TYPE value)
{
    CheckIndex(index);
    m_pnum[index] = value;
}

// インデックスのチェック
template <typename TYPE>
void CArray <TYPE> ::CheckIndex(const int index) const
{
    if((unsigned int)index < (unsigned int)m_nNumOf)
        return;

    cout << "インデックスが不正です！" << endl
         << "値 : " << index << endl;
    exit(1);
}

// メンバの参照
template <typename TYPE>
TYPE& CArray <TYPE> ::operator [](unsigned int index)
{
    CheckIndex(index);
    return m_pnum[index];
}

// メンバの参照
template <typename TYPE>
inline TYPE& CArray <TYPE> ::operator [](int index)
{
    // ちょっと強引でしょうか？
    return (*this)[(unsigned int)index];
}

// メンバの参照
template <typename TYPE>
TYPE CArray <TYPE> ::operator [](unsigned int index) const
{
    CheckIndex(index);
    return m_pnum[index];
}

// メンバの参照
template <typename TYPE>
inline TYPE CArray <TYPE> ::operator [](int index) const
{
    return (*this)[(unsigned int)index];
}

// 配列のコピー
template <typename TYPE>
bool CArray <TYPE> ::Copy(const CArray <TYPE> & rother)
{
    // 自分自身はコピーしない
    if(this == &rother)
        return true;

    Release();  // 配列が確保されているときはメモリを解放

    if(rother.IsValid() == true)
    {
        m_pnum = new TYPE[rother.NumOf()];
        if(m_pnum == NULL)
        {
            m_nNumOf = 0;
            return false;
        }
        m_nNumOf = rother.m_nNumOf;

        // コピーは = 演算子を使って行います
        // クラスを扱うときに問題になるからですね
        for(int i = 0; i < m_nNumOf; i++)
            m_pnum[i] = rother[i];
    }

    return true;
}

// = 演算子によるコピー
// もちろんテンプレート引数は戻り値の型にも必要です
template <typename TYPE>
CArray <TYPE> & CArray <TYPE> ::operator =(const CArray <TYPE> & rother)
{
    Copy(rother);
    return *this;
}

// メンバの初期化
template <typename TYPE>
void CArray <TYPE> ::Init()
{
    m_pnum   = NULL;
    m_nNumOf = 0;
}

// メモリの解放
// 配列が確保されているときだけメモリを解放します
// 解放した後はメンバを初期化します
template <typename TYPE>
void CArray <TYPE> ::Release()
{
    if(IsValid() == true)
    {
        delete [] m_pnum;
        Init();
    }
}

#endif // #ifnde __ARRAY_H__INCLUDED__

