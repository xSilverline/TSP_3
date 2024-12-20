//
// Created by Silver on 05.10.2024.
//

#include "../Headers/NearestNeighbour.h"

int **nnMatrix = nullptr;
int ** result = nullptr;
int cost=0;

double NearestNeighbour::runNN(int **graphMatrix, int v,int startingNode,bool print,int killSwitch,bool enchancedNN)
{
    int visited [v];
    int visitedDeep [v];
    int visitSum = 0;
    int visitSumDeep = 0;
    int totalCost = 0;
    int costList [v];
    result = new int * [v];
    int resultCounter=0;
    int minCost = MAXINT;
    nnMatrix = new int * [v];
    int minimalNode = 0;
    string path;
    int currentNode = 0;

    for(int i =0; i< v; i++)
    {
        visited[i] = 0;
        visitedDeep[i] = 0;
        costList[i] = 0;
        result[i] = new int [v];
        nnMatrix[i] = new int [v];
        std::copy(graphMatrix[i],graphMatrix[i]+v,nnMatrix[i]);
    }
    for(int i = 0; i<v; i++)
    {
        for(int j = 0; j<v; j++)
        {
            result[i][j] = 0;
        }
    }
    if(print)
    {
        if(!enchancedNN)
        {
            cout << "-- Nearest Neighbour -- " << endl;
        }
        else
            cout << "-- Enchanced Nearest Neighbour -- " << endl;

    }

    startCounter();
    for(int i=0; i<v;i++)
    {
        currentNode = i;
//        std::cout << "NODE: " << currentNode << endl;
        resultCounter = 0;
        result[i][resultCounter] = currentNode;
        resultCounter++;
        visitSum = 0;
        visitSumDeep = 0;
        totalCost = 0;
        for(int j = 0; j<v;j++)
        {
            visited[j] = 0;
            visitedDeep[j] = 0;
        }



        while(visitSum<v-1)
        {
//            std::cout << "NODE-BB: " << currentNode << endl;
            double killTimer = getCounter();

            if(killTimer > killSwitch)
                return killTimer;

            visited[currentNode] = 1;
            visitedDeep[currentNode] = 1;
            visitSum++;
            visitSumDeep++;
            int minimum = MAXINT;
            int minimumDeep = MAXINT;
            int goToN = -1;
            bool switchek = true;


            if(enchancedNN)
            {
                for (int neighbour = 0; neighbour < v; neighbour++) {
                    for (int j = 0; j < v; j++) {
                        visitedDeep[j] = visited[j];
                    }
                    visitSumDeep = visitSum;

                    int temp = nnMatrix[currentNode][neighbour];
                    int deepMin = MAXINT;
                    int goToDeep = -1;

                    if (temp != -1 && !visited[neighbour]) {
                        visitedDeep[neighbour] = 1;
                        visitSumDeep++;
                        if (visitSumDeep == v)
                            deepMin = nnMatrix[neighbour][i];
                        else {
                            for (int nextNeighbour = 0; nextNeighbour < v; nextNeighbour++) {
                                int deepTemp = nnMatrix[neighbour][nextNeighbour];
                                if (deepTemp != -1 && deepTemp < deepMin && !visitedDeep[nextNeighbour]) {
                                    deepMin = deepTemp;
                                    //goToDeep = nextNeighbour;
                                }
                            }
                        }

                        if (deepMin < MAXINT - temp)
                            if (temp + deepMin < minimumDeep) {
                                minimumDeep = temp + deepMin;
                                minimum = temp;
                                goToN = neighbour;
                            }
                    }
                }
            }
            else
            {
                for (int neighbour = 0; neighbour < v; neighbour++)
                {

                    int temp = nnMatrix[currentNode][neighbour];

                    if (temp != -1 && temp < minimum && !visited[neighbour]) {
                        minimum = temp;
                        goToN = neighbour;
                    }
                }
            }
            result[i][resultCounter] = goToN;
            resultCounter++;

            totalCost+=minimum;
            currentNode = goToN;

        }

        totalCost += graphMatrix[currentNode][i];
        costList[i] = totalCost;

        if(totalCost < minCost)
        {
            cost = totalCost;
            minCost = totalCost;
            minimalNode = i;
        }

    }

    double time = getCounter();
//    std::cout<<currentNode<<" -> "<<startingNode<<" "<<graphMatrix[currentNode][startingNode]<<endl;
    if(print)
    {
        for(int j = 0; j<v;j++)
            cout<< "Cost from "<<j<<" -> "<<costList[j]<<endl;



        if (path.size() < 17) {
            std::cout << "Best Path: " << endl;
            for (int i = 0; i<v;i++)  {
                cout <<result[minimalNode][i] << " -> ";
            }
            cout << result[minimalNode][0] << endl;
        }
        std::cout << "Cost: " << cost << endl;
        std::cout << "Time: " << time << " ms" << endl;
    }

    return time;
}

int NearestNeighbour::getCost()
{
    return cost;
}
