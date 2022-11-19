// PART - A

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

DIRECTORY dir[256];
char disk[256];
// * in disk means the block is empty, * cannot be a fileName

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
    cout << "\nDirectory Status\n\n";
    cout << "Directory size: " << currDirPointer << "\n\n";
    for(int x = 0; x < currDirPointer; x++){
        cout << dir[x].fileName << " " << dir[x].fileSize << " " << dir[x].startingIndex << "\n";
    }
    for(int i = 0; i < 17; i++){
        cout << "...";
    }
    cout << "\n";
}

void updateDirectory(QUERY input, int *currDirIndex, int startingIndex){
    DIRECTORY temp;
    temp.fileName = input.fileName;
    temp.fileSize = input.fileSize;
    temp.startingIndex = startingIndex;
    dir[*currDirIndex] = temp;
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
            if(currSize == requiredSize){
                res.first = i-currSize+1;
                res.second = i;
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
    updateDirectory(inp, currDirPointer, startingIndex);
    (*currDirPointer)++;
    for(int i = alloc.first; i <= alloc.second; i++){
        disk[i] = inp.fileName;
    }
    cout << "Current Disk and Dir status after creating file " << inp.fileName << "\n";
    printStatusDisDir(*currDirPointer);
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
    for(auto inp : input){
        if(inp.fileOperation == 'C') {
            createFile(inp, &currDirPointer);
            
        } else if (inp.fileOperation == 'M') {
            int startingIndex;
            int oldSize;
            for(auto x : dir) {
                if(x.fileName == inp.fileName) {
                    startingIndex = x.startingIndex;
                    oldSize = x.fileSize;
                    // Updating dir
                    x.fileSize = inp.fileSize;
                }
            }
            if(inp.fileSize <= oldSize) {
                // deleting file
                for(int j = startingIndex + inp.fileSize; j < startingIndex + oldSize; j++) {
                    disk[j] = '*';
                }
                for(int y = 0; y < currDirPointer; y++) {
                    if(dir[y].fileName == inp.fileName) {
                        dir[y].fileSize = inp.fileSize;
                        break;
                    }
                }
            } else {
                for(int i = startingIndex; i < startingIndex+oldSize; i++) {
                    disk[i] = '*';
                }
                QUERY customCreate;
                customCreate.fileName = inp.fileName;
                customCreate.fileOperation = 'C';
                customCreate.fileSize = inp.fileSize;
                createFile(customCreate, &currDirPointer);
                // Updating the already exisiting directory entry
                // (not creating new entry in dir on modification query),
                // thus reducing fragmentation
                for(int y = 0; y < currDirPointer; y++){
                    if(dir[y].fileName == inp.fileName) {
                        dir[y].fileSize = dir[currDirPointer-1].fileSize;
                        dir[y].startingIndex = dir[currDirPointer-1].startingIndex;
                        currDirPointer--;
                        break;
                    }
                }
            }
            cout << "Current Disk and Dir status after Modifying file " << inp.fileName << "\n";
            printStatusDisDir(currDirPointer);

        } else if (inp.fileOperation == 'D') {
            int startIndex;
            int oldSize;
            for(int i = 0; i < currDirPointer; i++) {
                if(dir[i].fileName == inp.fileName) {
                    startIndex = dir[i].startingIndex;
                    oldSize = dir[i].fileSize;
                    dir[i].fileSize = 0;
                    dir[i].startingIndex = -1;
                }
            }
            for(int i = startIndex; i < startIndex+oldSize; i++) {
                disk[i] = '*';
            }
            cout << "Current Disk and Dir status after deleting file " << inp.fileName << "\n";
            printStatusDisDir(currDirPointer);
        } else if (inp.fileOperation == 'P') {
            cout << "Printing file " << inp.fileName << ":\n\n";
            int startIndex;
            int fileSize;
            for(int i = 0; i < currDirPointer; i++) {
                if(dir[i].fileName == inp.fileName) {
                    startIndex = dir[i].startingIndex;
                    fileSize = dir[i].fileSize;
                    break;
                }
            }
            for(int i = startIndex; i < startIndex+fileSize; i++) {
                if(i%16 == 0) {
                    cout << "\n";
                } else {
                    cout << disk[i] << " ";
                }
            }
            cout << "\n\n";
        }


    }
    cout << "Final Status of Disk and Directory\n";
    printStatusDisDir(currDirPointer);
}
