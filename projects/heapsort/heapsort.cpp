
/* Orignally written September 2025
* 
* Max Heapify sorting algorithm
* Obtained & manipulaed from Corman - Ch6
* Worse-case: O(n lgn)
* 
* Array size n filled in with random numbers */

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

void maxHeapify(int A[], int i, int n);

int main() {
    int n = 100;
    int A[n];
    srand( time(0) );
    
    for (int i = 0; i < n; i++) { A[i] = rand() % 200; } // rand 0 - 199
    cout << "Before max heapify: \n";
    for (int i = 0; i < n; i++) { cout << A[i] << " "; } // pre-mH
    maxHeapify(A, 0, n);
    cout << "\n\nAfter max heapify: \n";
    for (int i = 0; i < n; i++) { cout << A[i] << " "; } // post-mH
    
    return 0; 
}

void maxHeapify(int A[], int i, int n) {
    int left = 2*i + 1;
    int right = 2*i + 2;
    int largest = i;
    
    if (left < n && A[left] > A[i])
        largest = left;
    else largest = i;
    
    if (right < n && A[right] > A[largest]) 
        largest = right;
    
    if (largest != i) {
        swap(A[i], A[largest]);
        maxHeapify(A, largest, n);
    }
}