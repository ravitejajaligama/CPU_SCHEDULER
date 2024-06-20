#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <climits>
#include <queue>
#include <cfloat>

using namespace std;

struct process {
    int pid;
    int arrivaltime;
    int bursttime;
    int priority;
    int remainingtime;
    int waitingtime;
    int turnaroundtime;
    int completiontime;
};

bool comparearrival(process a, process b) {
    return a.arrivaltime < b.arrivaltime;
}

bool comparepid(process a, process b) {
    return a.pid < b.pid;
}


void calculatefcfs(process processes[], int n) {
    int currenttime = 0;
    sort(processes, processes + n, comparearrival);
    for (int i = 0; i < n; i++) {
        if (currenttime < processes[i].arrivaltime) {
            currenttime = processes[i].arrivaltime;
        }
        processes[i].completiontime = currenttime + processes[i].bursttime;
        processes[i].turnaroundtime = processes[i].completiontime - processes[i].arrivaltime;
        processes[i].waitingtime = processes[i].turnaroundtime - processes[i].bursttime;
        currenttime += processes[i].bursttime;
    }
    sort(processes, processes + n, comparepid);
}

void calculatenonpreemptivesjf(process processes[], int n) {
    sort(processes, processes + n, comparearrival);
    int currenttime = 0, completed = 0;
    bool visited[n] = {false};
    while (completed != n) {
        int idx = -1;
        int minburst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currenttime && !visited[i] && processes[i].bursttime < minburst) {
                minburst = processes[i].bursttime;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].completiontime = currenttime + processes[idx].bursttime;
            processes[idx].turnaroundtime = processes[idx].completiontime - processes[idx].arrivaltime;
            processes[idx].waitingtime = processes[idx].turnaroundtime - processes[idx].bursttime;
            currenttime = processes[idx].completiontime;
            visited[idx] = true;
            completed++;
        } else {
            currenttime++;
        }
    }
    sort(processes, processes + n, comparepid);
}

void calculatepreemptivesjf(process processes[], int n) {
    sort(processes, processes + n, comparearrival);
    int currenttime = 0, completed = 0;
    int idx = -1, minremainingtime = INT_MAX;
    while (completed != n) {
        idx = -1;
        minremainingtime = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currenttime && processes[i].remainingtime > 0 && processes[i].remainingtime < minremainingtime) {
                minremainingtime = processes[i].remainingtime;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].remainingtime--;
            currenttime++;
            if (processes[idx].remainingtime == 0) {
                processes[idx].completiontime = currenttime;
                processes[idx].turnaroundtime = processes[idx].completiontime - processes[idx].arrivaltime;
                processes[idx].waitingtime = processes[idx].turnaroundtime - processes[idx].bursttime;
                completed++;
            }
        } else {
            currenttime++;
        }
    }
    sort(processes, processes + n, comparepid);
}

void calculatenonpreemptivepriority(process processes[], int n) {
    sort(processes, processes + n, comparearrival);
    int currenttime = 0, completed = 0;
    bool visited[n] = {false};
    while (completed != n) {
        int idx = -1;
        int minpriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currenttime && !visited[i] && processes[i].priority < minpriority) {
                minpriority = processes[i].priority;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].completiontime = currenttime + processes[idx].bursttime;
            processes[idx].turnaroundtime = processes[idx].completiontime - processes[idx].arrivaltime;
            processes[idx].waitingtime = processes[idx].turnaroundtime - processes[idx].bursttime;
            currenttime = processes[idx].completiontime;
            visited[idx] = true;
            completed++;
        } else {
            currenttime++;
        }
    }
    sort(processes, processes + n, comparepid);
}

void calculatepreemptivepriority(process processes[], int n) {
    sort(processes, processes + n, comparearrival);
    int currenttime = 0, completed = 0;
    int idx = -1, minpriority = INT_MAX;
    while (completed != n) {
        idx = -1;
        minpriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currenttime && processes[i].remainingtime > 0 && processes[i].priority < minpriority) {
                minpriority = processes[i].priority;
                idx = i;
            }
        }
        if (idx != -1) {
            processes[idx].remainingtime--;
            currenttime++;
            if (processes[idx].remainingtime == 0) {
                processes[idx].completiontime = currenttime;
                processes[idx].turnaroundtime = processes[idx].completiontime - processes[idx].arrivaltime;
                processes[idx].waitingtime = processes[idx].turnaroundtime - processes[idx].bursttime;
                completed++;
            }
        } else {
            currenttime++;
        }
    }
    sort(processes, processes + n, comparepid);
}

void calculateroundrobin(process processes[], int n, int quantum) {
    sort(processes, processes + n, comparearrival);
    int currenttime = 0, completed = 0;
    queue<int> q;
    bool visited[n] = {false};
    q.push(0);
    visited[0] = true;
    while (!q.empty()) {
        int idx = q.front();
        q.pop();
        if (processes[idx].remainingtime > quantum) {
            currenttime += quantum;
            processes[idx].remainingtime -= quantum;
        } else {
            currenttime += processes[idx].remainingtime;
            processes[idx].remainingtime = 0;
            processes[idx].completiontime = currenttime;
            processes[idx].turnaroundtime = processes[idx].completiontime - processes[idx].arrivaltime;
            processes[idx].waitingtime = processes[idx].turnaroundtime - processes[idx].bursttime;
            completed++;
        }
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivaltime <= currenttime && processes[i].remainingtime > 0 && !visited[i]) {
                q.push(i);
                visited[i] = true;
            }
        }
        if (processes[idx].remainingtime > 0) {
            q.push(idx);
        }
    }
    sort(processes, processes + n, comparepid);
}

void calculatetimes(process processes[], int n, int algorithm, bool preemptive, int quantum) {
    for (int i = 0; i < n; i++) {
        processes[i].remainingtime = processes[i].bursttime;
    }
    switch (algorithm) {
        case 1:
            calculatefcfs(processes, n);
            break;
        case 2:
            if (preemptive) {
                calculatepreemptivesjf(processes, n);
            } else {
                calculatenonpreemptivesjf(processes, n);
            }
            break;
        case 3:
            if (preemptive) {
                calculatepreemptivepriority(processes, n);
            } else {
                calculatenonpreemptivepriority(processes, n);
            }
            break;
        case 4:
            calculateroundrobin(processes, n, quantum);
            break;
        default:
            cout << "Invalid algorithm selection!" << endl;
            return;
    }
}

double calculateaveragetime(process processes[], int n, bool turnaround) {
    double total = 0;
    for (int i = 0; i < n; i++) {
        if (turnaround) {
            total += processes[i].turnaroundtime;
        } else {
            total += processes[i].waitingtime;
        }
    }
    return total / n;
}

void printresults(process processes[], int n) {
    cout << "\n+-----------+--------------+------------+--------------+----------------+----------------+\n";
    cout << "| Process ID| Arrival Time | Burst Time | Waiting Time | Turnaround Time| Completion Time|\n";
    cout << "+-----------+--------------+------------+--------------+----------------+----------------+\n";
    for (int i = 0; i < n; i++) {
        cout << "| " << setw(10) << processes[i].pid << " | "
             << setw(12) << processes[i].arrivaltime << " | "
             << setw(10) << processes[i].bursttime << " | "
             << setw(12) << processes[i].waitingtime << " | "
             << setw(14) << processes[i].turnaroundtime << " | "
             << setw(14) << processes[i].completiontime << " |\n";
    }
    cout << "+-----------+--------------+------------+--------------+----------------+----------------+\n";
}

void suggestoptimalalgorithm(process processes[], int n, bool preemptive, int quantum) {
    int optimalalgorithm = 1;
    double optimalaveragetime = DBL_MAX;
    for (int algorithm = 1; algorithm <= 4; algorithm++) {
        process tempprocesses[n];
        copy(processes, processes + n, tempprocesses);
        calculatetimes(tempprocesses, n, algorithm, preemptive, quantum);
        double averagetime = calculateaveragetime(tempprocesses, n, false);
        if (averagetime < optimalaveragetime) {
            optimalaveragetime = averagetime;
            optimalalgorithm = algorithm;
        }
    }
    cout << "\nSuggested Optimal Algorithm: ";
    switch (optimalalgorithm) {
        case 1:
            cout << "First-Come, First-Served (FCFS)\n";
            break;
        case 2:
            cout << "Shortest Job First (SJF)\n";
            break;
        case 3:
            cout << "Priority Scheduling\n";
            break;
        case 4:
            cout << "Round Robin\n";
            break;
    }

    process optimalprocesses[n];
    copy(processes, processes + n, optimalprocesses);
    calculatetimes(optimalprocesses, n, optimalalgorithm, preemptive, quantum);
    printresults(optimalprocesses, n);
    double averagewaitingtime = calculateaveragetime(optimalprocesses, n, false);
    double averageturnaroundtime = calculateaveragetime(optimalprocesses, n, true);
    cout << "\nAverage Waiting Time for Optimal Algorithm: " << fixed << setprecision(2) << averagewaitingtime << "\n";
    cout << "Average Turnaround Time for Optimal Algorithm: " << fixed << setprecision(2) << averageturnaroundtime << "\n";
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    process processes[n];
    cout << "Enter details of each process (pid arrivalTime burstTime priority):\n";
    for (int i = 0; i < n; i++) {
        cin >> processes[i].pid >> processes[i].arrivaltime >> processes[i].bursttime >> processes[i].priority;
    }
    int algorithm, quantum = 0;
    bool preemptive = false;
    cout << "Select Scheduling Algorithm:\n";
    cout << "1. First-Come, First-Served (FCFS)\n";
    cout << "2. Shortest Job First (SJF)\n";
    cout << "3. Priority Scheduling\n";
    cout << "4. Round Robin\n";
    cout << "Enter your choice: ";
    cin >> algorithm;
    if (algorithm == 2 || algorithm == 3) {
        cout << "Is the algorithm preemptive? (1 for yes, 0 for no): ";
        cin >> preemptive;
    }
    if (algorithm == 4) {
        cout << "Enter time quantum: ";
        cin >> quantum;
    }
    calculatetimes(processes, n, algorithm, preemptive, quantum);
    printresults(processes, n);
    double averagewaitingtime = calculateaveragetime(processes, n, false);
    double averageturnaroundtime = calculateaveragetime(processes, n, true);
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << averagewaitingtime << "\n";
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << averageturnaroundtime << "\n";
    suggestoptimalalgorithm(processes, n, preemptive, quantum);
    return 0;
}