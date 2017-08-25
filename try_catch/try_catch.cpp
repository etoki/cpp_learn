#include <iostream>
using namespace std;

//throw 文は「例外」を発生させるための命令です。
//この「例外」というものが try ブロック内で発生すると、catch ブロックに飛びます。

int main(){
    while(true)
    {
        try
        {
            int type;
            cin >> type;

            if(type == 0)
                return 0;
            else if(type == 1)
                throw 1;
            else
                throw "例外発生！";
        }
        catch(int fError)
        {
            cout << fError << endl;
        }
        catch(char const* fError)
        {
            cout << fError << endl;
        }
    }
    // 一応 return 0; を書いておきます
    return 0;
}
