#include<stdio.h>

const int SIZE = 100;

void swap(char* a, char* b) {
	int t = *a; *a = *b; *b = t;
}

int partition (char arr[], int low, int high) {
	char pivot = arr[high];
	int i = (low - 1); // Index of smaller element
	for (int j = low; j <= high- 1; j++)
		if (arr[j] <= pivot) {
			i++; // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}

	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void quickSort(char arr[], int low, int high){
	if (low < high) {
		int pi = partition(arr, low, high);

		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

int parseFile(char *filename, char *arr) {
    FILE *file = fopen(filename, "r");
    int n = 0;
    int c;

    if (file == NULL)
      return 0;

    while (n<SIZE-1 &&(c = fgetc(file)) != EOF)
    {
        arr[n++] = (char) c;
    }

    arr[n] = '\0'; 

    return n;
}

void printArray(char *arr, int n) {
    for(int i = 0; i<n && i<SIZE; i++) printf("%c ", arr[i]);
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        return 1;
    }
    char *filename = argv[1];
	char arr[SIZE];
    int n = parseFile(filename, arr);
    if(n>=1000) n=SIZE;
	quickSort(arr, 0, n-1);
	printArray(arr, n);
	return 0;
}
