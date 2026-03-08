#include <bits/stdc++.h>
using namespace std;
int getMin(const int arr[], int n)
{
    int res = arr[0];
    for (int i = 1; i < n; i++)
        res = min(res, arr[i]);
    return res;
}
int getMax(const int arr[], int n)
{
    int res = arr[0];
    for (int i = 1; i < n; i++)
        res = max(res, arr[i]);
    return res;
}
int main()
{
    const int arr[] = {12, 1234, 45, 67, 1};
    const int n = sizeof(arr) / sizeof(arr[0]);
    cout << "Minimum element of array: " << getMin(arr, n) << " ";
    cout << "Maximum element of array: " << getMax(arr, n);
    return 0;
}

