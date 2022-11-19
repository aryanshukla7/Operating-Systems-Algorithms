#include <bits/stdc++.h>
using namespace std;

void printvvi(vector<vector<int>> &processes){
    for(auto j : processes){
        for(auto k : j){
            cout << k << " ";
        }
        cout << "\n";
    }
}

void printvi(vector<int>&v){
    for(auto &i : v){
        cout << i << " ";
    }
    cout << "\n";
}


bool sortByArrivalTime(vector<int> &a, vector<int> &b){
    return (a[1] < b[1]);
}

int main(){
    vector<vector<int>> processes;
    queue <int> CPU_Q;
    queue <int> IO_Q;
    ifstream file;
    file.open("process.dat");
    int num_process, pid, arrival_time, cpu_burst_1, cpu_burst_2, io_burst;
    file >> num_process;
    cout << "Total Processes: " << num_process << "\n";
    int totalCPU = 0;
    int totalIO = 0;
    processes.resize(num_process, vector<int>(5, -1));
    for(int i = 0; i < 2*num_process; i++){
        if(i%2 == 0){
            file >> pid;
            // cout << "PID: " << pid << " ";
        } else {
            file >> arrival_time >> cpu_burst_1 >> io_burst >> cpu_burst_2;
            processes[pid-1] = {pid, arrival_time, cpu_burst_1, io_burst, cpu_burst_2};
            totalCPU += cpu_burst_1 + cpu_burst_2;
            totalIO += io_burst;
            // cout << "Arrival Time = " << arrival_time << " CPU Burst 1 = " << cpu_burst_1 << " IO Burst = " << io_burst << " CPU Burst 2 = " << cpu_burst_2 << "\n";
        }
    }
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    int clock = 0;
    int processCount = 0;
    int PID = processes[processCount][0];
    printvvi(processes);
    
    // CPU BURST 1
    vector<int> cpu;
    vector<int> io;
    while(clock <= totalCPU){
        if(processes[processCount][1] <= clock){
            for(int i = 0; i < processes[processCount][2]; i++){
                // cout << "PID: " << PID << " is in CPU\n";
                cpu.push_back(PID);
                clock++;
                // io.push_back(0);
            }
            int diff = cpu.size()-io.size();
            for(int i = 0; i < diff; i++){
                io.push_back(0);
            }
            for(int i = 0; i < processes[processCount][3]; i++){
                io.push_back(PID);
            }
        processCount++;
        if(processCount >= processes.size()) break;
        } else {
            cpu.push_back(0);
            clock++;
        }
        PID = processes[processCount][0];
    }
    processCount = 0;
    PID = processes[processCount][0];

    while(clock <= totalCPU){
        if(processes[processCount][1] <= clock){
            for(int i = 0; i < processes[processCount][4]; i++){
                // cout << "PID: " << PID << " is in CPU\n";
                cpu.push_back(PID);
                clock++;
            }
        processCount++;
        if(processCount >= processes.size()) break;
        } else clock++;
        PID = processes[processCount][0];
    }
    int diff = cpu.size()-io.size();
    for(int i = 0; i < diff; i++){
        io.push_back(0);
    }
    printvi(cpu);
    printvi(io);
}