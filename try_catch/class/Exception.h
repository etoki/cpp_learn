// Exception.h
#ifndef __EXCEPTION_H__010330_0353_57951860__INCLUDED__
#define __EXCEPTION_H__010330_0353_57951860__INCLUDED__

class CException
{
private:
    int m_nErrorCode;
public:
    CException(unsigned nErrorCode);
    virtual ~CException();
public:
    virtual unsigned GetErrorCode();
    virtual void DispErrorString();
};

inline CException::CException(unsigned nErrorCode)
{
    m_nErrorCode = nErrorCode;
}

#endif // #ifndef __EXCEPTION_H__010330_0353_57951860__INCLUDED__

