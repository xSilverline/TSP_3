//
// Created by Silver on 10.12.2024.
//

#ifndef TSP_3_TABUSEARCH_H
#define TSP_3_TABUSEARCH_H


#include <vector>
#include "Structures.h"
#include "NearestNeighbour.h"

class TabuSearch {
public:

    ResultSave runSearch(int **matrix, int size, int iterations, int tabuSize, int method, int ttl, int delta, bool print, int killSwitch, int optimal,int firstMethod,int startingNode);

private:
    ResultSave runTabuSearch(int **matrix, int size, int iterations, int tabuSize,int ttl, int delta, bool print, int killSwitch, int optimal,int firstMethod,int startingNode);

    void runRandomTabuSearch(int **matrix, int size, int iterations, int tabuSize);

    std::vector<int> getRandomNeighbor(const std::vector<int> &currentTour);

    std::vector<std::vector<int>> getAllNeighbours(const std::vector<int>& currentTour);

    int calculateTourCost(const std::vector<int> &tour, int **matrix);
};


#endif //TSP_3_TABUSEARCH_H
