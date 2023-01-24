#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <chrono>
#include <algorithm>
using namespace std;
using Algo = const std::function<void(std::vector<int>&)>&;

vector<vector<int>> conv(vector<vector<int>> a, vector<vector<int>> b, int n){
    vector<vector<int>> mult;
    int i;
    int k;
    int j;

    // Multiplying matrix a and b and storing in array mult.
    for(i = 0; i < n; ++i)
        for(j = 0; j < n; ++j)
            for(k = 0; k < n; ++k)
            {
                mult[i][j] += a[i][k] * b[k][j];
            }
    
    return mult;
};

void run(Algo algo, std::vector<int>& numbers, bool print_res, bool print_time) {
    using namespace std::chrono;
    auto start = steady_clock::now();
    algo(numbers);
    auto end = steady_clock::now();

    if (print_time) {
        duration<double> s = end-start;
        std::cout << std::fixed << s.count() << std::endl;
    }

    if (print_res) {
        std::cout << std::fixed;
        for (auto n : numbers)
            std::cout << n << std::endl;
    }
};

vector<vector<int>> add(vector<vector<int>> a, vector<vector<int>> b, int n){
    int i;
    int j;
    vector<vector<int>> result(n, vector<int> (n, 0));
    for (i=0; i<n; ++i)
        for (j=0; j<n; ++j)
            result[i][j] = a[i][j] + b[i][j];
    
    return result;
}

vector<vector<int>> substract(vector<vector<int>> a, vector<vector<int>> b, int n){
    int i;
    int j;
    vector<vector<int>> result(n, vector<int> (n, 0));
    for (i=0; i<n; ++i)
        for (j=0; j<n; ++j)
            result[i][j] = a[i][j] - b[i][j];
    
    return result;
}

vector<vector<int>> strassenMultiply(vector<vector<int>> A, vector<vector<int>> B, int n){
    int k = n/2;
    int i;
    int j;
    vector<vector<int>> C;

    if(n==1){
        C[1][1] = A[1][1]*B[1][1];
    }
    else{
        vector<vector<int>> A11(k, vector<int> (k, 0));
    vector<vector<int>> A12(k, vector<int> (k, 0));
    vector<vector<int>> A21(k, vector<int> (k, 0));
    vector<vector<int>> A22(k, vector<int> (k, 0));
    vector<vector<int>> B11(k, vector<int> (k, 0));
    vector<vector<int>> B12(k, vector<int> (k, 0));
    vector<vector<int>> B21(k, vector<int> (k, 0));
    vector<vector<int>> B22(k, vector<int> (k, 0));

    for(int i=0; i<k; i++)
    for(int j=0; j<k; j++) {
        A11[i][j] = A[i][j];
        A12[i][j] = A[i][k+j];
        A21[i][j] = A[k+i][j];
        A22[i][j] = A[k+i][k+j];
        B11[i][j] = B[i][j];
        B12[i][j] = B[i][k+j];
        B21[i][j] = B[k+i][j];
        B22[i][j] = B[k+i][k+j];
    }

    vector<vector<int>> P1;
    vector<vector<int>> P2;
    vector<vector<int>> P3;
    vector<vector<int>> P4;
    vector<vector<int>> P5;
    vector<vector<int>> P6;
    vector<vector<int>> P7;

    P1 = strassenMultiply(A11,substract(B12, B22, k),k);
    P2 = strassenMultiply(add(A11,A12,k), B22,k);
    P3 = strassenMultiply(add(A21, A22, k), B11, k);
    P4 = strassenMultiply(A22,substract(B21, B11, k), k);
    P5 = strassenMultiply(add(A11, A22, k), add(B11, B22, k), k);
    P6 = strassenMultiply(substract(A12, A22, k), add(B21, B22, k), k);
    P7 = strassenMultiply(substract(A11, A21, k), add(B11, B12, k), k);

    vector<vector<int>> C11;
    vector<vector<int>> C12;
    vector<vector<int>> C21;
    vector<vector<int>> C22;
    C11 = substract(add(add(P5,P4,k),P6,k),P2,k);
    C12 = add(P1,P2,k);
    C21 = add(P3,P4,k);
    C22 = substract(substract(add(P5,P1,k),P3,k),P7,k);

    
    for(int i=0; i<k; i++)
        for(int j=0; j<k; j++) {
            C[i][j] = C11[i][j];
            C[i][j+k] = C12[i][j];
            C[k+i][j] = C21[i][j];
            C[k+i][k+j] = C22[i][j];
        }
    }
    return C;
}

int main(int argc, char *argv[]) {
    struct {
        std::string algo;
        std::string file_path;
        bool print_res{false};
        bool print_time{false};
    } prog_args;

    // Read program arguments
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-a") {
            prog_args.algo = argv[i+1]; i++;
        } else if (arg == "-e1") {
            prog_args.file_path = argv[i+1]; i++;
        } else if (arg == "-e2") {
            prog_args.file_path = argv[i+1]; i++;
        } else if (arg == "-p") {
            prog_args.print_res = true;
        } else if (arg == "-t") {
            prog_args.print_time = true;
        }
    }

    // Read numbers into file with file path
    std::vector<int> matrix;
    {
        std::fstream ex_file(prog_args.file_path);
        int n;
        while (ex_file >> n)
            matrix.push_back(n);
    }

    // Apply correct algorithm
    if (prog_args.algo == "conv")
        // run(conv, numbers, prog_args.print_res, prog_args.print_time);
        matrix;
        
    else if(prog_args.algo == "strassen")
        // run(strassen, numbers, prog_args.print_res, prog_args.print_time);
        1;
    else if(prog_args.algo == "strassenSeuil")
        // run(strassenSeuil, numbers, prog_args.print_res, prog_args.print_time);
        1;
}