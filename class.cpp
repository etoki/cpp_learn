#include <iostream>

using namespace std;

class test_class{
public:
    int  no;
    int  point;
    void ClassDisp();
};

int main()
{

    test_class testc[] = {
        { 1, 10, },
        { 2, 20, },
        { 3, 30, },
    };

    testc[0].ClassDisp();

    return 0;
}

void test_class::ClassDisp(){
  cout << "No: "    << no << ", "
       << "Point: " << point << endl;
}
