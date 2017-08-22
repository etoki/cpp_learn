#include <iostream>
#include <stdio.h>

using namespace std;

void f(int x, int y)
{
    char szResult[50];

    sprintf(szResult, "f(%d, %d) = %d", x, y, 2 * x + y);
    cout << szResult << endl;
}

int main()
{
    f(1, 2);
    f(182, 144);
    return 0;
}
