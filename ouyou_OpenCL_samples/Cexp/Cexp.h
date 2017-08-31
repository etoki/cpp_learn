//===================================================================
//
// (C)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//
// 例外クラス
//
//===================================================================
#ifndef __CEXPH__
#define __CEXPH__

#include <string>

//-------------------------------------------------------------------
class Cexp
{
private:
    std::string mMessage;

 public:
    Cexp(const std::string &msg) {mMessage=msg;}
    virtual ~Cexp() {};

    const std::string message() const { return mMessage; }
    const char * c_str() const { return mMessage.c_str(); }

};
//-------------------------------------------------------------------

#endif  /* __CEXPH__ */
