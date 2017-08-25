// FileException.h
#ifndef __FILEEXCEPTION_H__010330_0427_30194800__INCLUDED__
#define __FILEEXCEPTION_H__010330_0427_30194800__INCLUDED__

#include "Exception.h"
sing namespace std;

class CFileException : public CException
{
public:
    CFileException(unsigned nErrorCode);
public:
    virtual unsigned GetErrorCode();
    virtual void DispErrorString();

    // エラーコード
public:
    enum ErrorCode
    {
        NoError,
        NotOpened,
        FailToRead,
        FailToWrite,
        LastError,
    };
};

//constructor
inline CFileException::CFileException(unsigned nErrorCode)
    : CException(nErrorCode)
{
}

#endif // #ifndef __FILEEXCEPTION_H__010330_0427_30194800__INCLUDED__

