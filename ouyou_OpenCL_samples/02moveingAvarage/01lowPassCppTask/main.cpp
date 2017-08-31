//===================================================================
// low pass, moving average
//
// (c)Copyright Spacesoft corp., 2012 All rights reserved.
//                               Hiro KITAYAMA
//===================================================================
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static const float wav[]=
{
//#include "../dim1.txt"
#include "../dim0.txt"
};


//-------------------------------------------------------------------
// lowPass
static void
lowPass(const float *in, float *out,
                    const size_t numOfTotal, const size_t windowSize)
{
    //0で初期化
    for(size_t i=0; i<windowSize-1; i++)
        out[i]=0.0f;

    //calc first value
    float value=0.0f;
    for(size_t i=0; i<windowSize; i++)
        value+=in[i];//wavの対象範囲までの値を足しあげてる

    // do moving average
    std::cout << value << std::endl;//total
//    std::cout << value/windowSize << std::endl;//wav合計を範囲で割る
//なぜ-1なのか？→入力値である引数と配列の順番を合わせるため
//上で範囲まで足しあげてるので、範囲で割るのは平均をだすため
//図3-3
    out[windowSize-1]=value/windowSize;//確保したメモリの一番後ろの1つ前に結果を代入
    std::cout << "out[windowSize-1]: " << out[windowSize-1] << std::endl;

    //範囲からwavの要素数まで
    for(size_t i=windowSize; i<numOfTotal; i++)
    {
        //直前でえた総和を使用して次の総和を計算。古い1つはいらない、新しいものを追加。
        std::cout << "value: " << value;
        value+=(in[i]-in[i-windowSize]);
        std::cout << " ,in[i]: " << in[i];
        std::cout << " ,in[i-win]: " << in[i-windowSize];
        std::cout << " ,value: " << value;
        out[i]=value/windowSize;
        std::cout << " ,out[i]: " << out[i] << std::endl;
    }
}


//-------------------------------------------------------------------
// main
int
main(int argc, char* argv[])
{
    if(argc<2)
    {
        fprintf(stderr, "移動平均のウィンドウサイズを指定してください.\n");
        return -1;
    }

//    std::cout << "sizeof wav:" << sizeof(wav) << std::endl;//99696 要素数xバイト数
//    std::cout << "sizeof wav[0]:" << sizeof(wav[0]) << std::endl;//4バイト

    size_t numOfTotal=sizeof(wav)/sizeof(wav[0]);
//    std::cout << numOfTotal << std::endl;//24924 要素数

    size_t windowSize=atoi(argv[1]);//引数argvをint型に変換
//    std::cout << windowSize << std::endl;//24924 要素数

    float *result=new float[numOfTotal];//numOfTotal個数分のメモリを取得
//    std::cout << result[0] << std::endl;//バイト数
//    std::cout << sizeof result << std::endl;//8

    //wav= 投入データ、result=取得したメモリ、numOfTotal=wavの要素数、windowSize=対象範囲
    lowPass(wav, result, numOfTotal, windowSize);

    //参照渡しなのでoutの部分がresultになって書き換えられる
//    for(size_t i=0; i<numOfTotal; i++)
//        printf("%6d\n", (int)result[i]);

    delete [] result;

    return 0;
}
