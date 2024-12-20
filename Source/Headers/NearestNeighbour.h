//
// Created by Silver on 10.12.2024.
//

#ifndef TSP_3_NEARESTNEIGHBOUR_H
#define TSP_3_NEARESTNEIGHBOUR_H


#include "TimeCounter.h"
#include "Structures.h"


class NearestNeighbour
{
private:
public:
    int getCost();
    double runNN(int ** graphMatrix, int v,int startingNode,bool print,int killSwitch,bool enchancedNN);

};

#endif //TSP_3_NEARESTNEIGHBOUR_H
