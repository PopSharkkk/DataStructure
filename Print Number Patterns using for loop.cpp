
#include <iostream>
using namespace std;
int main()
{
    int rows, columns, number = 1, n = 5;
    
    for (rows = 1; rows <= n; rows++)
    {

        for (columns = 1; columns <= rows; columns++)
        {
            cout << number << " ";
            number++;
        }

        cout << "\n";
    }
    return 0;
}