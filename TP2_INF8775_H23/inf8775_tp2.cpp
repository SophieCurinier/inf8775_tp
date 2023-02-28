#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <cmath>


using namespace std;
using Algo = const function<void(int**,int*, int)>&;

// Useful method
int arbitraryCity(int length){
    return 0;
}
int euclideanDistance(int x1, int y1, int x2, int y2){
    return lround(sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
}

void glouton(int** matrix, int* res, int length) {
    int total_dist = 0;
    // Initialized all city as unvisited
    bool * visitedCites = new bool[length];
    // Select an arbitrary city
    int currentCity = arbitraryCity(length);
    visitedCites[currentCity] = true;
    int nbUnvisitedCities = length - 1;
    res[0] = currentCity;

    // Calcul matrix Distance
    int **distanceMatrix = new int*[length];
    for (int i = 0; i<length; i++){
        distanceMatrix[i] = new int[length];
    }
    int distanceBetweenIAndJ ;
    for (int i=0; i<length; i++){
        for (int j=0; j<length; j++){
            if (i == j){
                distanceMatrix[i][j] == 0;
            } else {
                distanceBetweenIAndJ = euclideanDistance(matrix[i][0],matrix[i][1],matrix[j][0],matrix[j][1]);
                distanceMatrix[i][j] = distanceBetweenIAndJ;
                distanceMatrix[j][j] = distanceBetweenIAndJ;
            } 
        }   
    }

    // Find out the shortest edge connecting the current city and an unvisited city
    while (nbUnvisitedCities > 0){
        int nearestUnvisitedCity = -1;
        int distanceWithNearest;
        for (int i=0; i<length; i++){
            if (!visitedCites[i]){
                int distanceWithI = distanceMatrix[currentCity][i];
                if (nearestUnvisitedCity >= 0){
                    if (distanceWithNearest > distanceWithI){
                        nearestUnvisitedCity = i;
                        distanceWithNearest = distanceWithI;
                    }
                } else {
                    nearestUnvisitedCity = i;
                    distanceWithNearest = distanceWithI;
                }
            }
        }
        total_dist += distanceWithNearest;
        visitedCites[nearestUnvisitedCity] = true;
        res[length-nbUnvisitedCities] = nearestUnvisitedCity;
        nbUnvisitedCities -= 1;
        currentCity = nearestUnvisitedCity;
    }
    res[length] = res[0];

    // Delete arrays
    delete visitedCites;
    for (int i=0; i<length; i++){
        delete distanceMatrix[i];
    }
    delete distanceMatrix;
}

void progdyn(int** matrix, int* res, int length) {
    
}

void approx(int** matrix, int* res, int length) {

}

void run(Algo algo, int**& matrix, int* res, int length, bool print_res, bool print_time) {
    using namespace chrono;
    auto start = steady_clock::now();
    algo(matrix, res, length);
    auto end = steady_clock::now();

    if (print_time) {
        duration<double> s = end-start;
        std::cout << std::fixed << s.count() << std::endl;
    }
    if (print_res) {
        for (int i=0; i <= length; i++){
            std::cout << res[i] << "\n";    
        }
    }
}

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
    int length;
    int **ex;
    {
        std::fstream ex_file(prog_args.ex);
        int i = -1;
        int n;
        while (ex_file >> n){
            if (i == -1){
                length = int (n);
                ex = new int*[length];
                for (int j = 0; j < length; ++j)
                    ex[j] = new int[2];
            } else{
                ex[int ((i)/2)][int((i) % 2)] = n;
            }
            ++ i;
        }
    }
    int *res;
    {
        res = new int[length+1];
    }


    // Call correct algorithm
    if (prog_args.algo == "glouton") {
        run(glouton, ex, res, length, prog_args.print_res, prog_args.print_time);
    } else if(prog_args.algo == "progdyn") {
        run(progdyn, ex, res, length, prog_args.print_res, prog_args.print_time);
    } else if(prog_args.algo == "approx") {
        run(approx, ex, res, length, prog_args.print_res, prog_args.print_time);
    }
    for (int j = 0; j < length; ++j){
        delete ex[j];
    }
    delete ex;
    delete res;
    
    return 0;
}
