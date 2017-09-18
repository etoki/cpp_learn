#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//////////////////////////////////////////////
//宣言方法
//////////////////////////////////////////////

struct st1 {
	int num;
	char *str;
};

//typedef int B; Bはint型の別名になる
//以下は、st3がst2の別名になる

typedef struct st2{
	int num;
	char *str;
} st3;

typedef struct{
	int a;
	int b;
} st4;

struct st1 Disp(void);

int main(){

	//////////////////////////////////////////////
	//呼び出し方法
	//////////////////////////////////////////////
	st3 st;
	st4 st4_sin;// 配列じゃない
	st4 st4_arr[2] = { //配列
			{1,2},
			{3,4},
	};

	//配列じゃないなら&でアドレス指定
	st4 *st4_pt_sin;
	st4_pt_sin = &st4_sin;

	//&がなくても配列は先頭アドレス指定を指定
	st4 *st4_pt_arr;
	st4_pt_arr = st4_arr;

	//mallocで確保する
	st4 *st4_pt_alc;
//	st4_pt_alc->a = 10; //Err 実体はまだ存在しない
	st4_pt_alc = (st4*)malloc(sizeof(st4));

	st4 *st4_pt_alc_arr;
	st4_pt_alc_arr = (st4*)malloc(sizeof(st4) * 10);

	//////////////////////////////////////////////
	//アクセス方法
	//////////////////////////////////////////////

	st.num = 1;
	cout << st.num << endl;
	cout << st4_arr[0].b << endl;

	st4_sin.a = 100;
	cout << st4_pt_sin->a << endl;

	//配列はポインタがさすアドレスをずらすことでアクセスできる
	cout << st4_pt_arr[0].b << endl; //2
	cout << st4_pt_arr->a << endl; //1
	st4_pt_arr += 1;
	cout << st4_pt_arr->a << endl; //3

	//mallocで確保する
	st4_pt_alc->a = 10;
	cout << st4_pt_alc->a << endl;//10
	free(st4_pt_alc);

	//mallocで確保する、配列
	st4_pt_alc_arr[0].a = 11;
	cout << st4_pt_alc_arr[0].a << endl;//11
	free(st4_pt_alc_arr);

	//構造体型の関数
	struct st1 test;
	test = Disp();
	cout << test.num << endl;//200

	return 0;
}

struct st1 Disp(void){
	struct st1 st1_func;
	st1_func.num = 200;
	return st1_func;
}
