//31章から35章
#include <iostream>
#include <string.h>

using namespace std;
/*
template <typename TYPE>
void Disp(TYPE value){
    cout << value << endl;
};
*/
template <typename TYPE>
TYPE Max(TYPE a, TYPE b){
    return (a > b) ? a : b;
}

// 大きい方の値を返す関数（参照渡し）
template <typename TYPE>
TYPE& MaxR(TYPE& a, TYPE& b){
    return (a > b) ? a : b;
}

// 文字列用の Max （char* 用）36章
template < >
char* Max<char*>(char* a, char* b){
    return (strcmp(a, b) > 0) ? a : b;
}

// 文字列用の Max （const char* 用）36章
template < >
const char* Max<const char*>(const char* a, const char* b){
    return (strcmp(a, b) > 0) ? a : b;
}

class CStr //35章
{
private:
    char m_str[128];

public:
    CStr(const char* str){
        // strncpy はバッファサイズを考慮する
        // 文字列コピー関数です。1つ目に2つ目を3つ目分コピー。
        cout << "m_str is "  << m_str[0] << endl
             << "str is " << str << endl
             << "sizeof m_str " << sizeof m_str << endl;

        strncpy(m_str, str, sizeof m_str);

        cout << "m_str is "  << m_str[0] << endl
             << "str is " << str << endl
             << "sizeof m_str " << sizeof m_str << endl;

        // バッファサイズを越えるものをコピーするときは、
        // 最後にヌルターミネータは付きません
        m_str[(sizeof m_str) - 1] = 0;
    }
    const char* Get(){ return m_str; }
    //operatorがわからん
    bool operator > (CStr& rother){
        return strcmp(m_str, rother.m_str) > 0;
    }
};

int main(){
/*
    int b = 1;
    Disp(b);

    void* p;
    char  str[100];
    int   num;
    //voidであいまいな型を制御する
    p = Max<void*>(str, &num);
    cout << "より大きいアドレスは "
         << "strは" << &str << "、&numは" << &num << endl
         << p << " です。" << endl;

//35
    CStr str2("Would you like to program?"),
         str1("Would you love to program?");
    cout << MaxR(str1, str2).Get() << endl; //MaxRはテンプレートなのにそこからGetって？
*/
//36

    // str[1] は str[0] よりもアドレスは大きい
    // str[][num]の書き方がわからない
    char str[][32] = {
        "This is a pen.", "This is a book."
    };

    // Max<char*> が呼ばれます
    cout << Max(str[0], str[1]) << endl;

    // Max<const char*> で呼びます
    cout << Max<const char*>(str[0], "This is a zoo.") << endl;

    return 0;
};

/*
// 大きい方の値を返す関数（int 用）
int Max(int a, int b){ return (a > b) ? a : b; }

// 大きい方の値を返す関数（unsigned int 用）
unsigned int Max(unsigned int a, unsigned int b){
    return (a > b) ? a : b;}

// 大きい方の値を返す関数（void* 用）
void* Max(void* a, void* b){ return (a > b) ? a : b; }

//これをテンプレートにまとめると
template <typename TYPE> TYPE Max(TYPE a, TYPE b){
    return (a > b) ? a : b;}
*/

