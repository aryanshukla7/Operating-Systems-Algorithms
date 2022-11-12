#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

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
    char* file1diff[256];
    char* file2diff[256];
    char* sameLines[256];
    int file1diffptr = 0;
    int file2diffptr = 0;
    int sameLineptr = 0;
    while (fgets(line1, NUM_CHARS_PER_LINE, fileptr1) && fgets(line2, NUM_CHARS_PER_LINE, fileptr2)) {
        if(strcmp(line1, line2) == 0) {
            sameLines[sameLineptr] = line1;
            sameLineptr++;
        } else {
            printf("%s%s\n", line1, line2);
            file1diff[file1diffptr] = line1;
            file2diff[file2diffptr] = line2;
            file1diffptr = file1diffptr + 1;
            file2diffptr = file2diffptr + 1;
        }
    }
    printf("File 1 : \n");
    for(int i = 0; i < file1diffptr; i++) {
        printf("%s", file1diff[i]);
    }
    printf("-------------------------\n");
    printf("File 2 : \n");
    for(int i = 0; i < file2diffptr; i++) {
        printf("%s", file2diff[i]);
    }
}


// Varsity by zerodha
// Trading view
// 