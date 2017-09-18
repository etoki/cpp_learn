#include<stdio.h>
#include<string.h> //strcmp、strncmp関数を使うために必要
 
int main(void)
{
  // イコールで１文字のみ比較
  char s[] = "samurai";
 
  //uが文字の配列s[]の3番目の要素uと一致するかどうか
  printf("uの文字コードは%d\n",'u');
  printf("s[3](u)の文字コードは%d\n",s[3] );
  if('u' == s[3]){
    printf("uはs[3]と一致します。\n");
  } else{
    printf("uはs[3]と一致しません。\n");
  }
 
  //sが文字の配列s[]の3番目の要素uと一致するかどうか
  printf("sの文字コードは%d\n",'s');
  printf("s[3](u)の文字コードは%d\n",s[3] );
  if('s' == s[3]){
    printf("sはs[3]と一致します。\n");
  } else{
    printf("sはs[3]と一致しません。\n");
  }


  // strcmpで全文比較
  const char *s1 = "SAMURAI";
  const char *s2 = "samurai";
  const char *s3 = "samurai";
  const char *s4 = "samurao";
  
  //s1(SAMURAI)とs2(samurai)が同じ値かどうか
  if(strcmp(s1,s2)==0){
    printf("s1(=SAMURAI)とs2(=samurai)は同じ値です。\n");
  }else{
    printf("s1(=SAMURAI)とs2(=samurai)は同じ値ではありません。\n");
  }
  
  //s2(samurai)とs3(samurai)が同じ値かどうか
  if(strcmp(s2,s3)==0){
    printf("s2(=samurai)とs3(=samurai)は同じ値です。\n");
  }else{
    printf("s2(=samurai)とs3(=samurai)は同じ値ではありません。\n");
  }
  
  //s3(samurai)とs4(samurao)が同じ値かどうか
  if(strcmp(s3,s4)==0){
    printf("s3(=samurai)とs4(=samurao)は同じ値です。\n");
    
  }else{
    printf("s3(=samurai)とs4(=samurao)は同じ値ではありません。\n");
  }
  
  //s3(samurai)とs4(samurao)の比較
  printf("第1引数 < 第2引数の場合 %d\n",strcmp(s3, s4));
  printf("第1引数 > 第2引数の場合  %d\n",strcmp(s4, s3));
  printf("第1引数 = 第2引数の場合  %d\n",strcmp(s3, s3));


  // strncmpで任意の文字数比較
  const char *ss1 = "likes";
  const char *ss2 = "liked";
 
  //s1とs2の4番目までの文字が同じ値かどうか
  if(strncmp(ss1,ss2,4)==0){
     printf("s1とs2の4番目までの文字は同じ値です。\n");
  }else{
     printf("s1とs2の4番目までの文字は同じ値ではありません。\n");
  }
 
  //s1とs2の5番目までの文字が同じ値かどうか
  if(strncmp(ss1,ss2,5)==0){
     printf("s1とs2の5番目までの文字は同じ値です。\n");
  }else{
     printf("s1とs2の5番目までの文字は同じ値ではありません。\n");
  }

  return 0;
}

