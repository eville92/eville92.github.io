/* Originaly created September 2025
* 
* Quicksort algorithm
* Obtained & manipulated from Corman - Ch7
* Worse-case: B Omega(n^2)
* Best-Case: B Om(n lgn)
* 
* Array size n filled in with random numbers */

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int PARTITION(int A[], int p, int r);
void QUICKSORT(int A[], int p, int r);

int main() {
    int n = 100;
    int A[n];
    srand(time(0));

    for (int i = 0; i < n; i++) { A[i] = rand() % 200; } // rand 0 - 199
    cout << "Before quicksort: \n";
    for (int i = 0; i < n; i++) { cout << A[i] << " "; } // pre-QS
    QUICKSORT(A, 0, n - 1);
    cout << "\n\nAfter quicksort: \n";
    for (int i = 0; i < n; i++) { cout << A[i] << " "; } // post-QS

    return 0;
}

void QUICKSORT(int A[], int p, int r) {
    if (p < r) {
        int q = PARTITION(A, p, r);
        QUICKSORT(A, p, q - 1);
        QUICKSORT(A, q + 1, r);
    }
}

int PARTITION(int A[], int p, int r) {
    int x = A[r]; // pivot
    int i = p - 1;

    for (int j = p; j < r; j++) {
        if (A[j] <= x) {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);

    return i + 1; // pivot index
}