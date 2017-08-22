// TestFile.cpp
#include <iostream>
#include "MainDefs.h"
#include "File.h"
#include "TextFile.h"

using namespace std;

bool Open(CFile& rfile, const char* pszFlags){
    char buffer[512];

    cout << "ファイル名を指定して下さい > " << flush;
    cin >> buffer;
    return rfile.Open(buffer, pszFlags);
}

void Write(CTextFile& rtxt){
    char buffer[512];

    cout << "何を書き込みますか > " << flush;
    cin >> buffer;
    rtxt.WriteString(buffer);
}

void Load(CFile& rbin){
    char buffer[512];
    int  nRead;

    nRead = rbin.Read(buffer, numof(buffer));
    buffer[nRead] = 0;
    cout << buffer << endl;
}

void Viss(const int num){
    cout << "Viss : No." << num << endl;
}

int main(){
    CFile     bin;
    CTextFile txt;

//    char      buffer[128];
//    int       nRead;

//    if(txt.Open("Test.txt", "w") == false)
//        return 0;
//    txt.WriteString("色は匂へど散りぬるを");
//    txt.Close();

//    if(bin.Open("Test.txt", "r") == false)
//        return 0;
//    nRead = bin.Read(buffer, sizeof buffer - 1);
//    buffer[nRead] = 0;
//    cout << buffer << endl;

//    Viss(1);
//    bin.Open("Test.txt", "r");
//    Viss(2);
//    txt.Open("Test.txt", "r");
//    Viss(3);

    if(Open(txt, "w") == false)
        return 0;
    Write(txt);
    txt.Close();

    if(Open(bin, "r") == false)
        return 0;
    Load(bin);
    bin.Close();

    return 0;
}

