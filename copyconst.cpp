#include <iostream>
//他のものと被る可能性があるのでstd::を使うようにする
using namespace std;

class Entity {
public:
   int x;
};

//これだとクラス全体をビットとして値渡ししている
Entity fuga()
{

    Entity *a = new Entity;
    return *a;
//    return &new Entity
}

int main()
{
   Entity b = fuga();
   cout << b.x << endl;
   return 0;
}


