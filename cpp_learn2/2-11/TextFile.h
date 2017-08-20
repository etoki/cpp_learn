// TextFile.h
#ifndef __TEXTFILE_H__INCLUDED__
#define __TEXTFILE_H__INCLUDED__

#include <stdio.h>
#include <string.h>
#include "File.h"

using namespace std;

class CTextFile : public CFile{
public:
    int WriteString(const char* pszString);  // 文字列を書き込む
   int ReadLine(char* buffer, int nSize);   // １行読み出す関数
};

// 文字列を書き込む
inline int CTextFile::WriteString(const char* pszString){
    return Write(pszString, strlen(pszString));
}

#endif
