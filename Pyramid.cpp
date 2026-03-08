#include <bits/stdc++.h>

using namespace std;

int main () {
    int n, num = 1;
    if (!(cin >> n)) return 0;

    for (int r = 1; r <= n; r++) {
        for (int c = 0; c < r; c++){
            cout << num << " ";
            num++;
        }
        cout << "\n";
    }
    return 0;
}
