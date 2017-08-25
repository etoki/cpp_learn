// FileException.cpp
#include <iostream>
#include "FileException.h"
using namespace std;

unsigned CFileException::GetErrorCode()
{
    unsigned nErrorCode = CException::GetErrorCode();
    return (nErrorCode >= LastError ? NoError : nErrorCode);
}

void CFileException::DispErrorString()
{
    static const char* apszError[] = {
        "予期しないエラーです。",
        "ファイルが開かれていません。",
        "ファイルからの読み出しに失敗しました。",
        "ファイルへの書き込みに失敗しました。",
    };

    cout << apszError[GetErrorCode()] << endl;
}
