//
// Created by Silver on 10.12.2024.
//

#include <random>
#include <chrono>
#include "../Headers/SimulatedAnnealing.h"

default_random_engine generatorAnn(chrono::system_clock::now().time_since_epoch().count());


// Function to calculate the total cost of a TSP tour
int SimulatedAnnealing::calculateTourCost(const vector<int>& tour, int **matrix) {
    int cost = 0;
    for (int i = 0; i < tour.size() - 1; i++) {
        cost += matrix[tour[i]][tour[i + 1]];
    }
    cost += matrix[tour.back()][tour[0]]; // Return to the start
    return cost;
}

vector<int> SimulatedAnnealing::getRandomNeighbor(const vector<int>& currentTour)
{
    vector<int> neighbor = currentTour;

    uniform_int_distribution<int> distribution(0,neighbor.size() - 1);

    int i = distribution(generatorAnn);
    int j = distribution(generatorAnn);
    while (i == j) {
        j = distribution(generatorAnn);
    }
    swap(neighbor[i], neighbor[j]);

    return neighbor;
}

vector<vector<int>> SimulatedAnnealing::getAllNeighbours(const vector<int>& currentTour)
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

ResultSave SimulatedAnnealing::runAnnealing(int **matrix, int size,int initTemperature, double coolingRate, int alpha, bool print, int killSwitch, int optimal)
{

    uniform_real_distribution<double> randomValue(0.0f,1.0f);
    vector<int> currentSolution(size);

    double temperature = initTemperature;

    startCounter();
    for (int i = 0; i < size; i++)
    {
        currentSolution[i] = i;

    }


    for (int i = 0; i < size; i++)
    {
        std::uniform_int_distribution<int> destination(0, size - 1 - i);
        int target = destination(generatorAnn);
        if (target != (size - 1 - i))
            swap(currentSolution[size - 1 - i], currentSolution[target]);
    }

    vector<int> bestSolution = currentSolution;
    int bestCost = calculateTourCost(currentSolution, matrix);
    vector<int> currentNeighbour;
    int currentCost = bestCost;

    while(temperature > 1e-10)
    {
        if(getCounter()>=killSwitch)
        {
            ResultSave rs(killSwitch,abs(bestCost-optimal),(abs(double(bestCost-optimal))/optimal));
            return rs;
        }
        vector<int> neighbour = getRandomNeighbor(currentSolution);
        int neighbourCost = calculateTourCost(neighbour,matrix);
        double expValue = exp((currentCost - neighbourCost)/temperature);
        if(neighbourCost < currentCost || expValue > randomValue(generatorAnn))
        {
            currentSolution = neighbour;
            currentCost = neighbourCost;

            if(currentCost < bestCost)
            {
                bestCost = currentCost;
                bestSolution = currentSolution;
            }
        }

        temperature = temperature * coolingRate;

//        cout<<temperature<<endl;


    }






        // Output progress
//        cout << "Iteration " << iteration + 1
//             << ": Current Cost = " << candidateCost
//             << ", Best Cost = " << bestCost << endl;

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