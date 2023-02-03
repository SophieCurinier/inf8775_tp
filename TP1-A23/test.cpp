#include <fstream>
#include <functional>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <cmath>

using Int = long long;
using Algo = const std::function<void(int**,int**, int**, int)>&;

void conv(int** matrixA, int** matrixB, int** res, int length){
    for (int i = 0; i<length; i++){
        for (int j = 0; j<length; j++){
            res[i][j] = 0;
            for (int k = 0; k<length; k++){
                res[i][j] = res[i][j] + matrixA[i][k]*matrixB[k][j];
            }
        }
    }
}

void add(int** matrixA, int** matrixB, int** res, int length){
    for (int i = 0; i < length; i++){
        for (int j = 0; j < length; j++){
            res[i][j] = matrixA[i][j]+matrixB[i][j];
        }
    }
}

void sub(int** matrixA, int** matrixB, int** res, int length){
    for (int i = 0; i < length; i++){
        for (int j = 0; j < length; j++){
            res[i][j] = matrixA[i][j]-matrixB[i][j];
        }
    }
}

void strassen(int** matrixA, int** matrixB, int** res, int length){
    if (length == 1){
        conv(matrixA,matrixB,res,length);
    }
    else{
        int k = length / 2;
        int** c11 = new int*[k];
        int** c12 = new int*[k];
        int** c21 = new int*[k];
        int** c22 = new int*[k];
        int** matrixA11 = new int*[k];
        int** matrixA12 = new int*[k];
        int** matrixA21 = new int*[k];
        int** matrixA22 = new int*[k];
        int** matrixB11 = new int*[k];
        int** matrixB12 = new int*[k];
        int** matrixB21 = new int*[k];
        int** matrixB22 = new int*[k];
        int** m1 = new int*[k];
        int** m2 = new int*[k];
        int** m3 = new int*[k];
        int** m4 = new int*[k];
        int** m5 = new int*[k];
        int** m6 = new int*[k];
        int** m7 = new int*[k];
        int** r1 = new int*[k];
        int** l1 = new int*[k];
        for (int j = 0; j < k; ++j){
            c11[j] = new int[k];
            c12[j] = new int[k];
            c21[j] = new int[k];
            c22[j] = new int[k];
            matrixA11[j] = new int[k];
            matrixA12[j] = new int[k];
            matrixA21[j] = new int[k];
            matrixA22[j] = new int[k];
            matrixB11[j] = new int[k];
            matrixB12[j] = new int[k];
            matrixB21[j] = new int[k];
            matrixB22[j] = new int[k];
            m1[j] = new int[k];
            m2[j] = new int[k];
            m3[j] = new int[k];
            m4[j] = new int[k];
            m5[j] = new int[k];
            m6[j] = new int[k];
            m7[j] = new int[k];
            r1[j] = new int[k];
            l1[j] = new int[k];
        }
        for (int i=0; i<length; i++){
            for (int j=0; j<length; j++){
                if (i<k && j<k){
                    matrixA11[i][j] = matrixA[i][j];
                    matrixB11[i][j] = matrixB[i][j];
                }
                else if (i>=k && j<k){
                    matrixA21[i-k][j] = matrixA[i][j];
                    matrixB21[i-k][j] = matrixB[i][j];
                }
                else if (i<k && j>=k){
                    matrixA12[i][j-k] = matrixA[i][j];
                    matrixB12[i][j-k] = matrixB[i][j];
                }
                else if (i>=k && j>=k){
                    matrixA22[i-k][j-k] = matrixA[i][j];
                    matrixB22[i-k][j-k] = matrixB[i][j];
                }
            }
        }
        // m1
        add(matrixA11,matrixA22,r1,k);
        add(matrixB11,matrixB22,l1,k);
        strassen(r1,l1,m1,k);
        // m2
        add(matrixA21,matrixA22,r1,k);
        strassen(r1,matrixB11,m2,k);
        // m3
        sub(matrixB12,matrixB22,l1,k);
        strassen(matrixA11,l1,m3,k);
        // m4
        sub(matrixB21,matrixB11,l1,k);
        strassen(matrixA22,l1,m4,k);
        // m5
        add(matrixA11,matrixA12,r1,k);
        strassen(r1,matrixB22,m5,k);
        // m6
        sub(matrixA21,matrixA11,r1,k);
        add(matrixB11,matrixB12,l1,k);
        strassen(r1,l1,m6,k);
        //m7
        sub(matrixA12,matrixA22,r1,k);
        add(matrixB21,matrixB22,l1,k);
        strassen(r1,l1,m7,k);
        // c11
        add(m1,m4,r1,k);
        sub(r1,m5,l1,k);
        add(l1,m7,c11,k);
        //c12
        add(m3,m5,c12,k);
        //c21
        add(m2,m4,c21,k);
        //c22
        add(m1,m3,r1,k);
        add(r1,m6,l1,k);
        sub(l1,m2,c22,k);

        for (int i=0; i<length; i++){
            for (int j=0; j<length; j++){
                if (i<k && j<k){
                    res[i][j] = c11[i][j];
                }
                else if (i>=k && j<k){
                    res[i][j] = c21[i-k][j];
                }
                else if (i<k && j>=k){
                    res[i][j] = c12[i][j-k];
                }
                else if (i>=k && j>=k){
                    res[i][j] = c22[i-k][j-k];
                }
            }
        }
        for (int j = 0; j < k; ++j){
            delete c11[j];
            delete c12[j];
            delete c21[j];
            delete c22[j];
            delete matrixA11[j];
            delete matrixA12[j];
            delete matrixA21[j];
            delete matrixA22[j];
            delete matrixB11[j];
            delete matrixB12[j];
            delete matrixB21[j];
            delete matrixB22[j];
            delete m1[j];
            delete m2[j];
            delete m3[j];
            delete m4[j];
            delete m5[j];
            delete m6[j];
            delete m7[j];
            delete r1[j];
            delete l1[j];
        }
        delete c11;
        delete c12;
        delete c21;
        delete c22;
        delete matrixA11;
        delete matrixA12;
        delete matrixA21;
        delete matrixA22;
        delete matrixB11;
        delete matrixB12;
        delete matrixB21;
        delete matrixB22;
        delete m1;
        delete m2;
        delete m3;
        delete m4;
        delete m5;
        delete m6;
        delete m7;
        delete r1;
        delete l1;
    }
}
void strassenSeuil(int** matrixA, int** matrixB, int** res, int length){
    if (length <= 1024){
        conv(matrixA,matrixB,res,length);
    }
    else{
        int k = length / 2;
        int** c11 = new int*[k];
        int** c12 = new int*[k];
        int** c21 = new int*[k];
        int** c22 = new int*[k];
        int** matrixA11 = new int*[k];
        int** matrixA12 = new int*[k];
        int** matrixA21 = new int*[k];
        int** matrixA22 = new int*[k];
        int** matrixB11 = new int*[k];
        int** matrixB12 = new int*[k];
        int** matrixB21 = new int*[k];
        int** matrixB22 = new int*[k];
        int** m1 = new int*[k];
        int** m2 = new int*[k];
        int** m3 = new int*[k];
        int** m4 = new int*[k];
        int** m5 = new int*[k];
        int** m6 = new int*[k];
        int** m7 = new int*[k];
        int** r1 = new int*[k];
        int** l1 = new int*[k];
        for (int j = 0; j < k; ++j){
            c11[j] = new int[k];
            c12[j] = new int[k];
            c21[j] = new int[k];
            c22[j] = new int[k];
            matrixA11[j] = new int[k];
            matrixA12[j] = new int[k];
            matrixA21[j] = new int[k];
            matrixA22[j] = new int[k];
            matrixB11[j] = new int[k];
            matrixB12[j] = new int[k];
            matrixB21[j] = new int[k];
            matrixB22[j] = new int[k];
            m1[j] = new int[k];
            m2[j] = new int[k];
            m3[j] = new int[k];
            m4[j] = new int[k];
            m5[j] = new int[k];
            m6[j] = new int[k];
            m7[j] = new int[k];
            r1[j] = new int[k];
            l1[j] = new int[k];
        }
        for (int i=0; i<length; i++){
            for (int j=0; j<length; j++){
                if (i<k && j<k){
                    matrixA11[i][j] = matrixA[i][j];
                    matrixB11[i][j] = matrixB[i][j];
                }
                else if (i>=k && j<k){
                    matrixA21[i-k][j] = matrixA[i][j];
                    matrixB21[i-k][j] = matrixB[i][j];
                }
                else if (i<k && j>=k){
                    matrixA12[i][j-k] = matrixA[i][j];
                    matrixB12[i][j-k] = matrixB[i][j];
                }
                else if (i>=k && j>=k){
                    matrixA22[i-k][j-k] = matrixA[i][j];
                    matrixB22[i-k][j-k] = matrixB[i][j];
                }
            }
        }
        // m1
        add(matrixA11,matrixA22,r1,k);
        add(matrixB11,matrixB22,l1,k);
        strassenSeuil(r1,l1,m1,k);
        // m2
        add(matrixA21,matrixA22,r1,k);
        strassenSeuil(r1,matrixB11,m2,k);
        // m3
        sub(matrixB12,matrixB22,l1,k);
        strassenSeuil(matrixA11,l1,m3,k);
        // m4
        sub(matrixB21,matrixB11,l1,k);
        strassenSeuil(matrixA22,l1,m4,k);
        // m5
        add(matrixA11,matrixA12,r1,k);
        strassenSeuil(r1,matrixB22,m5,k);
        // m6
        sub(matrixA21,matrixA11,r1,k);
        add(matrixB11,matrixB12,l1,k);
        strassenSeuil(r1,l1,m6,k);
        //m7
        sub(matrixA12,matrixA22,r1,k);
        add(matrixB21,matrixB22,l1,k);
        strassenSeuil(r1,l1,m7,k);
        // c11
        add(m1,m4,r1,k);
        sub(r1,m5,l1,k);
        add(l1,m7,c11,k);
        //c12
        add(m3,m5,c12,k);
        //c21
        add(m2,m4,c21,k);
        //c22
        add(m1,m3,r1,k);
        add(r1,m6,l1,k);
        sub(l1,m2,c22,k);

        for (int i=0; i<length; i++){
            for (int j=0; j<length; j++){
                if (i<k && j<k){
                    res[i][j] = c11[i][j];
                }
                else if (i>=k && j<k){
                    res[i][j] = c21[i-k][j];
                }
                else if (i<k && j>=k){
                    res[i][j] = c12[i][j-k];
                }
                else if (i>=k && j>=k){
                    res[i][j] = c22[i-k][j-k];
                }
            }
        }
        for (int j = 0; j < k; ++j){
            delete c11[j];
            delete c12[j];
            delete c21[j];
            delete c22[j];
            delete matrixA11[j];
            delete matrixA12[j];
            delete matrixA21[j];
            delete matrixA22[j];
            delete matrixB11[j];
            delete matrixB12[j];
            delete matrixB21[j];
            delete matrixB22[j];
            delete m1[j];
            delete m2[j];
            delete m3[j];
            delete m4[j];
            delete m5[j];
            delete m6[j];
            delete m7[j];
            delete r1[j];
            delete l1[j];
        }
        delete c11;
        delete c12;
        delete c21;
        delete c22;
        delete matrixA11;
        delete matrixA12;
        delete matrixA21;
        delete matrixA22;
        delete matrixB11;
        delete matrixB12;
        delete matrixB21;
        delete matrixB22;
        delete m1;
        delete m2;
        delete m3;
        delete m4;
        delete m5;
        delete m6;
        delete m7;
        delete r1;
        delete l1;
    }
}

void run(Algo algo, int**& matrixA, int**& matrixB,  int** res, int length, bool print_res, bool print_time) {
    using namespace std::chrono;
    auto start = steady_clock::now();
    algo(matrixA, matrixB, res, length);
    auto end = steady_clock::now();

    if (print_time) {
        duration<double> s = end-start;
        std::cout << std::fixed << s.count() << std::endl;
    }
    if (print_res) {
        for (int i=0; i < length; i++){
            for (int j=0; j < length; j++){
                if (j==(length-1)){
                    std::cout << res[i][j] << "\n";    
                }
                else{
                    std::cout << res[i][j] << " ";
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    struct {
        std::string algo;
        std::string file_path1;
        std::string file_path2;
        bool print_res{false};
        bool print_time{false};
    } prog_args;

    // Read program arguments
    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "-a") {
            prog_args.algo = argv[i+1]; i++;
        } else if (arg == "-e1") {
            prog_args.file_path1 = argv[i+1]; i++;
        } else if (arg == "-e2") {
            prog_args.file_path2 = argv[i+1]; i++;
        } else if (arg == "-p") {
            prog_args.print_res = true;
        } else if (arg == "-t") {
            prog_args.print_time = true;
        }
    }

    // Read file and saved text information into matrix
    int length;
    int **matrixA;
    {
        std::fstream ex_file(prog_args.file_path1);
        Int i = -1;
        Int n;
        while (ex_file >> n){
            if (i == -1){
                length = int (pow(2,n));
                matrixA = new int*[length];
                for (int j = 0; j < length; ++j)
                    matrixA[j] = new int[length];
                ++i;
            } else{
                matrixA[int ((i)/length)][int((i) % length)] = n;
                ++ i;
            }
        }
    }
    int **matrixB;
    {
        std::fstream ex_file(prog_args.file_path2);
        Int i = -1;
        Int n;
        while (ex_file >> n){
            if (i == -1){
                matrixB = new int*[length];
                for (int j = 0; j < length; ++j)
                    matrixB[j] = new int[length];
                ++i;
            } else{
                matrixB[int ((i)/length)][int((i) % length)] = n;
                ++ i;
            }
        }
    }
    int **res;
    {
        res = new int*[length];
        for (int j = 0; j < length; ++j){
            res[j] = new int[length];
        }
    }

    // Apply correct algorithm
    if(prog_args.algo == "conv")
        run(conv, matrixA, matrixB, res, length, prog_args.print_res, prog_args.print_time);
    else if(prog_args.algo == "strassen")
        run(strassen, matrixA, matrixB, res, length, prog_args.print_res, prog_args.print_time);
    else if(prog_args.algo == "strassenSeuil")
        run(strassenSeuil, matrixA, matrixB, res, length, prog_args.print_res, prog_args.print_time);
    for (int j = 0; j < length; ++j){
        delete matrixA[j];
        delete matrixB[j];
        delete res[j];
    }
    delete matrixA;
    delete matrixB;
    delete res;
}