#include <bits/stdc++.h>
using namespace std;


// This will merge the two component arrays(A and B), A[low...mid] and B[mid+1, high]
// Usually an auxiliary array is used for merging (for storing intermediate swapping results of the two component arrays (A and B))
// This causes an extra space of N to be used, at max to store the auxiliary array. The auxiliary array can be replaced if we store the intermediate results
// in another text file, (output.txt). This will totally eliminate an extra N-space being used to store auxiliary array.
// Total space being used = N (to store input array) + k (constant space to store intermediate variables (less than 20 being used here)) < N + Q, where N/10 <= Q <= N/2
void merge(int arr[], int low, int mid, int high) {
    int i = low, j = mid+1, k = low;
    // auxiliary file used to store intermediate output
    ofstream aux;
    aux.open("output.txt");
    while(i <= mid && j<= high) {
        if(arr[i] < arr[j]) {
            aux << arr[i++] << "\n";
        } else {
            aux << arr[j++] << "\n";
        }
    }
    while(i <= mid) {
        aux << arr[i++] << "\n";
    }
    while(j <= high) {
        aux << arr[j++] << "\n";
    }
    aux.close();
    ifstream auxRead;
    auxRead.open("output.txt");
    int m = low;
    for(int m = low; m <= high; m++){
        auxRead >> arr[m];
    }
}


void sort(int arr[], int n) {
    int low, high, mid;
    int i, j;
    for(i = 2; i <= n; i = i*2) {
        for(j = 0; i+j-1 <= n; j = j + i) {
            low = j; 
            high = i+j-1;
            mid = low + (high - low)/2;
            merge(arr, low, mid, high);
        }
    }
    if(i/2 < n) {
        merge(arr, 0, (i/2) - 1, n);
    }
} 
 

int main() {
    ifstream inputFile;
    inputFile.open("input.txt");
    int N, SEED, LOW, HIGH, Q;
    if(inputFile.fail()) {
        cerr << "Please Provide correct input file.\n";
        exit(0);
    }
    inputFile >> N >> SEED >> LOW >> HIGH >> Q;
    // SEED = Last two digits of my roll number = 77
    srand(SEED);
    int arr[N];
    for(int i = 0; i < N; i++) {
        int randomNum = rand()%HIGH + LOW;
        // Input array will take up N space, We now have Q space left to work with, Q < N.
        arr[i] = randomNum;
    }
    
    sort(arr, N-1);
    cout << "Sorted results are stored in output.txt file.\n";
    return 0;
}