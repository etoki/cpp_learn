#include <iostream>

using namespace std;

class MyClass{
public:          //アクセス指定子
//private:
//protected:
    MyClass();   // コンストラクタ宣言
    MyClass(int num);   // オーバーロードの宣言
    virtual ~MyClass(); // デストラクタ20章、仮想関数にするのなぜ？

    int m_a;     //int 型のメンバ変数。名前は m_a
    int m_b[100];
    int* m_p;   // 動的配列→この価値は？

    void Disp0(int m_a);

    //インライン関数の宣言
    virtual void Disp(int m_a);
    //virtual オーバーライドできるようにする
};

//thisによるアドレス取得25章
class ThisClass
{
//    int a[100];
public:
    ThisClass* RetThis(){ return this; }
};

//継承
class ChildClass : public MyClass
{
public:
    ChildClass() { cout << "Child" << endl; }
    ~ChildClass(){ cout << "~Child" << endl; }

    //オーバーライド：継承先で再度定義すること
    virtual void Disp(int m_a);
};

// コンストラクタ：クラスのメンバ変数の初期化
//(引数なしのコンストラクタのことをデフォルトコンストラクタ)
MyClass::MyClass(): m_a(5) { //22章初期化
    cout << "MyClass" << endl
         << m_a << endl;
//    m_a = 0;
}

// オーバーロード(同名だが違う引数)
MyClass::MyClass(int num){
    cout << "MyClass" << endl;
    m_a = num;
}

// デストラクタ
MyClass::~MyClass(){ cout << "~MyClass" << endl; }

// メソッド
void MyClass::Disp0(int m_a){
    cout << "test0" << endl;
    int m_c = m_a;
    Disp(m_c); //仮想関数の実装がうまくいかない
}

void MyClass::Disp(int m_a){
    cout << m_a << "test1" << endl;
}

void ChildClass::Disp(int m_a){
    cout << m_a << "test2" << endl;
}


int main(){
    MyClass a, b(100); //MyClass 型のオブジェクトを生成
//    a.m_a = 123;  //メンバ変数に代入
//    a.Disp(111);     //メソッドの利用
//    b.Disp(222);

//    a.Disp0(777);
//    b.Disp0(666);

    ChildClass c;
//    c.Disp(333); //オーバーライドされた関数がよばれる
//    c.Disp0(555);
//    c.MyClass::Disp0(544);// 21章

//動的にオブジェクトを生成(19章)→この価値は？
//    MyClass *ptr = new MyClass;
//    MyClass *arr = new MyClass[10]; //配列生成
//    delete ptr;    //解放
//    delete [] arr; //解放

//アップキャスト？15,20章
//    MyClass *chd = new ChildClass;
//    delete chd;    //解放

    ThisClass test1, test2;

    cout << "test1 : " << &test1 << endl
         << "test2 : " << &test2 << endl
         << "test1 : " << test1.RetThis() << endl
         << "test2 : " << test2.RetThis() << endl;

    return 0;
}
