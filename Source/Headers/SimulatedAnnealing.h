//
// Created by Silver on 19.12.2024.
//

#ifndef TSP_3_SIMULATEDANNEALING_H
#define TSP_3_SIMULATEDANNEALING_H


#include "Structures.h"

class SimulatedAnnealing {
public:
    ResultSave runAnnealing(int **matrix, int size, int temperature, double coolingRate, int alpha, bool print, int killSwitch, int optimal);


private:


    int calculateTourCost(const vector<int> &tour, int **matrix);

    vector<vector<int>> getAllNeighbours(const vector<int> &currentTour);

    vector<int> getRandomNeighbor(const vector<int> &currentTour);
};


#endif //TSP_3_SIMULATEDANNEALING_H
