#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
using namespace std;

int total_swaps = 0;

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
    total_swaps++;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // pivot
    int i = (low - 1);     // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main(int argc, char **argv) {
    ifstream infile;
    if (argc < 2) {
        printf("You must provide the inputfile\n");
        return 0;
    }

    infile.open(argv[1]);
    string s;
    infile >> s;

    const int ASIZE = 64;
    int arr[ASIZE];
    for(int i = 0; i<ASIZE; i++) arr[i]=i;

    for(unsigned int i = 0; i<ASIZE && i<s.size(); i++) {
        arr[i] = int(s[i])%ASIZE;
    }

    quickSort(arr, 0, ASIZE-1);

    cout << "Total swapss: " << total_swaps << endl;

    return 0;
}