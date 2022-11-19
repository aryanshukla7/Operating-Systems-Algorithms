#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

int main() {
   // Hashing all the files in the directory at the same level as the a.out file.
   // the hashed data along with their path is being stored in input.dat, in a sorted order
   // The hashing technique used here is md5
   string command = "find ./ -type f -print0 | xargs -0 md5sum | sort -k1,32 | uniq -w32 -D > input.dat";
   system(command.c_str());
   FILE* fp = fopen("input.dat", "r");
   char * line = NULL;
   size_t len = 0;
   if (fp == NULL)
      exit(EXIT_FAILURE);

   while (getline(&line, &len, fp) != -1) {
      printf("%s", line);
   }

   fclose(fp);
   if (line)
   free(line);
   ifstream file;
   file.open("input.dat");
   // Comparing the hashes of files and deleting the repeated hashes, which indicates duplication (at any level from the root dir of the executable of this cpp code)
   string hash1, path1, hash2, path2;
   file >> hash1 >> path1;
   while(!file.eof()) {
      file >> hash2 >> path2;
      if(hash1 == hash2) {
         string rmfile = "rm " + path2;
         system(rmfile.c_str());
      } else {
         hash1 = hash2;
         path1 = path2;
         cout << hash1 << "\n";
      } 
   }
}
