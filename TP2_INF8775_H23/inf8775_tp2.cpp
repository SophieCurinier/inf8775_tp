#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

using namespace std;
using Algo = const function<void(int**&,int*&, int)>&;

const int INF = 1e9;

// Useful method
// Return initial city
int initialCity(int nbCities){
    return std::rand()%nbCities;
}

// Calcul eucliedean distance between 2 points rounded to the nearest integer.
int euclideanDistance(int x1, int y1, int x2, int y2){
    return lround(sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
}

// Fill distanceMatrix with euclidean distances between cities.
void initializeDistancMatrix(int**& matrix, int**& distanceMatrix, int length){
    int distanceBetweenIAndJ ;
    for (int i=0; i<length; i++){
        for (int j=i; j<length; j++){
            if (i == j){
                distanceMatrix[i][j] == 0;
            } else {
                distanceBetweenIAndJ = euclideanDistance(matrix[i][0],matrix[i][1],matrix[j][0],matrix[j][1]);
                distanceMatrix[i][j-i] = distanceBetweenIAndJ;
            } 
        }   
    }
}

// Get distance between i and j in distanceMatrix
int getDistanceBetweenIAndJ(int**& matrix, int i, int j)
{
    int distanceBetweenIAndJ = 0;
    if (i < j){
        distanceBetweenIAndJ = matrix[i][j-i];
    } else if (i > j)
    {
        distanceBetweenIAndJ = matrix[j][i-j];   
    }
    return distanceBetweenIAndJ;
}


/* || Method for glouton || */
void glouton(int**& matrix, int*& res, int length) {
    int totalDist = 0;

    // Initialized all city as unvisited
    bool * visitedCites = new bool[length];

    // Select an arbitrary city
    int currentCity = initialCity(length);
    visitedCites[currentCity] = true;
    int nbUnvisitedCities = length - 1;
    res[0] = currentCity;

    // Calcul and intialize matrix distance
    int **distanceMatrix = new int*[length];
    for (int i = 0; i<length; i++){
        distanceMatrix[i] = new int[length-i];
    }
    initializeDistancMatrix(matrix, distanceMatrix, length);

    // Find out the shortest edge connecting the current city and an unvisited city
    int nearestUnvisitedCity ;
    int distanceWithNearest ;
    while (nbUnvisitedCities > 0){
        nearestUnvisitedCity = -1;
        for (int i=0; i<length; i++){
            if (!visitedCites[i]){
                // int distanceWithI = distanceMatrix[currentCity][i];
                int distanceWithI = getDistanceBetweenIAndJ(distanceMatrix, i, currentCity);
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
        totalDist += distanceWithNearest;
        visitedCites[nearestUnvisitedCity] = true;
        res[length-nbUnvisitedCities] = nearestUnvisitedCity;
        nbUnvisitedCities -= 1;
        currentCity = nearestUnvisitedCity;
    }
    totalDist += getDistanceBetweenIAndJ(distanceMatrix, 0, res[length-1]);

    res[length] = res[0];

    totalDist += getDistanceBetweenIAndJ(distanceMatrix, res[length-1],res[length]);

    // Desallocate array
    delete visitedCites;
    for (int i=0; i<length; i++){
        delete distanceMatrix[i];
    }
    delete distanceMatrix;
}


/* || Methods for dynamic programmation || */
// Method to fill array for dynamic programation using the binary mask to represent the set of cities S
int tsp(int**& distanceMatrix, int**& memo, int currentCity, int length, int mask){
    // If mask is equal 2^length, so S is empty
    // Hovewer, if S is empty, then all cities have been visited. 
    if (mask == (1<<length)-1){
        return getDistanceBetweenIAndJ(distanceMatrix, 0, currentCity);
    }

    // If value has already is calculate by TSP, value is return
    if (memo[currentCity][mask] != -1){
        return memo[currentCity][mask];
    }
    int distance = INF;
    for (int nextCity=0; nextCity<length; nextCity++){
        // If nextCity has already visited, we passed to nextCity (because nextCity is not in S)
        if (mask&(1<<nextCity)){
            continue;
        }
        // Else we calculate tsp value
        distance = min(distance,getDistanceBetweenIAndJ(distanceMatrix, currentCity, nextCity) + tsp(distanceMatrix, memo, nextCity,length, mask|(1<<nextCity)));
    }
    memo[currentCity][mask] = distance;
    return distance;
}

// Get TSP path using array for dynamic programmation
void getTspPath(int**& distanceMatrix, int**& memo, int length, int*& path, int mask, int pos, int len) {
    // If S is empty
    if (mask == (1 << length) - 1) {
        path[length-1-len] = 0;
        return;
    }

    for (int i = 0; i < length; i++) {
        if ((mask & (1 << i)) == 0) {
            int next_dist = getDistanceBetweenIAndJ(distanceMatrix, pos, i) + tsp(distanceMatrix, memo, i, length, mask | (1 << i));
            if (next_dist == memo[pos][mask]) {
                path[length-len-1] = i;
                getTspPath(distanceMatrix, memo, length, path, mask | (1 << i), i, len + 1);
                break;
            }
        }
    }
}

void progdyn(int**& matrix, int*& res, int length) {
    // Calcul and intialize matrix distance
    int** distanceMatrix = new int*[length];
    for (int i=0; i<length; i++){
        // distanceMatrix[i] = new int[length-i];
        distanceMatrix[i] = new int[length-i];
    }
    initializeDistancMatrix(matrix, distanceMatrix, length);

    // Initialize memo array
    int** memo = new int*[length];
    for (int i=0; i<length; i++){
        memo[i] = new int[1<<length];
        for (int j=0; j<1<<length;j++){
            memo[i][j] = -1;
        }
    }

    // Select an arbitrary city
    int firstCity = 0;
    res[0] = firstCity;
    res[length] = firstCity;

    int distanceTotal = tsp(distanceMatrix, memo, firstCity, length, 1);

    // Fill res with path
    getTspPath(distanceMatrix, memo, length, res, 1, 0, 0);

    // Desallocate arrays
    for (int i=0; i<length; i++){
        delete distanceMatrix[i];
        delete memo[i];
    }
    delete memo;
    delete distanceMatrix;

}

/* || Methods for approximation || */
// Implementation of Prim Algorith
void prim(int**& distanceMatirx, int length, vector<pair<int,int>>& T ){
    // Intialize neighboor and distMin array
    int* neighboor = new int[length];
    int* distMin = new int[length];
    int k = 0;
    for (int i=1; i<length; i++){
        neighboor[i] = -1;
        distMin[i] = INF;
    }

    pair<int,int> valueAndNeighboor;
    distMin[0] = 0;

    for (int i=0; i<length-1; i ++){
        int min = INF;
        for (int j=0; j<length; j++){
            if ((0 <= distMin[j]) && (distMin[j] < min)){
                min = distMin[j];
                k = j;
            }
        }
        valueAndNeighboor.first = k;
        valueAndNeighboor.second = neighboor[k];
        T.push_back(valueAndNeighboor);
        distMin[k] = -1;
        for (int j=0; j<length; j++){
            if (getDistanceBetweenIAndJ(distanceMatirx, k, j) < distMin[j]){
                distMin[j] = getDistanceBetweenIAndJ(distanceMatirx, k,j);
                neighboor[j] = k;
            }
        }
    }

    // Desallocate array
    delete neighboor;
    delete distMin;
}

// Get TSP path using preorder traversal of Prim's tree
void preorderTraversal(int u, vector<pair<int, int>>& T, bool*& visited, int*& res, int index) {
    visited[u] = true;
    res[index++] = u;

    for (pair<int, int> edge : T) {
        int v = (edge.first == u ? edge.second : edge.first);
        if (!visited[v]) {
            preorderTraversal(v, T, visited, res, index);
        }
    }
}

// Using Prim Algo
void approx(int**& matrix, int*& res, int length) {
    // Calcul and intialize matrix distance
    int** distanceMatrix = new int*[length];
    for (int i=0; i<length; i++){
        distanceMatrix[i] = new int[length-i];
    }
    initializeDistancMatrix(matrix, distanceMatrix, length);

    // Initialize citiesvisited array
    bool* visitedCites = new bool[length];
    visitedCites[0] = true;

    // Select an arbitrary city
    int firstCity = initialCity(length);

    // Tree
    vector<pair<int,int>> tree;
    prim(distanceMatrix, length, tree);

    // Fill res with path obtained by preorder traversal of Prim's tree
    int index = 0;
    preorderTraversal(firstCity, tree, visitedCites, res, index);
    
    // Desallocate arrays
    for (int i=0; i<length; i++){
        delete distanceMatrix[i];
    }
    delete distanceMatrix;
}

/* || Print path methods || */
int positiveModulo(int a, int b){
    return ((a  % b + b) % b);
}

void printPath(int*& res, int length){
    bool finded = false;
    int indix = 0;

    while (res[indix] != 0){
        indix += 1;
    }

    std::cout << 0 << "\n";
    if (res[indix+1] < res[positiveModulo(indix-1, length)]){
        for (int i=0; i<length; i++){
            std::cout << res[positiveModulo(indix+1+i, length)] << "\n";
        }
    } else {
        for (int i=0; i<length; i++){
            std::cout << res[positiveModulo(indix-1-i, length)] << "\n";
        }
    }
}

void run(Algo algo, int**& matrix, int*& res, int length, bool print_res, bool print_time) {
    using namespace chrono;
    auto start = steady_clock::now();
    algo(matrix, res, length);
    auto end = steady_clock::now();

    if (print_time) {
        duration<double> s = end-start;
        std::cout << std::fixed << s.count() << std::endl;
    }
    if (print_res) {
        printPath(res, length);
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
    int length = 0;

    // Initialize resultat and ex array
    int **ex;
    int *res;
    {
        std::fstream ex_file(prog_args.ex);
        int i = -1;
        int n;
        while (ex_file >> n){
            if (i == -1){
                length = int (n);
                ex = new int*[length];
                res = new int[length+1];
                for (int j = 0; j < length; ++j)
                    ex[j] = new int[2];
            } else{
                ex[int ((i)/2)][int((i) % 2)] = n;
            }
            ++ i;
        }
    }

    // Call correct algorithm
    if (prog_args.algo == "glouton") {
        run(glouton, ex, res, length, prog_args.print_res, prog_args.print_time);
    } else if(prog_args.algo == "progdyn") {
        run(progdyn, ex, res, length, prog_args.print_res, prog_args.print_time);
    } else if(prog_args.algo == "approx") {
        run(approx, ex, res, length, prog_args.print_res, prog_args.print_time);
    }

    // Desallocate array
    if (length > 0){
        for (int j = 0; j < length; ++j){
            delete ex[j];
        }
        delete ex;
        delete res;
    }

    return 0;
}
