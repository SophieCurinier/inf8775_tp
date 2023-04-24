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
long int calculateCost(int**& res, unsigned int*& enclosureSize, int**& weightedMatrix, int numberOfEnclosure, int numberOfSubset, int*& subSet, int maxDistanceSubSet){
    long int cost = 0 ;
    int distance;
    bool unRespectSubsetConstrain = true;
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
                if (distance > maxDistanceSubSet){
                    unRespectSubsetConstrain = true;
                    break;
                }
            }
        }
    }

    if (!unRespectSubsetConstrain){
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

void verifyIsOkay(int**& res, unsigned int*& enclosureSize, int numberOfEnclosure){
    bool isOnTheSame = false;
    for (int i=0; i<numberOfEnclosure; i++){
        for (int j=0; j<enclosureSize[i]; j++){
            for (int p=0; p<numberOfEnclosure; p++){
                for (int k=0; k<enclosureSize[p]; k++){
                    if (i != p){
                        if ((res[i][2*j] == res[p][2*k]) && (res[i][2*j+1] == res[p][2*k+1])){
                            isOnTheSame = true;
                            std::cout << "break" << std::endl;
                        }
                    }
                }
            }
        }
    }
    std::cout << "Is " << isOnTheSame << std::endl;
}

void glouton(int*& subSet, int**& res, unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int**& weightedMatrix, int maxDistanceSubset){
    vector<int> isPlaced(numberOfEnclosure,-1) ;
    vector<float> sumWeight = vector<float>(numberOfEnclosure,0);
    int distanceInSubset = 0;
    bool isInSubset = false;

    for (int i=0; i<numberOfEnclosure; i++){
        for (int j=0; j<numberOfEnclosure; j++){
            sumWeight[i] += pow(weightedMatrix[i][j],2);
        }
    }


    for (int i=0; i<numberOfSubset; i++){
        isPlaced[subSet[i]] = -2;
    }

    vector<int> sortedEnclosure = vector<int>(numberOfEnclosure);
    for (int i=0; i<numberOfEnclosure; i++){
        sortedEnclosure[i] = i;
    }
    sort(sortedEnclosure.begin(), sortedEnclosure.end(),
         [&sumWeight](int a, int b) { return sumWeight[a] > sumWeight[b]; });
    int xLeft = -1;
    int xRight = 0;

    bool isInitial = true;
    int costLeft = 0;
    int costRight = 0;

    int xLeftSub ;
    int xRightSub;
    int restedSubXToPlace = numberOfSubset;
    unsigned int isReservedPlace = 0;
    int beginningReserdPlace ;
    bool verifySubsetPlacingIsPossible = (maxDistanceSubset > numberOfSubset);
    int y;
    
    //Placer les rectangles un par un en choisissant la position optimale
    for (int i : sortedEnclosure) {
        if (isInitial){
            placeRectangle(i,res,enclosureSize[i],xRight,-(xRight%2 != 0)*(enclosureSize[i]-1));
            isInitial = false;
            if ((isPlaced[i] == -2) && verifySubsetPlacingIsPossible){
                restedSubXToPlace -= 1;
                xLeftSub = xRight;
                xRightSub = xRight;
            }
            isPlaced[i] = xRight;
            xRight += 1;
        } else {
            costLeft = 0;
            costRight = 0;
            for (int j=0; j<numberOfEnclosure; j++){
                if (isPlaced[j] > -1){
                    costLeft += abs(xLeft - isPlaced[j])*(weightedMatrix[i][j]+weightedMatrix[j][i]);
                    costRight += abs(xRight - isPlaced[j])*(weightedMatrix[i][j]+weightedMatrix[j][i]);
                }
            }
            //if ((isPlaced[i] == -2) && verifySubsetPlacingIsPossible){
            if ((isPlaced[i] == -2) && verifySubsetPlacingIsPossible){
                if (isReservedPlace == 0){
                    bool watchAtLeft = abs(xLeft - xRightSub)+restedSubXToPlace <= maxDistanceSubset;
                    bool watchAtRight = abs(xLeftSub - xRight)+restedSubXToPlace <= maxDistanceSubset;
                    if (((watchAtLeft && watchAtRight) && (costLeft < costRight)) || (watchAtLeft && !watchAtRight)){
                        isPlaced[i] = xLeft;
                        placeRectangle(i, res, enclosureSize[i], xLeft, -(xLeft%2 != 0)*(enclosureSize[i]-1));
                        xLeft -= 1;
                        xLeftSub -= 1;
                    } else if (((watchAtLeft && watchAtRight) && (costLeft >= costRight)) || (!watchAtLeft && watchAtRight)) {
                        isPlaced[i] = xRight;
                        placeRectangle(i, res, enclosureSize[i], xRight, (xRight%2 != 0)*(enclosureSize[i]-1));
                        xRight += 1;
                        xRightSub += 1;
                    }
                } else {
                   placeRectangle(i, res, enclosureSize[i], xRightSub, -(xRightSub%2 != 0)*(enclosureSize[i]-1)); 
                   xRightSub += 1;
                }
                restedSubXToPlace -= 1;
                
            } else {
                if (((abs(xLeftSub - xRightSub) + restedSubXToPlace) == maxDistanceSubset) && (isReservedPlace == 0))  {
                costLeft = 0;
                costRight = 0;
                xRight += restedSubXToPlace;
                isReservedPlace = restedSubXToPlace;
                for (int j=0; j<numberOfEnclosure; j++){
                    if (isPlaced[j] > -1){
                        costLeft += abs(xLeft - isPlaced[j])*(weightedMatrix[i][j]+weightedMatrix[j][i]);
                        costRight += abs(xRight - isPlaced[j])*(weightedMatrix[i][j]+weightedMatrix[j][i]);
                    }
                }

                }
                if (costLeft < costRight){
                    isPlaced[i] = xLeft;
                    placeRectangle(i, res, enclosureSize[i], xLeft, -(xLeft%2 != 0)*(enclosureSize[i]-1));
                    xLeft -= 1;
                } else {
                    isPlaced[i] = xRight;
                    placeRectangle(i, res, enclosureSize[i], xRight, -(xRight%2 != 0)*(enclosureSize[i]-1));
                    xRight += 1;
                } 
            }
        }
    }
    
}

long int calculateCostNeighbor(int**& weightedMatrix, int*& subSet, vector<pair<int,int>>& solution, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, unsigned int*& enclosureSize){
    long int cost = 0 ;
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
                if (distance < maxDistanceSubSet){
                    respectSubsetConstrain = false;
                    break;
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
            bool isXAlign = (solution[otherEnclosure].first == newX); //If other enclosure and enclosure are on same column
            if (isXAlign){
                if ((solution[otherEnclosure].second + (int)enclosureSize[otherEnclosure] >= newY) && (solution[otherEnclosure].second <= newY)){
                    isPossibleVar = false;
                    break;  // Sort de la boucle car on a trouvé une collision
                }
                else if ((newY + (int)enclosureSize[enclosure] >= solution[otherEnclosure].second) && (newY <= solution[otherEnclosure].second)){
                    isPossibleVar = false;
                    break;  // Sort de la boucle car on a trouvé une collision
                }
            }
        }
    }

    return isPossibleVar;
}

void clustering(int**& res, vector<int>& tabouCluster, int**& weightedMatrix, unsigned int*& enclosureSize, int numberOfEnclosure, vector<int>& enclosureInCluster){
    int minX = INF;
    int maxX = -INF;

    for (int i=0; i<numberOfEnclosure; i++){
        if (res[i][0] < minX){
            minX = res[i][0];
        }
        if (res[i][0] > maxX){
            maxX = res[i][0];
        }
    }

    int numberOfCluster = numberOfEnclosure/10;

    vector<long int> costInCluster(numberOfCluster);

    for (int i=0; i<numberOfEnclosure; i++){
        for (int j=i; j<numberOfEnclosure; j++){
            if (i != j){
                for (int k=0; k<numberOfCluster; k++){
                    if ((res[i][0] >= minX + k*abs(maxX-minX)/numberOfCluster) && (res[i][0] < minX + (k+1)*abs(maxX-minX)/numberOfCluster)){
                        costInCluster[k] -= getNearestDistance(res, enclosureSize, i, j)*(weightedMatrix[i][j]+weightedMatrix[j][i]);
                    }
                }
            }
        }
    }

    long int minInCluster = INF;
    int intrestingCluster ;

    for (int k=0; k<numberOfCluster; k++){
        if ((costInCluster[k] < minInCluster) && (tabouCluster[k] == -1)){
            minInCluster = costInCluster[k];
            intrestingCluster = k;
        } 
    }

    enclosureInCluster.clear();
    for (int i=0; i<numberOfEnclosure; i++){
        if ((res[i][0] >= minX + intrestingCluster*abs(maxX-minX)/numberOfCluster) && (res[i][0] < minX + (intrestingCluster+1)*abs(maxX-minX)/numberOfCluster)){
            enclosureInCluster.push_back(i);
        }
    }
    
    for (int k=0; k<numberOfCluster; k++){
        if (k == intrestingCluster){
            tabouCluster[k] = 0;
        } else {
            if (tabouCluster[k] > -1){
                tabouCluster[k] += 1;
                if (tabouCluster[k] == (int)(numberOfCluster/4)){
                    tabouCluster[k] = -1;
                }
            }
        }
    }
}

void findNeighborhood(int**& res, vector<pair<int,int>>& currentSolution, int numberOfEnclosure, vector<vector<pair<int,int>>>&  neighborList, int maxDistance, unsigned int*& enclosureSize, int**& weightedMatrix){
        vector<int> enclosureInCluster ;
        vector<int> tabouCluster(100, -1);
        clustering(res, tabouCluster, weightedMatrix, enclosureSize, numberOfEnclosure, enclosureInCluster);

        for (int enclosure : enclosureInCluster){
        for (int x=-maxDistance; x<maxDistance; x++){
            for (int y=-maxDistance; y<maxDistance; y++){
                bool isPossibleVar = isPossible(enclosure, currentSolution, numberOfEnclosure, enclosureSize, currentSolution[enclosure].first+x, currentSolution[enclosure].second+y);
                if ((isPossibleVar) && (abs(x)+abs(y) <= maxDistance)){
                    vector<pair<int,int>> neighbor = currentSolution;
                    neighbor[enclosure] = pair<int,int>(currentSolution[enclosure].first+x, currentSolution[enclosure].second+y);
                    // Verifier la rangée d'après pour savoir si elle est vide,
                    if (x != 0){
                        bool enclosureVerticalIsEmpty = true;
                        for (int i=0; i<numberOfEnclosure; i++){
                            if (neighbor[i].first == currentSolution[enclosure].first){
                                enclosureVerticalIsEmpty = false;
                                break;
                            }
                        }
                        if (enclosureVerticalIsEmpty){
                            for (int i=0; i<numberOfEnclosure; i++){
                                if (neighbor[i].first <= currentSolution[enclosure].first){
                                    neighbor[i].first += 1;
                                }
                            }
                        }
                    }
                    neighborList.push_back(neighbor);
                }
            }
        }
        }
}

void tabouSearch(int**& weightedMatrix, int*& subSet, int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, int maxDistance, int tabouTenure, bool printP){
    vector<vector<pair<int,int>>> tabouList = vector<vector<pair<int,int>>>(1,vector<pair<int,int>>(numberOfEnclosure));
    tabouList.clear();
    
    vector<pair<int,int>> currentSolution = vector<pair<int,int>>(numberOfEnclosure);
    for (int i=0; i<numberOfEnclosure; i++){
        currentSolution[i] = pair<int,int>(res[i][0], res[i][1]);
    }
    long int bestScore = calculateCostNeighbor(weightedMatrix,subSet,currentSolution,numberOfEnclosure,numberOfSubset,maxDistanceSubSet,enclosureSize);

    vector<vector<pair<int,int>>>  neighborList;

    bool work = true;

    //Tabou
    bool isInTabouList = false;
    bool sameSolution = false;

    int numberOfIterationWithoutChangement = 0;
    bool newSolutionFind = false;

    while (work && (numberOfIterationWithoutChangement < 20)){
        // findNeighborhood2(currentSolution, numberOfEnclosure, neighborList, maxDistance, enclosureSize);
        findNeighborhood(res, currentSolution, numberOfEnclosure, neighborList, maxDistance, enclosureSize, weightedMatrix);
        newSolutionFind = false;
        for (int neighbor=0; neighbor<neighborList.size(); neighbor++){
            // Verify is solution is in tabou list
            isInTabouList = false;
            for (int tabou=0; tabou<tabouList.size(); tabou++){
                sameSolution = true;
                for (int i=0; i<numberOfEnclosure; i++){
                    if ((tabouList[tabou][i].first != neighborList[neighbor][i].first) || (tabouList[tabou][i].second != neighborList[neighbor][i].second)){
                        sameSolution = false;
                    }
                }
                if (sameSolution){
                    isInTabouList = true;
                }
            }

            if (!isInTabouList){
                long int neighborScore = calculateCostNeighbor(weightedMatrix, subSet, neighborList[neighbor], numberOfEnclosure, numberOfSubset, maxDistance, enclosureSize);
                if (bestScore < neighborScore){
                    currentSolution = neighborList[neighbor];
                    bestScore = neighborScore;

                    for (int enclosure=0; enclosure<numberOfEnclosure; enclosure++){
                        placeRectangle(enclosure,res,enclosureSize[enclosure],currentSolution[enclosure].first,currentSolution[enclosure].second);
                    }

                    tabouList.push_back(currentSolution);
                    if (tabouList.size() > tabouTenure){
                        tabouList.erase(tabouList.begin());
                    }

                    std::cout << bestScore << std::endl;
                    if (printP){
                        printEnclosure(res, enclosureSize,numberOfEnclosure);
                        //printPlan(res, enclosureSize, numberOfEnclosure);
                    }
                    
                    newSolutionFind = true;
                }
            }
        }

        if (!newSolutionFind){
            numberOfIterationWithoutChangement ++;
            std::cout << "iteration " << numberOfIterationWithoutChangement << std::endl;
        } else {
            numberOfIterationWithoutChangement = 0;
        }
    }
}



/* || Algorithme || */
void algo(int**& weightedMatrix, int*& subSet, int**& res,  unsigned int*& enclosureSize, int numberOfEnclosure, int numberOfSubset, int maxDistanceSubSet, bool printP){
    long int cost;
    
    // Initialization with glouton algorithme
    glouton(subSet, res, enclosureSize, numberOfEnclosure, numberOfSubset, weightedMatrix, maxDistanceSubSet);
    // verifyIsOkay(res,enclosureSize,numberOfEnclosure);
    cost = calculateCost(res,enclosureSize,weightedMatrix,numberOfEnclosure,numberOfSubset, subSet, maxDistanceSubSet);
    std::cout << cost << std::endl;
    if (printP){
        printEnclosure(res,enclosureSize,numberOfEnclosure);
        printPlan(res, enclosureSize, numberOfEnclosure);
    }
    
    // Algorithme tabou
    tabouSearch(weightedMatrix,subSet,res,enclosureSize,numberOfEnclosure,numberOfSubset,maxDistanceSubSet,5,10,printP);
    
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