#ifndef __INTARRAY_H_INCLUDED__
#define __INTARRAY_H_INCLUDED__
#include <cstddef>

class CIntArray
{
    // メンバ変数
private:
    int* m_pnum;   // 動的配列
    int  m_nNumOf; // 配列の要素数

    // コンストラクタ・デストラクタ
public:
    CIntArray(const int nNumOf);
    CIntArray(const CIntArray& rother);  // コピーコンストラクタ
    ~CIntArray();

    // メンバへのアクセス関数
public:
    int Get(const int index) const;
    void Set(const int index, const int value);

    // インデックスのチェック
private:
    void CheckIndex(const int index) const;

    // コピー
public:
    bool Copy(const CIntArray& rother);  // 配列のコピー
    //void operator =(const CIntArray& rother); // = 演算子によるコピー
    CIntArray& operator =(const CIntArray& rother); // 26章

    // 諸関数
private:
    void Init();     // メンバの初期化
    void Release();  // メモリの解放

    // その他の情報の取得
public:
    // Success 関数の名前を IsValid に変更して下さい
    bool IsValid() const;  // m_pnum の値が有効かどうか
//    bool Success() const;  // メモリの確保が成功したか

    int NumOf() const;     // 配列の要素数
    int SizeOf() const;    // 配列のサイズ
};

// メモリの確保が成功したか
//inline bool CIntArray::Success() const
inline bool CIntArray::IsValid() const
{
    return m_pnum != NULL;
}

// 配列の要素数
inline int CIntArray::NumOf() const
{
    return m_nNumOf;
}

// 配列のサイズ
inline int CIntArray::SizeOf() const
{
    return m_nNumOf * sizeof *m_pnum;
}

#endif

