// FCFS: First Come First Serve

#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream file;
    file.open("input.txt");
    int curr, count = 0;
    vector<int> reqQueue;
    int numTracks, numSector, bytesPerSector, rpm, avgSeekTime, initialHeadPos;
    while(file >> curr) {
        if(count == 0) {
            numTracks = curr;
        } else if(count == 1) {
            numSector = curr;
        } else if(count == 2) {
            bytesPerSector = curr;
        } else if(count == 3) {
            rpm = curr;
        } else if(count == 4) {
            avgSeekTime = curr;
        } else if(count == 5) {
           initialHeadPos = curr;
        } else {
            reqQueue.push_back(curr);
            while(file >> curr) {
                reqQueue.push_back(curr);
            }
        }
        count++;
    }

    int prevReq = initialHeadPos;
    int totalSeekTime = 0;
    for(auto i : reqQueue) {
        cout << i << " ";
    }
    cout << "\n";
    cout << "Average Rotational Delay: " << (60*1000)/(2*rpm)  << " ms\n";
    for(auto i : reqQueue) {
        totalSeekTime += abs(i-prevReq);
        prevReq = i;
    }
    cout << "Total Seek Time using FCFS: " << totalSeekTime*avgSeekTime << " ms\n";
} 