#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <cmath>

using namespace std;
using Algo = const function<void(vector<vector<int>>, vector<vector<int>>, vector<vector<int>>&)>&;

#define seuil 2

void conv(vector<vector<int>> ex1, vector<vector<int>> ex2, vector<vector<int>> &res) {
    for (int i = 0; i < ex1.size(); ++i) {
        for (int j = 0; j < ex1.size(); ++j) {
            res[i][j] = 0;
            for (int k = 0; k < ex1.size(); ++k) {
                res[i][j] += ex1[i][k]*ex2[k][j];
            }
        }
    }
}

vector<vector<int>> extractMatrix(vector<vector<int>> matrix, int fromL, int toL, int fromC, int toC) {
    vector<vector<int>> sub;
    for (int i = fromL; i < toL; ++i) {
        sub.push_back(vector<int>(matrix[i].begin() + fromC, matrix[i].begin() + toC));
    }

    return sub;
}

void addMatrix(vector<vector<int>> ex1, vector<vector<int>> ex2, vector<vector<int>> &res){
    for (int i = 0; i < ex1.size(); i++){
        for (int j = 0; j < ex1.size(); j++){
            res[i][j] = ex1[i][j] + ex2[i][j];
        }
    }
}

void subMatrix(vector<vector<int>> ex1, vector<vector<int>> ex2, vector<vector<int>> &res){
    for (int i = 0; i < ex1.size(); i++){
        for (int j = 0; j < ex1.size(); j++){
            res[i][j] = ex1[i][j] - ex2[i][j];
        }
    }
}

void strassen(vector<vector<int>> ex1, vector<vector<int>> ex2, vector<vector<int>> &res) {
    if (ex1.size() == 1) {
        res[0][0] = ex1[0][0] * ex2[0][0];
        return;
    } else {
        vector<vector<int>> A11 = extractMatrix(ex1, 0, ex1.size() / 2, 0, ex1.size() / 2); 
        vector<vector<int>> A12 = extractMatrix(ex1, 0, ex1.size() / 2, ex1.size() / 2, ex1.size()); 
        vector<vector<int>> A21 = extractMatrix(ex1, ex1.size() / 2, ex1.size(), 0, ex1.size() / 2); 
        vector<vector<int>> A22 = extractMatrix(ex1, ex1.size() / 2, ex1.size(), ex1.size() / 2, ex1.size());
        vector<vector<int>> B11 = extractMatrix(ex2, 0, ex2.size() / 2, 0, ex2.size() / 2); 
        vector<vector<int>> B12 = extractMatrix(ex2, 0, ex2.size() / 2, ex2.size() / 2, ex2.size()); 
        vector<vector<int>> B21 = extractMatrix(ex2, ex2.size() / 2, ex2.size(), 0, ex2.size() / 2); 
        vector<vector<int>> B22 = extractMatrix(ex2, ex2.size() / 2, ex2.size(), ex2.size() / 2, ex2.size());
        
        vector<int> tmp(ex1.size() / 2);
        vector<vector<int>> result1(ex1.size() / 2, tmp), result2(ex1.size() / 2, tmp);
        vector<vector<int>> C11(ex1.size() / 2, tmp);
        vector<vector<int>> C12(ex1.size() / 2, tmp);
        vector<vector<int>> C21(ex1.size() / 2, tmp);
        vector<vector<int>> C22(ex1.size() / 2, tmp);
        

        vector<vector<int>> M1(ex1.size() / 2, tmp);
        addMatrix(A11, A22, result1);
        addMatrix(B11, B22, result2);
        strassen(result1, result2, M1);
        
        vector<vector<int>> M2(ex1.size() / 2, tmp);
        addMatrix(A21, A22, result1);
        strassen(result1, B11, M2);
        
        vector<vector<int>> M3(ex1.size() / 2, tmp);
        subMatrix(B12, B22, result1);
        strassen(A11, result1, M3);
        
        vector<vector<int>> M4(ex1.size() / 2, tmp);
        subMatrix(B21, B11, result1);
        strassen(A22, result1, M4);
        
        vector<vector<int>> M5(ex1.size() / 2, tmp);
        addMatrix(A11, A12, result1);
        strassen(result1, B22, M5);
        
        vector<vector<int>> M6(ex1.size() / 2, tmp);
        subMatrix(A21, A11, result1);
        addMatrix(B11, B12, result2);
        strassen(result1, result2, M6);
        
        vector<vector<int>> M7(ex1.size() / 2, tmp);
        subMatrix(A12, A22, result1);
        addMatrix(B21, B22, result2);
        strassen(result1, result2, M7);

        // C11
        addMatrix(M1, M4, result1);
        addMatrix(result1, M7, result2);
        subMatrix(result2, M5, C11);
        
        // C12
        addMatrix(M3, M5, C12);
        
        //C21
        addMatrix(M2, M4, C21);
        
        // C22
        addMatrix(M1, M3, result1);
        addMatrix(result1, M6, result2);
        subMatrix(result2, M2, C22);

        // Coller res
        for (int i = 0; i < ex1.size() / 2; i++)
        {
            for (int j = 0; j < ex1.size() / 2; j++)
            {
                res[i][j] = C11[i][j];
                res[i][j + ex1.size() / 2] = C12[i][j];
                res[i + ex1.size() / 2][j] = C21[i][j];
                res[i + ex1.size() / 2][j + ex1.size() / 2] = C22[i][j];
            }
        }
    }
}

void strassenSeuil(vector<vector<int>> ex1, vector<vector<int>> ex2, vector<vector<int>> &res) {
    if (ex1.size() == seuil) {
        conv(ex1, ex2, res);
        return;
    } else {
        vector<vector<int>> A11 = extractMatrix(ex1, 0, ex1.size() / 2, 0, ex1.size() / 2); 
        vector<vector<int>> A12 = extractMatrix(ex1, 0, ex1.size() / 2, ex1.size() / 2, ex1.size()); 
        vector<vector<int>> A21 = extractMatrix(ex1, ex1.size() / 2, ex1.size(), 0, ex1.size() / 2); 
        vector<vector<int>> A22 = extractMatrix(ex1, ex1.size() / 2, ex1.size(), ex1.size() / 2, ex1.size());
        vector<vector<int>> B11 = extractMatrix(ex2, 0, ex2.size() / 2, 0, ex2.size() / 2); 
        vector<vector<int>> B12 = extractMatrix(ex2, 0, ex2.size() / 2, ex2.size() / 2, ex2.size()); 
        vector<vector<int>> B21 = extractMatrix(ex2, ex2.size() / 2, ex2.size(), 0, ex2.size() / 2); 
        vector<vector<int>> B22 = extractMatrix(ex2, ex2.size() / 2, ex2.size(), ex2.size() / 2, ex2.size());
        
        vector<int> tmp(ex1.size() / 2);
        vector<vector<int>> result1(ex1.size() / 2, tmp), result2(ex1.size() / 2, tmp);
        vector<vector<int>> C11(ex1.size() / 2, tmp);
        vector<vector<int>> C12(ex1.size() / 2, tmp);
        vector<vector<int>> C21(ex1.size() / 2, tmp);
        vector<vector<int>> C22(ex1.size() / 2, tmp);
        

        vector<vector<int>> M1(ex1.size() / 2, tmp);
        addMatrix(A11, A22, result1);
        addMatrix(B11, B22, result2);
        strassen(result1, result2, M1);
        
        vector<vector<int>> M2(ex1.size() / 2, tmp);
        addMatrix(A21, A22, result1);
        strassen(result1, B11, M2);
        
        vector<vector<int>> M3(ex1.size() / 2, tmp);
        subMatrix(B12, B22, result1);
        strassen(A11, result1, M3);
        
        vector<vector<int>> M4(ex1.size() / 2, tmp);
        subMatrix(B21, B11, result1);
        strassen(A22, result1, M4);
        
        vector<vector<int>> M5(ex1.size() / 2, tmp);
        addMatrix(A11, A12, result1);
        strassen(result1, B22, M5);
        
        vector<vector<int>> M6(ex1.size() / 2, tmp);
        subMatrix(A21, A11, result1);
        addMatrix(B11, B12, result2);
        strassen(result1, result2, M6);
        
        vector<vector<int>> M7(ex1.size() / 2, tmp);
        subMatrix(A12, A22, result1);
        addMatrix(B21, B22, result2);
        strassen(result1, result2, M7);

        // C11
        addMatrix(M1, M4, result1);
        addMatrix(result1, M7, result2);
        subMatrix(result2, M5, C11);
        
        // C12
        addMatrix(M3, M5, C12);
        
        //C21
        addMatrix(M2, M4, C21);
        
        // C22
        addMatrix(M1, M3, result1);
        addMatrix(result1, M6, result2);
        subMatrix(result2, M2, C22);

        // Coller res
        for (int i = 0; i < ex1.size() / 2; i++)
        {
            for (int j = 0; j < ex1.size() / 2; j++)
            {
                res[i][j] = C11[i][j];
                res[i][j + ex1.size() / 2] = C12[i][j];
                res[i + ex1.size() / 2][j] = C21[i][j];
                res[i + ex1.size() / 2][j + ex1.size() / 2] = C22[i][j];
            }
        }
    }
}

void run(Algo algo, vector<vector<int>> ex1, vector<vector<int>> ex2, vector<vector<int>> &res, bool print_res, bool print_time) {
    using namespace chrono;
    auto start = steady_clock::now();
    algo(ex1, ex2, res);
    auto end = steady_clock::now();

    if (print_time) {
        duration<double> s = end - start;   
        cout << fixed << s.count() << endl;
    }

    if (print_res) {
        cout << fixed; 
        for (int i = 0; i < res.size(); ++i) {
            for (int j = 0; j < res.size(); ++j) {
                cout << res[i][j] << " ";
            }
            cout << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    struct {
        string algo;
        string ex1;
        string ex2;
        bool print_res{false};
        bool print_time{false};
    } prog_args;

    // Parse program arguments
    for (int i = 1; i < argc; ++i) {
        string arg(argv[i]);    
        if (arg == "-a") {
            prog_args.algo = argv[i+1]; i++;
        } else if (arg == "-e1") {
            prog_args.ex1 = argv[i+1]; i++;
        } else if (arg == "-e2") {
            prog_args.ex2 = argv[i+1]; i++;
        } else if (arg == "-p") {
            prog_args.print_res = true;
        } else if (arg == "-t") {
            prog_args.print_time = true;
        }
    }

    // Read data into 2D static array   
    fstream e1(prog_args.ex1);
    fstream e2(prog_args.ex2);

    // Remove first line  from each exemplaire
    string line;
    int num;
    getline(e1, line);
    getline(e2, line);

    // Initialize matrixes
    vector<vector<int>> ex1 = {};
    vector<vector<int>> ex2 = {};

    while (getline(e1, line)) {
        istringstream ss(line);
        vector<int> l;
        while (ss >> num) {
            l.push_back(num);
        }  
        ex1.push_back(l);
    }

    while (getline(e2, line)) {
        istringstream ss(line);
        vector<int> l;
        while (ss >> num) {
            l.push_back(num);
        }  
        ex2.push_back(l);  
    }

    e1.close();
    e2.close();

    vector<vector<int>> res(ex1.size(), vector<int>(ex1.size()));

    // Call correct algorithm
    if (prog_args.algo == "conv") {
        run(conv, ex1, ex2, res, prog_args.print_res, prog_args.print_time);
    } else if(prog_args.algo == "strassen") {
        run(strassen, ex1, ex2, res, prog_args.print_res, prog_args.print_time);
    } else if(prog_args.algo == "strassenSeuil") {
        run(strassenSeuil, ex1, ex2, res, prog_args.print_res, prog_args.print_time);
    }
    
    return 0;
}
