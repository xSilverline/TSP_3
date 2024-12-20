//
// Created by Silver on 19.12.2024.
//

#ifndef TSP_3_RUN_H
#define TSP_3_RUN_H


#include "NearestNeighbour.h"
#include "Structures.h"
#include "TabuSearch.h"
#include "SimulatedAnnealing.h"

class Run
{

private:
    int ** graphMatrix;
    int graphSize;
    int repeats;
    int startingNode;



//    void buildTree(int **matrix, int size, int start);


public:
    void run(int ** graphMatrix, int graphSize, int repeats,int start,int killSwitch,const string& method,string output,string input,int opt,bool print,int iterations, int tabuSize, int ttl, int delta, int temperature, double coolingRate, int alpha);


//    void addToTree(int **matrix, int size, Node* parent, int *visited);

//    int caluculateBound(int **matrix, int siz,int visited[]);
};



#endif //TSP_3_RUN_H
