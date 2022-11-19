#include <bits/stdc++.h>
using namespace std;

#define NUM_CHARS_PER_LINE 256

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Provide two text files to compare\n");
        exit(1);
    }
    char const* const filename1 = argv[1];
    char const* const filename2 = argv[2];
    FILE* fileptr1 = fopen(filename1, "r");
    FILE* fileptr2 = fopen(filename2, "r");
    char line1[NUM_CHARS_PER_LINE];
    char line2[NUM_CHARS_PER_LINE];
    
    vector<string> diff1file;
    vector<string> diff2file;
    while (fgets(line1, NUM_CHARS_PER_LINE, fileptr1) && fgets(line2, NUM_CHARS_PER_LINE, fileptr2)) {
        if(strcmp(line1, line2)) {
            diff1file.push_back(line1);
            diff2file.push_back(line2);
        }
    }
    cout << "FIle 1 ->\n\n";
    for(auto i : diff1file) {
        cout << "< " << i;
    }
    while(fgets(line1, NUM_CHARS_PER_LINE, fileptr1)) {
        cout << "< " << line1;
    }
    cout << "\n\n-----------------------------\n\n";
    cout << "FIle 2 ->\n\n";
    for(auto i : diff2file) {
        cout << "> " << i;
    }
    while(fgets(line2, NUM_CHARS_PER_LINE, fileptr2)) {
        cout << "> " << line2;
    }

}

