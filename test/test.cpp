#include <iostream>
using namespace std;
void go(){}
int main(int argc, char const *argv[])
{
    int x=3332;
    long  * y= (long*) x;
    cout<< y;
    return 0;
}
