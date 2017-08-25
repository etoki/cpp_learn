#include <iostream>
using namespace std;

void Func()
{
    char* buf1 = NULL;
    char* buf2 = NULL;

    try
    {
        buf1 = new char[0x7FFFFFFF];
        cout << buf1 << endl;
        if(buf1 == NULL) //trueのためthrow
            cout << "buf1 test" << endl;
            throw 1;

        buf2 = new char[0x7FFFFFFF];
        cout << buf2 << endl;
        if(buf2 == NULL)
            cout << "buf2 test" << endl;
            throw 2;
    }
    catch(int fError)//まずここを通る
    {
        switch(fError)
        {
        case 2:
            cout << "case2 test" << endl;
            delete [] buf1;
        }
        cout << "throw test" << endl;//ここが次に通る
        throw;//値は今処理している例外と同じものを投げます
    }

    delete [] buf1;
    delete [] buf2;
}

int main()
{
    try
    {
        Func();
    }
    catch(int fError)
    {
        cout << "エラー終了しました。エラーコード "
             << fError << endl;//最後にここが通る
    }

    return 0;
}
