// File.h
#ifndef __FILE_H__INCLUDED__
#define __FILE_H__INCLUDED__

#include <stdio.h>

using namespace std; 

class CFile
{
    // メンバ変数
protected:
    FILE* m_pfile;  // ファイル

private:
    bool  m_bCopy;  // コピーコンストラクタで作られたかどうか

    // コンストラクタ・デストラクタ
public:
    CFile();                     // コンストラクタ
    CFile(const CFile& rother);  // コピーコンストラクタ
    ~CFile();                    // デストラクタ

    // ファイルの開閉
public:
    bool Open(const char* pszFile, const char* pszFlags);
                   // ファイルを開く
    void Close();  // ファイルを閉じる

private:
    bool ModifyFlags(const char* pszSource, char* pszDest, int nSize);
                   // フラグの調整

    // ファイルの操作
public:
    size_t Read(void* pData, size_t nSize);         // ファイルから読み出す
    size_t Write(const void* pData, size_t nSize);  // ファイルに書き込む

    // 評価
public:
    bool IsValid() const;  // m_pfile の値が有効かどうか
    bool Eof();  // ファイルの終端に達したかどうか
};

// m_pfile の値が有効かどうか
inline bool CFile::IsValid() const{
    return (m_pfile != NULL);
};

#endif
