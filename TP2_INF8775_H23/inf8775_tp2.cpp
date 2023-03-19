#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;
using Algo = const function<void(int**,int*, int)>&;

const int INF = 1e9;

// Useful method
// Return initial city
int initialCity(){
    return 0;
}

// Calcul eucliedean distance between 2 points rounded to the nearest integer.
int euclideanDistance(int x1, int y1, int x2, int y2){
    return lround(sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
}

// Fill distanceMatrix with euclidean distances between cities.
void initializeDistancMatrix(int** matrix, int** distanceMatrix, int length){
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
}


/* || Method for glouton || */
void glouton(int** matrix, int* res, int length) {
    int totalDist = 0;
    // Initialized all city as unvisited
    bool * visitedCites = new bool[length];

    // Select an arbitrary city
    int currentCity = initialCity();
    visitedCites[currentCity] = true;
    int nbUnvisitedCities = length - 1;
    res[0] = currentCity;

    // Calcul and intialize matrix distance
    int **distanceMatrix = new int*[length];
    for (int i = 0; i<length; i++){
        distanceMatrix[i] = new int[length];
    }
    initializeDistancMatrix(matrix, distanceMatrix, length);

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
        totalDist += distanceWithNearest;
        visitedCites[nearestUnvisitedCity] = true;
        res[length-nbUnvisitedCities] = nearestUnvisitedCity;
        nbUnvisitedCities -= 1;
        currentCity = nearestUnvisitedCity;
    }
    totalDist += distanceMatrix[0][res[length-1]];

    // Delete before sending
    std::cout << totalDist << std::endl;

    res[length] = res[0];

    // Desallocate array
    delete visitedCites;
    for (int i=0; i<length; i++){
        delete distanceMatrix[i];
    }
    delete distanceMatrix;
}


/* || Methods for dynamic programmation || */
// Method to fill array for dynamic programation using the binary mask to represent the set of cities S
int tsp(int** distanceMatrix, int** memo, int currentCity, int length, int mask){
    // If mask is equal 2^length, so S is empty
    // Hovewer, if S is empty, then all cities have been visited. 
    if (mask == (1<<length)-1){ 
        return distanceMatrix[currentCity][0];
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
        distance = min(distance, distanceMatrix[currentCity][nextCity] + tsp(distanceMatrix, memo, nextCity,length, mask|(1<<nextCity)));
    }
    memo[currentCity][mask] = distance;
    return distance;
}

// Get TSP path using array for dynamic programmation
void getTspPath(int** distanceMatrix, int** memo, int length, int* path, int mask, int pos, int len) {
    // If S is empty
    if (mask == (1 << length) - 1) {
        path[length-1-len] = 0;
        return;
    }

    for (int i = 0; i < length; i++) {
        if ((mask & (1 << i)) == 0) {
            int next_dist = distanceMatrix[pos][i] + tsp(distanceMatrix, memo, i, length, mask | (1 << i));
            if (next_dist == memo[pos][mask]) {
                path[length-len-1] = i;
                getTspPath(distanceMatrix, memo, length, path, mask | (1 << i), i, len + 1);
                break;
            }
        }
    }
}

void progdyn(int** matrix, int* res, int length) {
    // Calcul and intialize matrix distance
    int** distanceMatrix = new int*[length];
    for (int i=0; i<length; i++){
        distanceMatrix[i] = new int[length];
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
    int firstCity = initialCity();
    res[0] = firstCity;
    res[length] = firstCity;

    // ---- Delete before finish --- //
    int distanceTotal = tsp(distanceMatrix, memo, firstCity, length, 1);
    std::cout << "DistanceTotal " << distanceTotal << std::endl;
    // ---- Delete before finish --- //

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


/* || Methods for dynamic programmation || */
// Implementation of Prim Algorith
void prim(int** distanceMatirx, int length, vector<pair<int,int>>& T ){
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
            if (distanceMatirx[k][j] < distMin[j]){
                distMin[j] = distanceMatirx[k][j];
                neighboor[j] = k;
            }
        }
    }

    // Desallocate array
    delete neighboor;
    delete distMin;
}

// Get TSP path using preorder traversal of Prim's tree
void preorderTraversal(int u, vector<pair<int, int>>& T, bool* visited, int* res, int& index) {
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
void approx(int** matrix, int* res, int length) {
    // Calcul and intialize matrix distance
    int** distanceMatrix = new int*[length];
    for (int i=0; i<length; i++){
        distanceMatrix[i] = new int[length];
    }
    initializeDistancMatrix(matrix, distanceMatrix, length);

    // Initialize citiesvisited array
    bool* visitedCites = new bool[length];
    visitedCites[0] = true;

    // Select an arbitrary city
    int firstCity = initialCity();

    // Tree
    vector<pair<int,int>> tree;
    prim(distanceMatrix, length, tree);

    // Fill res with path obtained by preorder traversal of Prim's tree
    int index = 0;
    res[0] = firstCity;
    preorderTraversal(firstCity, tree, visitedCites, res, index);
    res[length] = firstCity;

    // A enlever avant remise
    int distanceTotal = 0;
    for (int i=0; i<length; i++){
        distanceTotal += distanceMatrix[i][i+1];
    }
    std::cout << distanceTotal << std::endl;
    //
    
    // Desallocate arrays
    for (int i=0; i<length; i++){
        delete distanceMatrix[i];
    }
    delete distanceMatrix;res[length] = firstCity;
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
