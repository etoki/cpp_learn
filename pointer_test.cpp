#include <iostream>

using namespace std;

void f(int x);
void fp(int *p);

//1、構造体の定義
struct test_const{ // 構造体
    int  no;
    int  point;
};

void ConstDisp(const   test_const& test);
void pConstDisp(const  test_const* ptest);
void p2ConstDisp(const test_const* ptest);

int main()
{
    int a;
    int* p;
    int& r = a;

    // printf("%d\n", a); //value
    // printf("%d\n", &a); //address
    // printf("%d\n", p); //address
    // printf("%d\n", r); //value same as a value
    // printf("%d\n\n", &r); //address same as a address
    //
    // p = &a;
    // printf("%d\n", p); //address same as a address
    // printf("%d\n\n", *p); //value same as a value
    //
    // *p = 5;
    // printf("%d\n", *p); //value
    // printf("%d\n", a); //value same as a p
    // printf("%d\n\n", r); //value same as a p
    //
    // f(a);
    // f(*p);
    // f(r);
    //
    // fp(&a);
    // fp(p);
    // fp(&r);

    //2、構造体の初期化
    test_const test[] = {
        { 1, 10, },
        { 2, 20, },
        { 3, 30, },
    };
    // ConstDisp(test[0]);
    // pConstDisp(&test[1]);
    // p2ConstDisp(&test[2]);

    printf("No: %d point: %d\n", test[0].no, test[0].point);

    return 0;
}

void f(int  x){  printf("result: %d\n",    x); }
void fp(int *p){ printf("p_result: %d\n", *p); }

void ConstDisp(const test_const &test){
  cout << "No: "    << test.no << ", "
       << "Point: " << test.point << endl;
}

void pConstDisp(const test_const *ptest){
  cout << "p1 No: " << (*ptest).no << ", "
       << "Point: " << (*ptest).point << endl;
}

void p2ConstDisp(const test_const *ptest){
  cout << "p2 No: " << ptest->no << ", "
       << "Point: " << ptest->point << endl;
}
