#include <iostream>

using namespace std;

void Func();

int main()
{
    extern int a;

    Func();
    a = 5;
    Func();

    return 0;
}
