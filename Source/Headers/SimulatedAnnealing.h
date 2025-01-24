//
// Created by Silver on 10.12.2024.
//

#ifndef TSP_3_SIMULATEDANNEALING_H
#define TSP_3_SIMULATEDANNEALING_H


#include "Structures.h"
#include "NearestNeighbour.h"

class SimulatedAnnealing {
public:
    ResultSave runAnnealing(int **matrix, int size, int temperature, double coolingRate, bool print, int killSwitch, int optimal,int firstMethod,int startingNode,double finalTemp,int iterations,int coolingMethod,int neighbourMethod);


private:


    int calculateTourCost(const vector<int> &tour, int **matrix);

    vector<vector<int>> getAllNeighbours(const vector<int> &currentTour);

    vector<int> getRandomNeighbor(const vector<int> &currentTour,int method);
};


#endif //TSP_3_SIMULATEDANNEALING_H
