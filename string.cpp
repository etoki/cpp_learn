#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    int i;
    char szHello[] = "やぁ、こんちは";

    cout << "「" << szHello << "」を文字コードに変えると、" << endl;

    for(i = 0; i < strlen(szHello); i++)
        cout << (int)(unsigned char)szHello[i] << ", ";

    cout << (int)(unsigned char)szHello[i] << endl
         << "になります。" << endl;

    return 0;
}
