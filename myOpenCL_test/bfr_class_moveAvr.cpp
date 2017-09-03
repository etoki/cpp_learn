#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static const float wav[]={
#include "data.txt"
};

static void
//wav= 投入データ、result=取得したメモリ、numOfTotal=wavの要素数、windowSize=対象範囲
//lowPass(wav, result, numOfTotal, windowSize); //*wav, *result, numOfTotal=要素数, winSize=範囲
lowPass(const float *in, float *out, const size_t numOfTotal, const size_t windowSize){
    //0で初期化,0番目はデフォで0
    for(size_t i=0; i<windowSize-1; i++)
        out[i]=0.0f;

    //calc first value
    float value=0.0f;
    for(size_t i=0; i<windowSize; i++)
        value+=in[i];//wavの対象範囲までの値を足しあげてる

    // do moving average
//    std::cout << "value: " << value << std::endl;//wavからwinSize個数取り出して合計する
//    std::cout << value/windowSize << std::endl;//wav合計を範囲で割る
//入力値である引数と配列の順番(0スタート)を合わせるため1を引く
//上で範囲まで足しあげてるので、範囲で割るのは平均をだすため
//図3-3
    out[windowSize-1] = value/windowSize;//確保したメモリの一番後ろの1つ前に平均を代入
    std::cout << "範囲の平均:  " << out[windowSize-1] << std::endl;
//    size_t out_num = sizeof(out)/sizeof(out[0]);
//    for(int j=0; j<out_num; j++){
//       std::cout << "out: " << out[j] << std::endl;//8
//    }

    //範囲からwavの要素数まで
    for(size_t i=windowSize; i<numOfTotal; i++){
        //直前でえた総和を使用して次の総和を計算。古い1つはいらない、新しいものを追加。
        std::cout << "value:" << value;
        value+=(in[i]-in[i-windowSize]);
        std::cout << ", data[" << i << "]:" << in[i];
        std::cout << ", data[" << i-windowSize << "]:" << in[i-windowSize];
        std::cout << ", data[i]-data[i-winSize]:" << in[i] << "-" << in[i-windowSize] << "=" << in[i]-in[i-windowSize];
        std::cout << ", value+=(data[i]-data[i-winSize]):" << value;
        out[i]=value/windowSize;
        std::cout << ", value/windowSize:" << value << "/" << windowSize;
        std::cout << ", out["<< i << "]=value/windowSize:" << out[i] << std::endl;
    }
}


int
main(int argc, char* argv[]){
    if(argc<2){
        fprintf(stderr, "移動平均のウィンドウサイズを指定してください.\n");
        return -1;
    }
//    std::cout << "sizeof wav:" << sizeof(wav) << std::endl;//60 要素数15xバイト数4

    size_t numOfTotal=sizeof(wav)/sizeof(wav[0]);
//    std::cout << numOfTotal << std::endl;//60/4=15 要素数
//    for(int j=0;j<numOfTotal;j++){
//       std::cout << "data: " << wav[j] << std::endl;//8
//    }

    size_t windowSize=atoi(argv[1]);//引数argvをint型に変換

    float *result=new float[numOfTotal];//要素数分のメモリを取得
//    for(int j=0;j<30;j++){
//       std::cout << "&result" << &result[j] << std::endl;//8
//    }

    //wav= 投入データ、result=取得したメモリ、numOfTotal=wavの要素数、windowSize=対象範囲
    lowPass(wav, result, numOfTotal, windowSize); //*wav, *result, numOfTotal=要素数, winSize=範囲
//  lowPass(const float *in, float *out, const size_t numOfTotal, const size_t windowSize){

    //参照渡しなのでoutの部分がresultになって書き換えられる
    for(size_t i=0; i<numOfTotal; i++)
        printf("%6d\n", (int)result[i]);

    delete [] result;

    return 0;
}
