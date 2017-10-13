#include <iostream>

using namespace std;

double Add(double a, double b){ return a + b; }
double Sub(double a, double b){ return a - b; }
double Mul(double a, double b){ return a * b; }
double Div(double a, double b){ return a / b; }

double (* const afpOps[])(double, double) = {
    Add, Sub, Mul, Div,
};

enum EOperator
{
    ADD, SUB, MUL, DIV,
};

bool InputOp(EOperator& rfOp)
{
    char letter;

    cout << "演算子を指定して下さい > " << flush;
    cin >> letter;

    switch(letter)
    {
    case '+': rfOp = ADD; break;
    case '-': rfOp = SUB; break;
    case '*': rfOp = MUL; break;
    case '/': rfOp = DIV; break;
    default :      return false;
    }

    return true;
}

void InputVals(double& ra, double& rb)
{
    cout << "２つの数値を入力して下さい > " << flush;
    cin >> ra >> rb;
}

int main()
{
    static const char* s_apszOps[] = {
        " + ", " - ", " * ", " / ",
    };
    EOperator fOp;
    double     a, b;

    while(InputOp(fOp))
    {
        InputVals(a, b);

        cout << a << s_apszOps[fOp] << b << " = "
             << afpOps[fOp](a, b) << endl;
    }

    cout << "終了します。" << endl;
    return 0;
}
