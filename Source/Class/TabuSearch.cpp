//
// Created by Silver on 10.12.2024.
//

#include "../Headers/TabuSearch.h"
#include "../Headers/Structures.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <random>
#include <chrono>

default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());


// Function to calculate the total cost of a TSP tour
int TabuSearch::calculateTourCost(const vector<int>& tour, int **matrix) {
    int cost = 0;
    for (int i = 0; i < tour.size() - 1; i++) {
        cost += matrix[tour[i]][tour[i + 1]];
    }
    cost += matrix[tour.back()][tour[0]]; // Return to the start
    return cost;
}

vector<vector<int>> TabuSearch::getAllNeighbours(const vector<int>& currentTour)
{
    vector<vector<int>> allNeighbours;
    vector<int> currentNeighbour;
    for(int i = 0; i<currentTour.size(); i++)
    {
        for(int j = i+1; j<currentTour.size();j++)
        {
            currentNeighbour = currentTour;
            swap(currentNeighbour[i],currentNeighbour[j]);
            allNeighbours.push_back(currentNeighbour);
        }
    }

    return allNeighbours;
}
// Generate a random neighbor by swapping two cities in the tour
vector<int> TabuSearch::getRandomNeighbor(const vector<int>& currentTour)
{
    vector<int> neighbor = currentTour;

    uniform_int_distribution<int> distribution(0,neighbor.size() - 1); // Avoid the first city (start point)

    int i = distribution(generator);
    int j = distribution(generator);
    while (i == j) {
        j = distribution(generator);
    }
    swap(neighbor[i], neighbor[j]);

    return neighbor;
}

void TabuSearch::runRandomTabuSearch(int **matrix, int size, int iterations,int tabuSize)
{
    vector<int> currentSolution(size);
    vector<vector<int>> tabuList;
    for (int i = 0; i < size; i++) {
        currentSolution[i] = i;
    }

    for (int i = 0; i < size; i++)
    {
        std::uniform_int_distribution<int> destination(0, size - 1 - i);
        int target = destination(generator);
        if (target != (size - 1 - i))
            swap(currentSolution[size - 1 - i], currentSolution[target]);
    }

    vector<int> bestSolution = currentSolution;
    int bestCost = calculateTourCost(currentSolution, matrix);


    for (int iteration = 0; iteration < iterations; iteration++) {
        vector<int> candidateSolution;
        int candidateCost = INT_MAX;

        // Generate a valid neighbor not in the tabu list
        do {
            candidateSolution = getRandomNeighbor(currentSolution);
        } while (find(tabuList.begin(), tabuList.end(), candidateSolution) != tabuList.end());

        candidateCost = calculateTourCost(candidateSolution, matrix);

        // Update the current solution
        if (candidateCost < bestCost) {
            bestSolution = candidateSolution;
            bestCost = candidateCost;
        }

        currentSolution = candidateSolution;

        // Update the tabu list
        tabuList.push_back(currentSolution);
        if (tabuList.size() > tabuSize) {
            tabuList.erase(tabuList.begin());
        }

        // Output progress
//        cout << "Iteration " << iteration + 1
//             << ": Current Cost = " << candidateCost
//             << ", Best Cost = " << bestCost << endl;
    }

    // Output the best solution
    cout << "\nOptimal Tour: ";
    for (int city : bestSolution) {
        cout << city << " ";
    }
    cout << "\nOptimal Cost: " << bestCost << endl;



}

ResultSave TabuSearch::runTabuSearch(int **matrix,int size,int iterations, int tabuSize, int ttl, int delta,bool print, int killSwitch, int optimal,int firstMethod,int startingNode)
{
//    int ttl = 10;
//    int delta = 0;
    vector<int> currentSolution(size);
    int noImprovement = 0;

//    vector<vector<int>> tabuList;
    TabuList tabuList(tabuSize,ttl);

    startCounter();


    if(firstMethod == 0)
    {
        for (int i = 0; i < size; i++)
        {
            currentSolution[i] = i;
        }
        for (int i = 0; i < size; i++) {
            std::uniform_int_distribution<int> destination(0, size - 1 - i);
            int target = destination(generator);
            if (target != (size - 1 - i))
                swap(currentSolution[size - 1 - i], currentSolution[target]);
        }
    }
    else
    {
        NearestNeighbour nn;
        nn.runNN(matrix,size,startingNode,false,killSwitch,false);
        currentSolution = nn.getPath();
    }

    vector<int> bestSolution = currentSolution;
    int bestCost = calculateTourCost(currentSolution, matrix);
    vector<int> currentNeighbour;

    while(true)
    {

        if(bestCost <= optimal )
        {
            if(print)
                cout<<" Found optimal value!"<<endl;
            break;
        }
        if(noImprovement >= iterations && iterations != -1)
        {
            if(print)
                cout<<" No improvement limit reached! "<<endl;
            break;
        }
        if( getCounter() >= killSwitch)
        {
            if(print)
                cout<<" Time limit reached! "<<endl;
            break;
        }


        int candidateCost = INT_MAX;
        vector<int> candidateSolution;
        CandidatePair candidatePair{};

        int candidateTaboCost = INT_MAX;
        vector<int> candidateTaboSolution;
        CandidatePair candidateTaboPair{};

        for(int i = 0; i<currentSolution.size(); i++)
        {
            for(int j = i+1; j<currentSolution.size();j++)
            {

                currentNeighbour = currentSolution;
                swap(currentNeighbour[i],currentNeighbour[j]);
                int currentNeighbourCost = calculateTourCost(currentNeighbour,matrix);
                if(currentNeighbourCost<candidateCost)
                {
                    if(!tabuList.searchForPair(i,j) || currentNeighbourCost < (bestCost-delta)) // if not in taboo or so good that we break the LAW, make it candidate
                    {
                        candidateSolution = currentNeighbour;
                        candidateCost = currentNeighbourCost;
                        candidatePair.from = i;
                        candidatePair.to = j;
                    }
                    else // if all in taboo , save best from them. Escape from local minimum
                    {
                        candidateTaboPair.from = i;
                        candidateTaboPair.to = j;
                        candidateTaboCost = currentNeighbourCost;
                        candidateTaboSolution = currentNeighbour;
                    }
                }

            }
        }
        tabuList.reduceLife();

        if(candidateCost != INT_MAX)
        {
            if(candidateCost<bestCost)
            {
                bestCost = candidateCost;
                bestSolution = candidateSolution;
                noImprovement = 0;
            }

            tabuList.addPair(candidatePair.from,candidatePair.to);
            currentSolution = candidateSolution;
        }
        else
        {
//            bestCost = candidateTaboCost;
//            bestSolution = candidateTaboSolution;
            tabuList.addPair(candidateTaboPair.from,candidateTaboPair.to);
            currentSolution = candidateTaboSolution;
        }





        // Output progress
//        cout << "Iteration " << iteration + 1
//             << ": Current Cost = " << candidateCost
//             << ", Best Cost = " << bestCost << endl;
    }
    double time = getCounter();

    // Output the best solution
    double relativError = (abs(double(bestCost-optimal))/optimal);
    double absoluteError = abs(bestCost-optimal);
    ResultSave resultSave(time, absoluteError,relativError);
    if(print)
    {
        cout << "\nOptimal Tour: ";
        for (int city: bestSolution) {
            cout << city << " ";
        }
        cout << "\nOptimal Cost: " << bestCost << endl;

        cout<<"Relative Error : "<<relativError<<endl;
        cout<<"Relative Error [%]: "<<relativError * 100<<" %"<<endl;
        cout<<"Absolute Error : "<<absoluteError<<endl;

    }

    return resultSave;


}


ResultSave TabuSearch::runSearch(int **matrix, int size,int iterations,int tabuSize,int method, int ttl, int delta, bool print, int killSwitch, int optimal,int firstMethod,int startingNode)
{

//    if(method == 1)
//    {
//
//        return runRandomTabuSearch(matrix,size,iterations,tabuSize);
//
//    }

    return runTabuSearch(matrix,size,iterations,tabuSize,ttl,delta,print,killSwitch,optimal, firstMethod, startingNode);

}
