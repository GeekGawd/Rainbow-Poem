#include <iostream>
#include <set>
using namespace std;
  
int main()
{
    set<int> myset{ 1, 2, 3, 4, 5 };
    myset.erase(5);

    for( auto val: myset )
    {
        cout<<val<<endl;
    }

}