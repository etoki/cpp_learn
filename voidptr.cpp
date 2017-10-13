#include <iostream>

using namespace std;

int main()
{
    char   c = 1;
    int    i = 2;
    double d = 3;
    void*  p;

    p = &c;
    cout << *(char*)p << endl;

    p = &i;
    cout << *(int*)p << endl;

    p = &d;
    cout << *(double*)p << endl;

/*
    p = &c;
    cout << *p << endl;

    p = &i;
    cout << *p << endl;

    p = &d;
    cout << *p << endl;
*/
    return 0;
}
