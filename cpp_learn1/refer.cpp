#include <iostream>

using namespace std;

int main()
{
    int a;
    int& r = a;

    a = 0;
    cout << "a  = " << a << endl
         << "r  = " << r << endl << endl;

    r = 100;
    cout << "a  = " << a << endl
         << "r  = " << r << endl << endl;

    cout << "&a = " << (unsigned int)&a << endl
         << "&r = " << (unsigned int)&r << endl;

    return 0;
}
