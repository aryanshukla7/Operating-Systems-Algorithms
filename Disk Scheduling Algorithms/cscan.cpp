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

    for(auto i : reqQueue) {
        cout << i << " ";
    }
    cout << "\n";
    cout << "Average Rotational Delay: " << (60*1000)/(2*rpm) << " ms\n";
    sort(reqQueue.begin(), reqQueue.end());
    int largestBeforeInit = 0;
    for(auto i : reqQueue) {
        if(i <= initialHeadPos) {
            largestBeforeInit = max(largestBeforeInit, i);
        }
    }

    // When initial Head points to minimum request query, the arm does not come back
    // as all the requests are handled in one forward sweep only.
    int totalSeekTime = abs(numTracks-initialHeadPos) + numTracks + largestBeforeInit;
    if(largestBeforeInit == initialHeadPos) {
        totalSeekTime = abs(numTracks - initialHeadPos);
    }
    cout << "Total Seek Time using CSCAN: " << totalSeekTime*avgSeekTime << " ms\n";
} 