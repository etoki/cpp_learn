#include <iostream>

using namespace std;

void DispSize(const char* array)
{
    cout << "この配列のサイズは " << sizeof array
         << " バイトです。" << endl;
}

int main(){
  int a = 1;
  char b = 1;
  char data[100];
  int data2[100];
  cout << sizeof data << endl //100
       << sizeof a << endl //4
       << sizeof b << endl //1
       << &data << endl //0x7ffc534aad80
       << sizeof &data << endl; //8
  DispSize(data);
  cout << sizeof data2 << endl //400
       << sizeof *data2 << endl //4
       << *data2 << endl //ランダムな値
       << sizeof data2 / sizeof *data2 << endl;
  return 0;
}

