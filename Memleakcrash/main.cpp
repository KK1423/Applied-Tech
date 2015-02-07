#include <iostream>

using namespace std;

int main()
{
    while(true)
    {
        try{
            int*n = new int[10];
        }
        catch(std::bad_alloc)
        {
            cout<<"q";
        }
    }
}
