// SSTF: Shortest Seek Time First

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
    while(reqQueue.size() > 0) {
        int minReq = abs(prevReq-reqQueue[0]);
        int minReqPos;
        for(int i = 0; i < reqQueue.size(); i++) {
            minReq = min(abs(prevReq-reqQueue[i]), minReq);
            if(abs(prevReq-reqQueue[i]) == minReq) {
                minReqPos = i;
            }
        }
        totalSeekTime += abs(reqQueue[minReqPos]-prevReq);
        prevReq = reqQueue[minReqPos];
        reqQueue.erase(reqQueue.begin()+minReqPos);
    }
    cout << "Total Seek Time using SSTF: " << totalSeekTime*avgSeekTime << " ms\n";
} 