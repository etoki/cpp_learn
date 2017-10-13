#include<iostream>
#define SET 1
using namespace std;

int main(){

	int res;
/*
	int param;
	cin >> param;
	cout << param << endl;
*/
#if SET
	cout << "if course: ";
	res = 1+1;
	cout << res << endl;
#else
	cout << "else course: ";
	res = 1+2;
	cout << res << endl;
#endif
	return 0;
}

