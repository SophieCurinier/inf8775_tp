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

using namespace std;
using Algo = const function<void(int**&,int*&, int**&, unsigned int*&, int, int, int, bool)>&;

int INF = INT_MAX;

/** || Usefull Methods || **/
unsigned int getManhattanDistance(int x1, int y1, int x2, int y2){
    return abs(x2-x1)+abs(y2-y1);
}

unsigned int getNearestDistance(int**& res, unsigned int*& enclosureSize, int u, int v){
    int xu = res[u][0];
    int xv = res[v][0];
    int yu = res[u][1];
    int yv = res[v][1];
    unsigned int distance = abs(xu - xv);
    bool isUTop = (yu+enclosureSize[u] > yv+enclosureSize[v]);
    if(isUTop){
        if (yu > yv+enclosureSize[v]){
            distance += abs(yu-yv+(int)enclosureSize[v]);
        }
    } else {
        if (yv > yu+enclosureSize[u]){
            distance += abs(yv-yu+(int)enclosureSize[u]);
        }
    }
    return distance;
}

unsigned int getNearestDistanceSolution(unsigned int*& enclosureSize, int u, int v, int xu, int yu, int xv, int yv){
    unsigned int distance = abs(xu - xv);
    bool isUTop = (yu+enclosureSize[u] > yv+enclosureSize[v]);
    if(isUTop){
        if (yu > yv+enclosureSize[v]){
            distance += abs(yu-yv+(int)enclosureSize[v]);
        }
    } else {
        if (yv > yu+enclosureSize[u]){
            distance += abs(yv-yu+(int)enclosureSize[u]);
        }
    }
    return distance;
}

// Print result
void printEnclosure(int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure){
    for (int enclosure=0; enclosure<numberOfEnclosure; enclosure++){
        for (int i=0; i<enclosureSize[enclosure]; i++){
            std::cout << res[enclosure][2*i] << " " << res[enclosure][2*i+1] << " ";
        }
        std::cout << "\n";
    }
}

// Print result in plan
void printPlan(int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure){
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
                std::cout << " -" << " ";
            } else {
                if (place[x][y] < 10){
                    std::cout << " " << place[x][y] << " ";
                } else {
                    std::cout << place[x][y] << " ";
                }   
            }

            if (x == place.size()-1){
                std::cout << "\n";
            }
        }
    }
}

void run(Algo algo, int**& weightedMatrix, int*& subSet, int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, bool printP) {
    algo(weightedMatrix, subSet, res, enclosureSize, numberOfEnclosure, numberOfSubset, maxDistanceSubSet, printP);
}

/* || Calculation of cost ||*/
// General cost
int calculateCost(int**& res, unsigned int*& enclosureSize, int**& weightedMatrix, int numberOfEnclosure, int numberOfSubset, int*& subSet, int maxDistanceSubSet){
    int cost = 0 ;
    int distance;
    bool respectSubsetConstrain = true;
    for (int u=0; u<numberOfEnclosure; u++){
        // For each other enclosure find most nearest enclosure
        for (int v=0; v<numberOfEnclosure; v++){
            if (u != v){
                distance = getNearestDistance(res,enclosureSize,u,v);
                cost -= distance * weightedMatrix[u][v];
            }
        }
    }

    for (int u=0; u<numberOfSubset; u++){
        for (int v=0; v<numberOfSubset; v++){
            if (u != v){
                distance = getNearestDistance(res,enclosureSize,subSet[u],subSet[v]);
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
void placeRectangle(int u,int**& res,unsigned int uSize, int x0, int y0){
    for (int index=0; index<uSize; index++){
        res[u][2*index] = x0;
        res[u][2*index+1] = y0+index;
    }
}

int findNextEnclosure(int**& res, int x, vector<bool>& isPlaced, unsigned int*& enclosureSize, int numberOfEnclosure, int**& weightedMatrix){
    int bestRectIndex = 0;
    int bestScore = INF;
    int score;
    int distance;
    for (unsigned int i=0; i<numberOfEnclosure; i++){
        if (!isPlaced[i]){
            score = 0;
            for (unsigned int j=0; j<numberOfEnclosure; j++){
                if (isPlaced[j]){
                    distance = abs(res[j][0] - x);
                    score += distance*(weightedMatrix[j][i]+weightedMatrix[i][j]);
                }
            }
            if (score < bestScore){
                bestScore = score;
                bestRectIndex = i;
            }
        }
    }

    return bestRectIndex;
}

void glouton(int*& subSet, int**& res, unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int**& weightedMatrix, int maxDistanceSubset){
    vector<int> isPlaced ;
    vector<float> sumWeight = vector<float>(numberOfEnclosure,0);
    int distanceInSubset = 0;
    bool isInSubset = false;

    for (int i=0; i<numberOfEnclosure; i++){
        for (int j=0; j<numberOfEnclosure; j++){
            sumWeight[i] += weightedMatrix[i][j];
        }
    }

    vector<int> sortedEnclosure = vector<int>(numberOfEnclosure);
    for (int i=0; i<numberOfEnclosure; i++){
        sortedEnclosure[i] = i;
    }
    sort(sortedEnclosure.begin(), sortedEnclosure.end(),
         [&sumWeight](int a, int b) { return sumWeight[a] > sumWeight[b]; });
    int x = 0;
    //Placer les rectangles un par un en choisissant la position optimale
    for (int i : sortedEnclosure) {
        placeRectangle(i,res,enclosureSize[i],x,0);
        x += 1;
    }
    
}

void initialization(int*& subSet, int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset){
    // 1 : Placer les rectangles du SubSet les uns a coté des autres avec un cube de cote en commun, alternant un vers le haut, un vers le bas
    int x = 0;
    int y = 0;
    vector<bool> isPlaced = vector<bool>(numberOfEnclosure, false);
    for (int i=0; i<numberOfSubset; i++)
    {
        int enclosure = subSet[i];
        if (x%2 == 0){
            y = 0;
        } else {
            y = -enclosureSize[enclosure]+1;
        }
        placeRectangle(enclosure, res, enclosureSize[enclosure], x, y);
        isPlaced[enclosure] = true; 
        x += 1;
    }

    for (int i=0; i<numberOfEnclosure; i++)
    {
        int enclosure = i;
        if (!isPlaced[enclosure]){
            if (x%2 == 0){
                y = 0;
            } else {
                y = -enclosureSize[enclosure]+1;
            }
            placeRectangle(enclosure, res, enclosureSize[enclosure], x, y);
            x += 1;
        }
    }
}

int calculateCostNeighbor(int**& weightedMatrix, int*& subSet, vector<pair<int,int>>& solution, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, unsigned int*& enclosureSize){
    int cost = 0 ;
    int distance;
    bool respectSubsetConstrain = true;
    for (int u=0; u<numberOfEnclosure; u++){
        // For each other enclosure find most nearest enclosure
        for (int v=0; v<numberOfEnclosure; v++){
            if (u != v){
                distance = getNearestDistanceSolution(enclosureSize,u,v,solution[u].first,solution[u].second,solution[v].first,solution[v].second);
                cost -= distance * weightedMatrix[u][v];
            }
        }
    }

    for (int u=0; u<numberOfSubset; u++){
        for (int v=0; v<numberOfSubset; v++){
            if (u != v){
                distance = getNearestDistanceSolution(enclosureSize,u,v,solution[subSet[u]].first,solution[subSet[u]].second,solution[subSet[v]].first,solution[subSet[v]].second);
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

bool isPossible(int enclosure, vector<pair<int,int>> solution, int numberOfEnclosure, unsigned int*& enclosureSize, int newX, int newY){
    bool isPossibleVar = true;
    for (int otherEnclosure = 0; otherEnclosure<numberOfEnclosure; otherEnclosure++){
        if (otherEnclosure != enclosure){
            bool isXAlign = (solution[otherEnclosure].first == newX);
            if (isXAlign){
                if ((newY <= solution[otherEnclosure].second + enclosureSize[otherEnclosure]) && (solution[otherEnclosure].second < newY + 1)){
                    isPossibleVar = false;
                    break;  // Sort de la boucle car on a trouvé une collision
                }
                else if ((solution[otherEnclosure].second <= newY + enclosureSize[enclosure]) && (newY < newY + 1)){
                    isPossibleVar = false;
                    break;  // Sort de la boucle car on a trouvé une collision
                }
            }
        }
    }

    return isPossibleVar;
}

void findNeighborhood(int enclosure, vector<pair<int,int>>& solution, int numberOfEnclosure, vector<vector<pair<int,int>>>&  neighborList, int maxDistance, unsigned int*& enclosureSize){
    for (int x=-maxDistance; x<maxDistance; x++){
        for (int y=-maxDistance; y<maxDistance; y++){
            bool isPossibleVar = isPossible(enclosure, solution, numberOfEnclosure, enclosureSize, solution[enclosure].first+x, solution[enclosure].second+y);
            if ((isPossibleVar) && (abs(x)+abs(y) <= maxDistance)){
                vector<pair<int,int>> neighbor = solution;
                neighbor[enclosure] = pair<int,int>(solution[enclosure].first+x, solution[enclosure].second+y);
                neighborList.push_back(neighbor);
            }
        }
    }
}

void tabouSearch(int**& weightedMatrix, int*& subSet, int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, int maxDistance, int tabouTenure, int maxIteration, bool printP){
    vector<vector<pair<int,int>>> tabouList = vector<vector<pair<int,int>>>(1,vector<pair<int,int>>(numberOfEnclosure));
    tabouList.clear();
    
    vector<pair<int,int>> bestSolution = vector<pair<int,int>>(numberOfEnclosure);
    for (int i=0; i<numberOfEnclosure; i++){
        bestSolution[i] = pair<int,int>(res[i][0], res[i][1]);
    }
    int bestScore = calculateCostNeighbor(weightedMatrix,subSet,bestSolution,numberOfEnclosure,numberOfSubset,maxDistanceSubSet,enclosureSize);

    vector<pair<int,int>> bestNeighbor = vector<pair<int,int>>(numberOfEnclosure);
    bestNeighbor = bestSolution;
    bool isFind ;
    int bestNeighborScore ;

    bool isInTabouList;
    bool isSame;

    int neighborScore;
    vector<pair<int,int>> neighborSolution;
    pair<int,int> tabouICoor;
    vector<pair<int,int>> tabouSolution;
    vector<vector<pair<int,int>>>  neighborList;

    for (int i=0; i<maxIteration; i++){
        
        isFind = false;
        bestNeighborScore = -INF;

        for (int enclosure = 0; enclosure<numberOfEnclosure; enclosure++){
            findNeighborhood(enclosure,bestNeighbor,numberOfEnclosure,neighborList,maxDistance,enclosureSize);
            for (int neighbor = 0; neighbor<neighborList.size(); neighbor++){
                // Find if neighbor is not in tabo list
                isInTabouList = false;
                for (int tabou=0; tabou < tabouList.size(); tabou++){
                    isSame = true;
                    for (int i=0; i<numberOfEnclosure; i++){
                        tabouICoor = tabouList[tabou][i];
                        if ((tabouICoor.first != neighborList[neighbor][i].first) || (tabouICoor.second != neighborList[neighbor][i].second)){
                            isSame = false;
                        }
                    }
                    if (!isSame){
                        isInTabouList = true;
                    }
                }

                if (!isInTabouList){
                   neighborSolution = neighborList[neighbor];
                   neighborScore = calculateCostNeighbor(weightedMatrix,subSet,neighborSolution,numberOfEnclosure,numberOfSubset,maxDistanceSubSet,enclosureSize);
                   if (neighborScore > bestNeighborScore){
                        bestNeighbor = neighborSolution;
                        bestNeighborScore = neighborScore;
                        isFind = true;
                   }
                }
            }
        }
        if (!isFind && tabouList.size()>0){
            tabouSolution = tabouList[rand()%tabouList.size()];
            bestNeighbor = tabouSolution;
        }

        if (bestNeighborScore > bestScore){
            bestSolution = bestNeighbor;
            bestScore = bestNeighborScore;

            for (int enclosure=0; enclosure<numberOfEnclosure; enclosure++){
                placeRectangle(enclosure,res,enclosureSize[enclosure],bestSolution[enclosure].first,bestSolution[enclosure].second);
            }
            std::cout << bestScore << std::endl;
            if (printP){
                printEnclosure(res, enclosureSize,numberOfEnclosure);
            }
        }

        tabouList.push_back(bestNeighbor);
        if (tabouList.size() > tabouTenure){
            tabouList.erase(tabouList.begin());
        }

    }
}

/* || Algorithme || */
void algo(int**& weightedMatrix, int*& subSet, int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, bool printP){
    int cost;
    
    // Initialization with glouton algorithme
    // initialization(subSet,res,enclosureSize,numberOfEnclosure,numberOfSubset);
    glouton(subSet, res, enclosureSize, numberOfEnclosure, numberOfSubset, weightedMatrix, maxDistanceSubSet);
    cost = calculateCost(res,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset, subSet, maxDistanceSubSet);
    std::cout << cost << std::endl;
    if (printP){
        printEnclosure(res,enclosureSize,numberOfEnclosure);
    }
    
    // Algorithme tabou
    int maxIteration = 1000000;
    tabouSearch(weightedMatrix,subSet,res,enclosureSize,numberOfEnclosure,numberOfSubset,maxDistanceSubSet,10,50,maxIteration,printP);
    
    /* == A enlever ==*/
    printPlan(res, enclosureSize, numberOfEnclosure);
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
    int numberOfEnclosure = 0;
    int numberOfSubSet = 0;
    int maxDistanceSubSet = 0;
    int **weightedMatrix ;
    int *subSet;
    int **res;
    unsigned int *enclosureSize ;
    {
        std::fstream ex_file(prog_args.ex);
        int p = -3;
        int n;
        while (ex_file >> n){
            if (p == -3){
                numberOfEnclosure = int (n);
                res = new int*[numberOfEnclosure];
                weightedMatrix = new int*[numberOfEnclosure];
                enclosureSize = new unsigned int[numberOfEnclosure];
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
    run(algo, weightedMatrix, subSet, res, enclosureSize, numberOfEnclosure, numberOfSubSet, maxDistanceSubSet,prog_args.print_res);

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