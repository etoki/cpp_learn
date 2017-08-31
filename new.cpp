#include <iostream>

using namespace std;

int main(){
 int n;
 int *a_heap;
 int i;
 
 cout << "Please input...";
 cin >> n;
 
 float *b_heap = new float[n];//n個分のアドレスを入れてる？
 cout << "*b_heap" << *b_heap << endl;
 cout << "b_heap" << b_heap << endl;

 a_heap = new int [n];
 for(int i = 0; i < n ; i++){
//    a_heap[i] = 2*i;
    b_heap[i] = 2*i;
//    cout<<"a:" << a_heap[i] << "\n";
    cout<<"bv:" << b_heap[i] << "\n";
    cout<<"ba:" << &b_heap[i] << "\n";
 }
 delete[] a_heap;
 delete[] b_heap;
}

