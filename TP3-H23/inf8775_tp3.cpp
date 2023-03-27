#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <limits>

using namespace std;
using Algo = const function<void(int**&,int*&, int**&, int*&, int, int, int)>&;

// Useful method

/* || Method for algo1 || */
void algo1(int**& weightedMatrix, int*& subSet, int**& res,  int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet){

}

/* || Methods for algo2 || */

/* || Methods for algo3 || */

/* || Print enclosure map methods || */
void printEnclosure(int**& res,  int*& enclosureSize, int numberOfEnclosure){
    for (int enclosure=0; enclosure<numberOfEnclosure; enclosure++){
        for (int i=0; i<enclosureSize[enclosure]; i++){
            std::cout << res[enclosure][i] << " " << res[enclosure][i+1] << " ";
        }
        std::cout << "\n";
    }
}

void run(Algo algo, int**& weightedMatrix, int*& subSet, int**& res,  int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, bool print_res, bool print_time) {
    using namespace chrono;
    auto start = steady_clock::now();
    algo(weightedMatrix, subSet, res, enclosureSize, numberOfEnclosure, numberOfSubset, maxDistanceSubSet);
    auto end = steady_clock::now();

    if (print_time) {
        duration<double> s = end-start;
        std::cout << std::fixed << s.count() << std::endl;
    }
    if (print_res) {
        printEnclosure(res, enclosureSize, numberOfEnclosure);
    }
}

/* || Main method || */
int main(int argc, char *argv[]) {
    struct {
        string algo;
        string ex;
        bool print_res{false};
        bool print_time{false};
    } prog_args;

    // Parse program arguments
    for (int i = 1; i < argc; ++i) {
        string arg(argv[i]);    
        if (arg == "-a") {
            prog_args.algo = argv[i+1]; i++;
        } else if (arg == "-e") {
            prog_args.ex = argv[i+1]; i++;
        } else if (arg == "-p") {
            prog_args.print_res = true;
        } else if (arg == "-t") {
            prog_args.print_time = true;
        }
    }
    // Initialize resultat and parameters
    int numberOfEnclosure = 0;
    int numberOfSubSet = 0;
    int maxDistanceSubSet = 0;
    int **weightedMatrix ;
    int *subSet;
    int **res;
    int *enclosureSize ;
    {
        std::fstream ex_file(prog_args.ex);
        int p = -3;
        int n;
        while (ex_file >> n){
            if (p == -3){
                numberOfEnclosure = int (n);
                res = new int*[numberOfEnclosure];
                weightedMatrix = new int*[numberOfEnclosure];
                enclosureSize = new int[numberOfEnclosure];
                for (int line = 0; line<numberOfEnclosure; line++){
                    weightedMatrix[line] = new int[numberOfEnclosure];
                }
            } else if (p == -2){
                numberOfSubSet = int (n);
                subSet = new int[numberOfSubSet];
            } else if (p == -1) {
                maxDistanceSubSet = int (n);
            } else{
                if (p < numberOfSubSet) {
                    subSet[p] = int (n);
                } else if ((p >= numberOfSubSet) && (p < numberOfEnclosure+numberOfSubSet)){
                    enclosureSize[p-numberOfSubSet] = int (n);
                    res[p-numberOfSubSet] = new int [2*int(n)];
                } else {
                    weightedMatrix[(int)((p-numberOfSubSet-numberOfEnclosure)/numberOfEnclosure)][(p-numberOfSubSet-numberOfEnclosure)%numberOfEnclosure] = int (n);
                }
            }
            ++ p;
        }
    }

    // Call correct algorithm
    if (prog_args.algo == "algo1") {
        run(algo1, weightedMatrix, subSet, res, enclosureSize, numberOfEnclosure, numberOfSubSet, maxDistanceSubSet, prog_args.print_res, prog_args.print_time);
    }

    // Desallocate array
    for (int n=0; n<numberOfEnclosure; n++){
        delete weightedMatrix[n] ;
        delete res[n] ;
    }

    delete weightedMatrix ;
    delete subSet ;
    delete res ;
    delete enclosureSize ;

    return 0;
}
