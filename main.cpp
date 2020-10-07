#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <climits>
#include <utility> // pair
#include <ctime>

using namespace std;

// first: Process Length (pj)
// second: Arrival Time (rj)
vector<int> findWaitingTime(vector<pair<int, int>> proc, int n, vector<int> wt) {
    int pl[n];

    // copy the process length into pl[]
    for (int i = 0; i < n; i++) {
        pl[i] = proc[i].first;
    }

    int complete = 0, t = 0, min = INT_MAX;
    int shortest = 0, finishTime = 0;
    bool check = false;

    // repeat until all processes get completed
    while (complete != n) {

        // find process with minimum remaining time among the processes that arrives until current time
        for (int j = 0; j < n; j++) {
            if ((proc[j].second <= t) && (pl[j] < min) && pl[j] > 0) {
                min = pl[j];
                shortest = j;
                check = true;
            }
        }

        // if there's no remaining process, continues counting time
        if (check == false) {
            t++;
            continue;
        }

        // reduce remaining process time by one
        pl[shortest]--;

        // update minimum
        min = pl[shortest];
        if (min == 0)
            min = INT_MAX;

        // if a process gets completed
        if (pl[shortest] == 0) {
            complete++;
            check = false;

            // finish time of current process
            finishTime = t + 1;
            wt[shortest] = finishTime;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        // increment time
        t++;
    }
    return wt;
}

int main() {
    // read csv
    vector<pair<int, int>> data;
    ifstream myFile("/home/ziva/CLionProjects/C++/SRPT/test_instance.csv");

    // Make sure the file is open
    if (!myFile.is_open()) throw runtime_error("Could not open file");

    // helper vars
    string line, colname;
    int tempCol = 0;
    // read first line (the column names)
    if (myFile.good()) {
        getline(myFile, line);

        // create a stringstream from line
        stringstream ss(line);

        // extract each column name
        while (getline(ss, colname, ',')) {
            // initialize and add <colname, int> pairs to data
            stringstream colnameS(colname);
            colnameS >> tempCol;
            data.push_back({tempCol, 0});
        }
    }

    while (getline(myFile, line)) {
        stringstream ss(line);
        string val;

        int colIdx = 0;
        int temp = 0;

        while (getline(ss, val, ',')) {
            // convert string to int
            stringstream valS(val);
            valS >> temp;

            data[colIdx].second = temp;
            colIdx++;
        }
    }
    myFile.close();

    // delete first column r_j, p_j
    data.erase(data.begin());

    // todo sort by Arrival Time (second param)?
    vector<pair<int, int>> rawData = data;

    // execute SRPT
    cout << "There are k jobs...." << endl;
    for (int k = 20; k <= 100; k += 20) {
        vector<pair<int, int>> process;
        cout << "k = " << k << ", ";

        // prepare data for SRPT
        for (int j = 0; j < k; j++) {
            process.push_back({data[j].first, data[j].second});
//            cout << data[j].second << ", ";
        }
//        cout << endl;
        for (int j = 0; j < k; j++) {
//            cout << data[j].first << ",  ";
        }
//        cout << endl;

        // SRPT
        vector<int> wt(k);
        clock_t start = clock();
        wt = findWaitingTime(process, process.size(), wt);
        clock_t end = clock();

        // run time
        double elapsed_secs = double(end - start);
        cout << "elapsed run time: " << elapsed_secs << " ms, ";

        // count total process finished time
        int total = 0;
        for (int b = 0; b < wt.size(); b++) {
            total += wt[b];
        }
        cout << "Objective Value: " << total << endl;
    }
    return 0;
}
