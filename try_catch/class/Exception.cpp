// Exception.cpp
#include <iostream>
#include "Exception.h"
using namespace std;

CException::~CException(){
}

unsigned CException::GetErrorCode(){
    return m_nErrorCode;
}

void CException::DispErrorString(){
    cout << "An error has occured." << endl
         << "The error number is "
         << GetErrorCode() << '.' << endl;
}
