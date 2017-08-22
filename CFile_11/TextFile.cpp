// TextFile.cpp
#include <iostream>
#include <stdio.h>
#include "File.h"
#include "MainDefs.h"
#include "TextFile.h"

using namespace std;

// １行読み出す関数
// 改行コードは切り捨てられます
// 戻り値は文字列の長さになります
// バッファに入りきらないとき（オーバーフロー）は TF_OVERFLOW を返します
int CTextFile::ReadLine(char* buffer, size_t nSize){
    if(IsValid() == false)
        return EOF;
    if(fgets(buffer, nSize, m_pfile) == NULL)
        return EOF;

    size_t nLength = strlen(buffer);

    // 最後に改行のない場合は、オーバーフローか、改行のない最後の行です
    // 改行のない最後の行がたまたまバッファの長さ－１の長さの時もオーバーフローと判定します
    if(buffer[nLength - 1] != '\n')
        return (nLength == nSize - 1) ? TF_OVERFLOW : nLength;

    nLength -= 1;
    buffer[nLength] = 0;
    return nLength;
};

// ファイルを開く
//bool CTextFile::Open(const char* pszFile, const char* pszFlags)
//{
//    cout << "CTextFile::Open" << endl;
//    Close();
//    char bufFlags[8];
//    if(ModifyFlags(pszFlags, bufFlags, numof(bufFlags)) == false)
//        return false;
//    m_pfile = fopen(pszFile, bufFlags);
//    return (m_pfile != NULL);
//};

// フラグの調整
bool CTextFile::ModifyFlags(const char* pszSource, char* pszDest, int nSize)
{
    cout << "CTextFile::ModifyFlags" << endl;

    bool bBinary;  // 'b' の指定があるかどうか

    bBinary = (strchr(pszSource, 'b') != NULL);

    // フラグが多すぎるときは false を返す
    if((int)strlen(pszSource) - bBinary > nSize - 1)
        return false;

    // 強制的にテキストモードで開きます
    // 'b' 以外の文字だけコピーします
    for(; *pszSource; pszSource++)
    {
        if(*pszSource != 'b')
        {
            *pszDest = *pszSource;
            pszDest++;
        }
    }
    return true;
};

