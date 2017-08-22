#include <iostream>

using namespace std;

int main(){
 int n;
 int *a_heap;
 int i;
 
 cout << "Please input...";
 cin >> n;
 
 a_heap = new int [n];

 for(int i = 0; i < n ; i++){
    a_heap[i] = 2*i;
 }
 for(int i = 0; i < n ; i++){
    cout<< a_heap[i] << "\n";
 }
 delete[] a_heap;
}

