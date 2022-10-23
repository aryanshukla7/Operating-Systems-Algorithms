#include <bits/stdc++.h>
using namespace std;

typedef struct {
    char fileName;
    int fileSize;
    vector<pair<int, int>> location;
} DIRECTORY;

typedef struct {
    char fileName;
    char fileOperation;
    int fileSize;
} QUERY;



DIRECTORY dir[256];
// * in disk means the block is empty, * cannot be a fileName
char disk[256];


void printStatusDisDir(int currDirPointer) {
    for(int i = 0; i < 17; i++){
        cout << "...";
    }
    cout << "\n";
    for(int i = 0; i < 256; i++){
        if(i%16 == 0){
            cout << "\n";
        } 
        cout << disk[i] << " ";
    }
    cout << "\n\nDirectory Status\n\n";
    cout << "Directory size: " << currDirPointer << "\n\n";
    for(int x = 0; x < currDirPointer; x++){
        cout << dir[x].fileName << " " << dir[x].fileSize << " ";
        if(dir[x].location.size() == 0) {
            cout << "NULL\n";
        } else {
            cout << "{";
            for(int i = 0; i < dir[x].location.size(); i++) {
                cout << "[" << dir[x].location[i].first << "," << dir[x].location[i].second << "]";
                cout << ", ";
            }
            cout << "}\n";
        }
    }
    for(int i = 0; i < 17; i++){
        cout << "...";
    }
    cout << "\n";
}

void updateDirectory(QUERY input, int *currDirPointer, pair<int, int> location){
    bool flag = true;
    for(int i = 0; i < *currDirPointer; i++) {
        if(dir[i].fileName == input.fileName) {
            dir[i].fileSize = input.fileSize;
            pair<int, int> loc(location.first, location.second);
            dir[i].location.push_back(loc);
            flag = !flag;
            break;
        }
    }
    if(flag){
        DIRECTORY temp;
        temp.fileName = input.fileName;
        temp.fileSize = input.fileSize;
        pair<int, int> loc(location.first, location.second);
        temp.location.push_back(loc);
        dir[*currDirPointer] = temp;
        (*currDirPointer)++;
    }
}

pair<int, int> firstFit(int requiredSize, char refChar){
    int currSize = 0;
    pair<int, int> res;
    for(int i = 0; i < 256; i++){
        if(disk[i] == refChar) {
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


void createFile(QUERY inp, int *currDirPointer) {
    pair<int, int> alloc;
    alloc = firstFit(inp.fileSize, '*');
    int startingIndex = alloc.first;
    int endingIndex = alloc.second;
    pair<int, int> loc;
    loc.first = startingIndex;
    loc.second = endingIndex;
    for(int i = alloc.first; i <= alloc.second; i++){
        disk[i] = inp.fileName; 
    }
    updateDirectory(inp, currDirPointer, loc);
    int remainingSize = inp.fileSize - (alloc.second - alloc.first) - 1;
    while(remainingSize > 0){
        pair<int, int> reAlloc;
        reAlloc = firstFit(remainingSize, '*');
        for(int i = reAlloc.first; i <= reAlloc.second; i++){
            disk[i] = inp.fileName;
        }
        updateDirectory(inp, currDirPointer, reAlloc);
        remainingSize -= reAlloc.second;
    }
    cout << "Current Disk and Dir status after creating file " << inp.fileName << "\n";
    printStatusDisDir(*currDirPointer);
}

void deleteFile(char fileName, int currDirPointer) {
    for(int i = 0; i < 256; i++) {
        if(disk[i] == fileName){
            disk[i] = '*';
        }
    }
    for(int i = 0; i < currDirPointer; i++) {
        if(dir[i].fileName == fileName) {
            dir[i].fileSize = 0;
            dir[i].location.resize(0);
            break;
        }
    }
}



int main() {
    fill_n(disk, 256, '*');
    disk[0] = '#';
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
            cout << "Current Disk and Dir status after deleting file " << inp.fileName << "\n";
            deleteFile(inp.fileName, currDirPointer);
            printStatusDisDir(currDirPointer);
        } else if (inp.fileOperation == 'P') {
            cout << "Printing file " << inp.fileName << ":\n";
            int fileSize;
            for(auto x : dir) {
                if(x.fileName == inp.fileName) {
                    fileSize = x.fileSize;
                    break;
                }
            }
            for(int y = 0; y < fileSize; y++) {
                if(y%16 == 0){
                    cout << "\n";
                } else {
                    cout << inp.fileName << " ";
                }
            }
            cout << "\n\n";
        }
    }
    cout << "Final Status of Disk and Directory: \n";
    printStatusDisDir(currDirPointer);
}