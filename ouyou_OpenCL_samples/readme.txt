readme.txt


◎プログラムはOpenCL 1.2向けに作られています。

◎*.cpp, *.h, *.txt, *.cl以外のファイルやディ
　レクトリはWindows/VC++用です。
  不明な点は、書籍の環境を参照してください。

◎04imgBasic内のソースは、ファイルを共有します。
  各依存関係は、各ディレクトリのreadme.txtを参照
  してください。

◎06geometryBasic\から09imageObject\のプログラム
  はCexp\に含まれるソースを使用します。


◎Cexp\に含まれるClUtils.cppをOpenCL 1.1以前で使用したい
  場合、ソースに含まれる「#define opencl12」をコメント
  アウトしてください。
　これで、動作するようになりますが、処理しない画素にゴミ
　が残る場合があります。詳細は、書籍の本文を参照してく
　ださい。

◎カーネル(*.cl)が実行ファイルと同一ディレクトリに存在
　しなければならないものもあります。詳細は、書籍の本文を
　参照してください。



 (c)Copyright Spacesoft corp., 2012 All rights reserved.
                               Hiro KITAYAMA
