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

vector<int> SimulatedAnnealing::getRandomNeighbor(const vector<int>& currentTour,int method)
{
    vector<int> neighbor = currentTour;

    uniform_int_distribution<int> distribution(0,neighbor.size() - 1);

    int i = distribution(generatorAnn);
    int j = distribution(generatorAnn);
    while (i == j) {
        j = distribution(generatorAnn);
    }

    if(method == 0)
    {
        swap(neighbor[i], neighbor[j]);
    }
    else if(method == 1)
    {
        if(i > j)
            swap(i,j);

        int fragmentLenght = (j - i) + 1;
        int fragment[fragmentLenght];

        int counter = 0;
        for(int x = i; x<= j; x++)
        {
            fragment[j-i-counter] = neighbor[x];
            counter++;
        }
        for(int x = i; x<= j; x++)
        {
            neighbor[x] = fragment[x-i];
        }
    }


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

ResultSave SimulatedAnnealing::runAnnealing(int **matrix, int size,int initTemperature, double coolingRate, bool print, int killSwitch, int optimal,int firstMethod,int startingNode,double finalTemp,int iterations,int coolingMethod,int neighbourMethod)
{

    uniform_real_distribution<double> randomValue(0.0f,1.0f);
    vector<int> currentSolution(size);

    double temperature = initTemperature;
    int noImprovement=0;
    int eraLenght = 50;

    startCounter();
    if(firstMethod == 0)
    {
        for (int i = 0; i < size; i++)
        {
            currentSolution[i] = i;
        }
        for (int i = 0; i < size; i++) {
            std::uniform_int_distribution<int> destination(0, size - 1 - i);
            int target = destination(generatorAnn);
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
    int currentCost = bestCost;
    int eraNumber = 0;
    while(true)
    {

        if(bestCost <= optimal )
        {
            if(print)
                cout<<" Found optimal value!"<<endl;
            break;
        }

        if(temperature <= finalTemp )
        {
            if(print)
                cout<<" Cooled down!"<<endl;
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
        int currentIteration = 0;
        noImprovement++;
        eraLenght = (int)(size/log(eraNumber+2)); // Decrease size with temp. drop
        while(currentIteration < eraLenght)
        {
            vector<int> neighbour = getRandomNeighbor(currentSolution,neighbourMethod);
            int neighbourCost = calculateTourCost(neighbour, matrix);
            double expValue = exp((currentCost - neighbourCost) / temperature);

            if (neighbourCost < currentCost || expValue > randomValue(generatorAnn)) {
                currentSolution = neighbour;
                currentCost = neighbourCost;

                if (currentCost < bestCost) {
                    bestCost = currentCost;
                    bestSolution = currentSolution;
                    noImprovement = 0;
                }
            }
            currentIteration++;
        }
        eraNumber++;
        if(coolingMethod == 0)
            temperature = temperature * coolingRate;
        else if(coolingMethod == 1)
            temperature = (initTemperature/log(1+eraNumber));


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