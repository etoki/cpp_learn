#include <iostream>
#include <stdio.h>

using namespace std;

int num1, num2;
 
int tobinary(int num) {
    int bit;
    int bin[100];
    int count = 0;
    while (true) {
        bit = num % 2;
        num = num / 2;
        bin[count] = bit;
        if (num == 0) {
            break;
        }
        count++;
    }
    for (int i = count; i >= 0; i--) {
        cout << bin[i];
    }
    cout << "\n";
    return 0;
}
 
int main() {
	cout << "Enter decimal number: ";
	cin >> num1 >> num2;

	cout << "decimal " << num1 << " to binary : ";
	tobinary(num1);

	cout << "decimal " << num2 << " to binary : ";
	tobinary(num2);

	cout << num1 << " & " << num2 << " = ";
	tobinary(num1 & num2);

	cout << num1 << " | " << num2 << " = ";
	tobinary(num1 | num2);
	return 0;
}

