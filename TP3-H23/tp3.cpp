#include <fstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <limits.h>
#include <math.h>
#include <vector>
#include <numeric>
#include <queue>

using namespace std;
using Algo = const function<void(int**&,int*&, int**&, int*&, int, int, int)>&;

int INF = INT_MAX;

/** || Usefull Methods || **/
int getManhattanDistance(int x1, int y1, int x2, int y2){
    return abs(x2-x1)+abs(y2-y1);
}

int getNearestDistance(int**& res, int*& closureSize, int u, int v){
    int minDistance = INF;
    for (int i=0; i<closureSize[u]; i++){
        for (int j=0; j<closureSize[v]; j++){
            minDistance = min(minDistance, getManhattanDistance(res[u][i*2],res[u][i*2+1],res[v][j*2],res[v][j*2+1]));
        }
    }
    return minDistance;
}

/* || Print method || */
// Print result
void printEnclosure(int**& res,  int*& enclosureSize, int numberOfEnclosure){
    for (int enclosure=0; enclosure<numberOfEnclosure; enclosure++){
        for (int i=0; i<enclosureSize[enclosure]; i++){
            std::cout << res[enclosure][2*i] << " " << res[enclosure][2*i+1] << " ";
        }
        std::cout << "\n";
    }
}

// Print result in plan
void printPlan(int**& res,  int*& enclosureSize, int numberOfEnclosure){
    int minX = INF;
    int minY = INF;
    int maxX = -INF;
    int maxY = -INF;

    for (int index=0; index<numberOfEnclosure; index++){
        for (int i=0; i<enclosureSize[index]; i++){
            if (res[index][2*i] > maxX){
                maxX = res[index][2*i];
            } else if (res[index][2*i] < minX){
                minX = res[index][2*i];
            }
            if (res[index][2*i+1] > maxY){
                maxY = res[index][2*i+1];
            } else if (res[index][2*i+1] < minY){
                minY = res[index][2*i+1];
            }
        }
    }

    vector<vector<int>> place(maxX-minX+1,vector<int>(maxY-minY+1,-1));
    for (int index=0; index<numberOfEnclosure; index++){
        for (int i=0; i<enclosureSize[index]; i++){
            place[res[index][2*i]-minX][res[index][2*i+1]-minY] = index;
        }
    }

    for (int y=place[0].size()-1; y>=0 ; y--){
        for (int x=0; x<place.size(); x++){
            if (place[x][y] == -1){
                std::cout << "-" << " ";
            } else {
                std::cout << place[x][y] << " ";
            }

            if (x == place.size()-1){
                std::cout << "\n";
            }
        }
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

/* || Calculation of cost ||*/
// General cost
int calculateCost(int**& res, int*& closureSize, int**& weightedMatrix, int numberOfEnclosure, int numberOfSubset, int*& subSet, int maxDistanceSubSet){
    int cost = 0 ;
    int distance;
    bool respectSubsetConstrain = true;
    for (int u=0; u<numberOfEnclosure; u++){
        // For each other enclosure find most nearest enclosure
        for (int v=0; v<numberOfEnclosure; v++){
            if (u != v){
                distance = getNearestDistance(res,closureSize,u,v);
                cost -= distance * weightedMatrix[u][v];
            }
        }
    }

    for (int u=0; u<numberOfSubset; u++){
        for (int v=0; v<numberOfSubset; v++){
            if (u != v){
                distance = getNearestDistance(res,closureSize,subSet[u],subSet[v]);
                if ((distance < maxDistanceSubSet) && respectSubsetConstrain){
                    respectSubsetConstrain = false;
                }
            }
        }
    }

    if (respectSubsetConstrain){
        cost += numberOfSubset*numberOfSubset;
    }
    return cost;
}

// Cost if rectangle
int calculateRectangleCost(vector<int> rectangleXCoor, int*& closureSize, int**& weightedMatrix, int numberOfEnclosure, int numberOfSubset, int*& subSet, int maxDistanceSubSet){
    int cost = 0 ;
    int distance;
    bool respectSubsetConstrain = true;
    for (int u=0; u<numberOfEnclosure; u++){
        // For each other enclosure find most nearest enclosure
        for (int v=0; v<numberOfEnclosure; v++){
            if (u != v){
                distance = abs(rectangleXCoor[u]-rectangleXCoor[v]);
                cost -= distance * weightedMatrix[u][v];
            }
        }
    }

    for (int u=0; u<numberOfSubset; u++){
        for (int v=0; v<numberOfSubset; v++){
            if (u != v){
                distance = abs(rectangleXCoor[u]-rectangleXCoor[v]);
                if ((distance < maxDistanceSubSet) && respectSubsetConstrain){
                    respectSubsetConstrain = false;
                }
            }
        }
    }

    if (respectSubsetConstrain){
        cost += numberOfSubset*numberOfSubset;
    }
    return cost;
}

/* || Intialisation || */
void placeRectangle(int u,int**& res,int uSize, int x0, int y0){
    for (int index=0; index<uSize; index++){
        res[u][2*index] = x0;
        res[u][2*index+1] = y0+index;
    }
}

void initialize(int**& res, int*& enclosureSize, int numberOfEnclosure){
    for (int i=0; i<numberOfEnclosure; i++){
        placeRectangle(i, res, enclosureSize[i], i, 0);
    }
}

/* || Swape rectange just after initialization || */
void swapeTwoRectangle(int**& res, int*& enclosureSize, int u, int v){
    int x0u = res[u][0];
    int x0v = res[u][0];

    for (int i=0; i<enclosureSize[u]; i++){
        res[u][2*i] = x0v;
    }
    for (int j=0; j<enclosureSize[v]; j++){
        res[v][2*j]= x0u;
    }
}


void findBestPlace(int**& res, int*& enclosureSize, int**& weightedMatrix, int numberOfEnclosure, int numberOfSubset, int*& subSet, int maxDistanceSubSet){
    int cost = 0;
    int minCost ;
    vector<int> minCombination ;

    vector<int> v(numberOfEnclosure);
    for (int i=0; i<numberOfEnclosure; i++){
        v[i] = res[i][0];
    }
    minCost = calculateRectangleCost(v,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset, subSet, maxDistanceSubSet);

    vector<int> indices(numberOfEnclosure);
    std::iota(indices.begin(), indices.end(), 0);

    do {
        std::vector<int> current(numberOfEnclosure);
        for (int i = 0; i < numberOfEnclosure; i++) {
            current[i] = v[indices[i]];
        }
        int current_cost = calculateRectangleCost(current,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset,subSet,maxDistanceSubSet);
        if (current_cost > minCost) {
            minCost = current_cost;
            minCombination = current;
        }
    } while (std::next_permutation(indices.begin(), indices.end()));

    // Change
    for(int i=0; i<numberOfEnclosure; i++){
        placeRectangle(i,res,enclosureSize[i],minCombination[i],0);
    }
}

void glouton(int**& res, int**& weightedMatrix, int*& enclosureSize, int numberOfEnclosure, int cost, int numberOfSubset, int*& subSet, int maxDistanceSubset){
    // int maxSize = - INF;
    // Petit sur grand
    int minSize = INF;
    int minSizeIndex = 0;
    for (int i=0; i<numberOfEnclosure; i++)
    {
        if (minSize>enclosureSize[i])
        {
            minSizeIndex = i;
            minSize = enclosureSize[i];
        }
    }

    // Watch voisin 
    // Met sur le plus petit des deux
    int neighboorIndexAtLeft ;
    int neighboorIndexAtRight ;
    int minSizeX = res[minSizeIndex][0];
    queue<int> atLeft ;
    for (int i=0; i<numberOfEnclosure; i++)
    {
        if ((res[i][0] == (minSizeX-1)) && i != minSizeIndex)
        {
            neighboorIndexAtLeft = i;
        }
        if ((res[i][0] == (minSizeX+1)) && i!= minSizeIndex)
        {
            neighboorIndexAtRight = i;
        }

    }
    // To know left or right side
    int side ;
    // Choice 


    placeRectangle(minSizeIndex, res, enclosureSize[minSizeIndex], res[neighboorIndex][0], enclosureSize[neighboorIndex]);
    //moveEnclosure(res,closureSize, numberOfEnclosure, minSizeIndex,res[neighboorIndex][0],closureSize[neighboorIndex]);
    int atLeftSize = atLeft.size();
    for (int i=0; i<atLeftSize; i++)
    {
        int u = atLeft.front();
        placeRectangle(u, res, enclosureSize[u], res[u][0]-1,0);
        //moveEnclosure(res,closureSize,numberOfEnclosure,u,res[u][0]-1,0);
        atLeft.push(u);
    }
    int newCost = calculateCost(res,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset, subSet,maxDistanceSubset);
    std::cout << "Old Cost " << cost << " New cost " << newCost << std::endl;
    if (newCost < cost){
        for (int i=0; i<atLeftSize;i++)
        {
            int u = atLeft.front();
            placeRectangle(u, res, enclosureSize[u], res[u][0]+1,0);
        }
        placeRectangle(minSizeIndex, res, enclosureSize[minSizeIndex], res[minSizeIndex][0]+1,0);
    }
    printPlan(res, enclosureSize, numberOfEnclosure);
    
}

/* || Algorithme || */
void algo(int**& weightedMatrix, int*& subSet, int**& res,  int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet){
    initialize(res, enclosureSize, numberOfEnclosure);
    // printPlan(res, enclosureSize, numberOfEnclosure);
    int cost = calculateCost(res,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset, subSet, maxDistanceSubSet);
    std::cout << cost << std::endl;
    findBestPlace(res, enclosureSize, weightedMatrix, numberOfEnclosure, numberOfSubset, subSet, maxDistanceSubSet);
    // Put mini rectangle 
    printPlan(res, enclosureSize, numberOfEnclosure);
    cost = calculateCost(res,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset, subSet, maxDistanceSubSet);
    std::cout << cost << std::endl;
    glouton(res,weightedMatrix,enclosureSize,numberOfEnclosure,cost,numberOfSubset,subSet,maxDistanceSubSet);
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
    run(algo, weightedMatrix, subSet, res, enclosureSize, numberOfEnclosure, numberOfSubSet, maxDistanceSubSet, prog_args.print_res, prog_args.print_time);

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