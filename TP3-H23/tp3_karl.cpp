#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <vector>
#include <numeric>

using namespace std;
using Algo = const function<vector<vector<vector<int>>>(int, int, int, vector<int>, vector<int>, vector<vector<int>>)>&;

int distance(int x1, int y1, int x2, int y2) {
    return abs(x2-x1) + abs(y2-y1);
}

vector<vector<vector<int>>> algo(int n, int m, int k, vector<int> theme, vector<int> size, vector<vector<int>> weight) {
    vector<vector<vector<int>>> sol(n, vector<vector<int>>());
    vector<int> remaining(n);
    iota(remaining.begin(), remaining.end(), 0);

    for(int i = 0; i < theme.size(); i++) {
        int idx = theme[i];
        vector<vector<int>> enclos{};

        if (i % 2 == 0) {
            for (int j = 0; j < size[idx]; j++) {
                enclos.push_back({i/2, j});
            }
        }
        else {
            for(int j = 0; j < size[idx]; j++) {
                enclos.push_back({(i-1)/2, -j-1});
            }
        }
        sol[idx] = enclos;
        remaining.erase(remove(remaining.begin(), remaining.end(), idx), remaining.end());
    }

    for(int i = 0; i < remaining.size(); i++) {
        int idx = remaining[i];
        vector<vector<int>> enclos;
        if(i % 2 == 0) {
            for(int j = 0; j < size[idx]; j++) {
                enclos.push_back({-i/2-1, j});
            }
        }
        else {
            for(int j = 0; j < size[idx]; j++) {
                enclos.push_back({-(i-1)/2-1, -j-1});
            }
        }

        sol[idx] = enclos;
    }

    return sol;
}

void print() {
    cout << "Printing result" << endl;
}

void run(Algo algo, int n, int m, int k, vector<int> theme, vector<int> size, vector<vector<int>> weight, bool print_res, bool print_time) {
    using namespace chrono;
    auto start = steady_clock::now();
    vector<vector<vector<int>>> sol = algo(n, m, k, theme, size, weight);
    auto end = steady_clock::now();

    ofstream outfile("outc.txt");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < size[i] - 1; j++) {
            outfile << sol[i][j][0] << " " << sol[i][j][1] << " ";
        }
        outfile << sol[i].back().front() << " " << sol[i].back()[1] << endl;
    }

    outfile.close();

    if (print_time) {
        duration<double> s = end-start;
        std::cout << std::fixed << s.count() << std::endl;
    }

    if (print_res) {
        print();
    }
}

/* || Main method || */
int main(int argc, char *argv[]) {
    struct {
        string ex;
        bool print_res{false};
        bool print_time{false};
    } prog_args;

    // Parse program arguments
    for (int i = 1; i < argc; ++i) {
        string arg(argv[i]);    
        if (arg == "-e") {
            prog_args.ex = argv[i+1]; i++;
        } else if (arg == "-p") {
            prog_args.print_res = true;
        } 
    }
    
    // Initialize resultat and parameters
    int numberOfEnclosures_n;
    int numberOfSubSet_m;
    int maxDistanceSubSet_k;
    vector<int> theme;
    vector<int> size;
    vector<vector<int>> weight;

    {
        std::fstream ex_file(prog_args.ex);

        string line;
        // First line
        getline(ex_file, line);
        istringstream ss1(line);
        ss1 >> numberOfEnclosures_n >> numberOfSubSet_m >> maxDistanceSubSet_k;

        // Theme vector
        getline(ex_file, line);
        istringstream ss2(line);
        int num;
        while (ss2 >> num) {
            theme.push_back(num);
        }

        // Size vector
        int i = 0;
        while (i < numberOfEnclosures_n && getline(ex_file, line)) {
            istringstream ss3(line);
            ss3 >> num;
            size.push_back(num);
            i++;
        }

        // Weights matrix
        while (getline(ex_file, line)) {
            istringstream ss4(line);
            vector<int> l;
            while (ss4 >> num) {
                l.push_back(num);
            }
            weight.push_back(l);
        }
    }

    // Run algorithm to solve placing enclosures at a zoo
    run(algo, numberOfEnclosures_n, numberOfSubSet_m, maxDistanceSubSet_k, theme, size, weight,prog_args.print_res, prog_args.print_time);

    return 0;
}