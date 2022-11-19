// PART B

#include <bits/stdc++.h>
using namespace std;

typedef struct {
    char fileName;
    int fileSize;
    int startingIndex;
} DIRECTORY;

typedef struct {
    char fileName;
    char fileOperation;
    int fileSize;
} QUERY;

typedef struct {
    char fileName;
    int nextIndex;
    int lastIndex;
} FILE_;

// next Index and last index are going to keep track of the occurance of files located at different non-contiguous blocks in disk.


DIRECTORY dir[256];
// * in disk means the block is empty, * cannot be a fileName
FILE_ disk[256];


void printStatusDisDir(int currDirPointer) {
    for(int i = 0; i < 17; i++){
        cout << "...";
    }
    cout << "\n";
    for(int i = 0; i < 256; i++){
        if(i%16 == 0){
            cout << "\n";
        } 
        cout << disk[i].fileName << " ";
    }
    cout << "\n\nLINKING ARRAY: \n";
    for(int i = 0; i < 256; i++) {
        if(i%16 == 0) {
            cout << "\n";
        }
        cout << "[" << disk[i].nextIndex << "," << disk[i].lastIndex << "] ";
    }
    cout << "\n\nDirectory Status\n\n";
    cout << "Directory size: " << currDirPointer << "\n\n";
    for(int x = 0; x < currDirPointer; x++){
        cout << dir[x].fileName << " " << dir[x].fileSize << " " << dir[x].startingIndex << "\n";
    }
    cout << "\n";
    for(int i = 0; i < 17; i++){
        cout << "...";
    }
    cout << "\n";
}

void updateDirectory(QUERY input, int *currDirPointer){
    
}

pair<int, int> firstFit(int requiredSize, char refChar){
    int currSize = 0;
    pair<int, int> res;
    for(int i = 0; i < 256; i++){
        if(disk[i].fileName == refChar) {
            currSize++;
            if(currSize == requiredSize){
                res.first = i-currSize+1;
                res.second = i;
                break;
            }
        } else {
            if(currSize != 0 && currSize <= requiredSize){
                res.first = i-currSize;
                res.second = i-1;
                break;
            } else if (255-i < requiredSize){
                res.first = -1;
                res.second = -1;
                break;
            }
            currSize = 0;
        }
    }
    return res;
}


void updateDirectory(QUERY input, int *currDirPointer, int startingIndex){
    bool flag = true;
    for(int i = 0; i < *currDirPointer; i++) {
        if(dir[i].fileName == input.fileName) {
            dir[i].fileSize = input.fileSize;
            dir[i].startingIndex = startingIndex;
            flag = !flag;
            break;
        }
    }
    if(flag){
        DIRECTORY temp;
        temp.fileName = input.fileName;
        temp.fileSize = input.fileSize;
        temp.startingIndex = startingIndex;
        dir[*currDirPointer] = temp;
        (*currDirPointer)++;
    }
}


void createFile(QUERY inp, int *currDirPointer) {
    pair<int, int> alloc;
    alloc = firstFit(inp.fileSize, '*');
    cout << "Alloc: " << alloc.first << " " << alloc.second << "\n";
    updateDirectory(inp, currDirPointer, alloc.first);
    for(int i = alloc.first; i <= alloc.second; i++){
        disk[i].fileName = inp.fileName;
        if(i < alloc.second) {
            disk[i].nextIndex = i+1;
            disk[i].lastIndex = alloc.second;
        }
    }
    int remainingSize = inp.fileSize - (alloc.second - alloc.first) - 1;
    while(remainingSize > 0) {
        pair<int, int> reAlloc;
        reAlloc = firstFit(remainingSize, '*');
        disk[alloc.second].nextIndex = reAlloc.first;
        disk[alloc.second].lastIndex = reAlloc.second;
        // cout << "Alloc: " << reAlloc.first << " " << reAlloc.second << "\n";
        // updateDirectory(inp, currDirPointer, reAlloc.first);
        for(int i = reAlloc.first; i <= reAlloc.second; i++){
            disk[i].fileName = inp.fileName;
            if(i < reAlloc.second) {
                disk[i].nextIndex = i+1;
                disk[i].lastIndex = reAlloc.second;
            }
        }
        remainingSize -= reAlloc.second;
    }
    cout << "Current Disk and Dir status after creating file " << inp.fileName << "\n";
    printStatusDisDir(*currDirPointer);
    
}

void deleteFile(char fileName, int currDirPointer) {
    for(int i = 0; i < 256; i++) {
        if(disk[i].fileName == fileName){
            disk[i].fileName = '*';
            disk[i].nextIndex = 0;
            disk[i].lastIndex = 0;
        }
    }
    for(int i = 0; i < currDirPointer; i++) {
        if(dir[i].fileName == fileName) {
            dir[i].fileSize = 0;
            dir[i].startingIndex = -1;
            break;
        }
    }
}


int main() {
    FILE_ init;
    init.fileName = '*';
    fill_n(disk, 256, init);
    disk[0].fileName = '#';
    ifstream file;
    file.open("input.dat");
    char fileName, fileOperation;
    int numQueries, fileSize;
    file >> numQueries; 
    cout << "Total Queries: " << numQueries << "\n\n";
    vector<QUERY> input(numQueries);
    for(int i = 0; i < numQueries; i++){
        QUERY temp;
        file >> fileName >> fileOperation >> fileSize;
        temp.fileName = fileName;
        temp.fileOperation = fileOperation;
        temp.fileSize = fileSize;
        input[i] = temp;
    }

    cout << "INPUT VECTOR\n\n"; 
    for(auto i : input){
        cout << i.fileName << " " << i.fileOperation << " " << i.fileSize << "\n";
    }
    cout << "\nDISK INITIALLY\n\n";

    int currDirPointer = 0;
    printStatusDisDir(currDirPointer);

    for(auto inp : input) {
        if(inp.fileOperation == 'C') {
            createFile(inp, &currDirPointer);
        } else if (inp.fileOperation == 'M') {
            deleteFile(inp.fileName, currDirPointer);
            cout << "Current Disk and Dir status after deleting file " << inp.fileName << "\n";
            printStatusDisDir(currDirPointer);
            createFile(inp, &currDirPointer);
        } else if (inp.fileOperation == 'D') {
            deleteFile(inp.fileName, currDirPointer);
            cout << "Current Disk and Dir status after deleting file " << inp.fileName << "\n";
            printStatusDisDir(currDirPointer);
        } else if (inp.fileOperation == 'P') {
            cout << "Printing file " << inp.fileName << ":\n";
            int fileSize;
            int currIndex;
            for(auto x : dir) {
                if(x.fileName == inp.fileName) {
                    fileSize = x.fileSize;
                    currIndex = x.startingIndex;
                    break;
                }
            }
            char fileName = inp.fileName;
            int i = 0;
            while(fileName == inp.fileName){
                if(i%16 == 0){
                    cout << "\n";
                }
                cout << disk[currIndex].fileName << " ";
                currIndex = disk[currIndex].nextIndex;
                fileName = disk[currIndex].fileName;
                i++;
            }
            cout << "\n\n";
        }
    }
    cout << "Final Status of Disk and Directory: \n";
    printStatusDisDir(currDirPointer);
}