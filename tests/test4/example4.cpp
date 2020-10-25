// Quicksort
#include<iostream>
#include<fstream>

int action_counter = 0;
const int MAX_ACTION = 2500;

void swap(char* a, char* b)  
{  
    char t = *a;  
    *a = *b;  
    *b = t;  
    action_counter++;
}  
  
char partition (char arr[], char low, char high)  
{  
    char pivot = arr[high];
    char i = (low - 1);
  
    for (char j = low; j <= high - 1; j++)  
    {  
        if (arr[j] < pivot)  
        {  
            i++;
            swap(&arr[i], &arr[j]);
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}
  
void quickSort(char arr[], char low, char high)  
{
    if (low < high)  
    {
        char pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}

void bubbleSort(char arr[], int len) {
    for(int i=0; i<len; i++) {
        for(int j=len-1; j>i; j--) {
            if(arr[j] < arr[j-1]) {
                swap(&arr[j], &arr[j-1]);
            }
        }
    }
}

void printBuffer(char * buffer, unsigned int buffer_size){
    for(int i = 0; i<buffer_size; i++) {
        std::cout << int(buffer[i]) << ' ';
    }
    std::cout << std::endl;
    std::cout << "Action counter: " << action_counter << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc<2) {
        std::cerr << "You must specify the input file" << std::endl;
        return 101;
    }

    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.is_open()) {
        std::cerr << "Could not open the input file." << std::endl;
        return 102;
    }

    char c, i=0;
    const unsigned int BUFFER_SIZE = 80;
    char buffer[BUFFER_SIZE];

    while(input_file.get(c) && i<BUFFER_SIZE) {
        buffer[i++] = c;
    }
    
    if(i<80) return 1;
    // printBuffer(buffer, BUFFER_SIZE);
    // quickSort(buffer, 0, BUFFER_SIZE-1);
    bubbleSort (buffer, BUFFER_SIZE);
    printBuffer(buffer, BUFFER_SIZE);

    if(action_counter>MAX_ACTION) {
        throw "Bad is bad";
        return 1;
    }

    return 0;
}