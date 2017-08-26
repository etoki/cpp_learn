#include <iostream>
#include <stdio.h>

using namespace std;

void f(int x);
void fp(int *p);

//1、構造体の定義
struct test_const{ // 構造体
    int  no;
    int  point;
};

class test_class{ // 構造体
public:
    int  no;
    int  point;
};

void ConstDisp(const   test_const& test);
void pConstDisp(const  test_const* ptest);
void p2ConstDisp(const test_const* ptest);

void ClassDisp(const   test_class& test);
void pClassDisp(const  test_class* ptest);
void p2ClasstDisp(const test_class* ptest);

int main()
{
    int a;
    int* p;
    int& r = a;
    // int& r; &r = a; とはやらない


    // printf("%d\n", a); //value
    // printf("%d\n", &a); //address
    // printf("%d\n", p); //address
    // printf("%d\n", r); //value same as a value
    // printf("%d\n\n", &r); //address same as a address
    //
    // p = &a; //同じアドレス＝エイリアス
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

    test_class testc[] = {
        { 1, 10, },
        { 2, 20, },
        { 3, 30, },
    };

    // ConstDisp(test[0]);
    // pConstDisp(&test[1]);
    ClassDisp(testc[0]);
    // p2ConstDisp(&test[2]);

    // printf("No: %d point: %d\n", test[0].no, test[0].point);

    return 0;
}

void f(int  x){  cout << "result: " << x << endl; }
void fp(int *p){ cout << "p_result: " << *p << endl; }

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

// class
void ClassDisp(const test_class &test){
  cout << "No: "    << test.no << ", "
       << "Point: " << test.point << endl;
}

void pClassDisp(const test_class *ptest){
  cout << "p1 No: " << (*ptest).no << ", "
       << "Point: " << (*ptest).point << endl;
}

void p2ClassDisp(const test_class *ptest){
  cout << "p2 No: " << ptest->no << ", "
       << "Point: " << ptest->point << endl;
}
