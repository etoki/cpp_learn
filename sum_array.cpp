#include <iostream>
using namespace std;

int main(){
 int arr[10] = {0,1,2,3,4,5,6,7,8,9};
 int total;
// int out[10] = {0,0,0,0,0,0,0,0,0,0};

 int def = 3;
 size_t num = sizeof(arr)/sizeof(arr[0]);
// cout << num << endl;

 for(int i = def; i<num; i++){
    total+=(arr[i]-arr[1]);
    cout << "total "<< total << endl;
    cout << "waru "<< total/def << endl;
//    out[i] = total/def;
//    cout << "out "<< out[i] << endl;
 };
 return 0;
}

