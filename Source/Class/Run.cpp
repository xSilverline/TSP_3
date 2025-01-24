//
// Created by Silver on 01.11.2024.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include "../Headers/Run.h"


string outFileName;
string inFileName;
int optimal;


class WithComma: public numpunct<char> // class for decimal numbers with comma
{
protected: char do_decimal_point() const override { return ','; } // override the function that gives the decimal separator
};

void Run::run(int **matrix,int size, int reps,int start,int killSwitch,const string& method,string output,string input,int opt,bool print,int iterations, int tabuSize,int ttl, int delta, int temperature, double coolingRate, int firstMethod, double finalTemp,int coolingMethod, int neighbourMethod)
{
    optimal = opt;
    inFileName = input;
    outFileName = output;

    this->graphMatrix = matrix;
    this->graphSize = size;
    this->repeats = reps;

    this->startingNode = start;

    int initialBestCost = INT_MAX;
//    if(size > 10)
//    {
//        NearestNeighbour nn;
//        nn.runNN(matrix,size,start,false,killSwitch,false);
//        initialBestCost = nn.getCost()+1;
//        cout<<initialBestCost<<endl;
//    }
//
//
//    DFS dfs;
    locale myloc(  locale(),    // C++ default locale
                   new WithComma);
    ofstream outputFile;
    outputFile.imbue(myloc);
    outputFile.open (outFileName);
    outputFile<<"Input file\t"<<inFileName<<endl;
    outputFile<<"Optimal cost\t"<<optimal<<endl;
    double timeRandomTS=0;
    double timeTS=0;
    double timeSA=0;

    double oneTimeRTS=0;
    double oneTimeTS=0;
    double oneTimeSA=0;


    if(method == "ts")
    {
        cout<<" ---- Setting for TS ----"<<endl<<endl;
        cout<<"Taboo List Size: "<<tabuSize<<endl;
        cout<<"Iterations: "<<iterations<<endl;
        cout<<"Cadency Time: "<<ttl<<endl;
        cout<<"Aspiration: "<<delta<<endl<<endl;
    }

    if(method == "sa")
    {
        cout<<" ---- Setting for SA ----"<<endl<<endl;
        cout<<"Temperature: "<<temperature<<endl;
        cout<<"Cooling Rate: "<<coolingRate<<endl<<endl;

    }


    for(int i = 0; i< reps;i++)
    {
        cout<<" ------- RUN "<<i+1<<" -------"<<endl<<endl;
        if (method == "ts")
        {
            TabuSearch tabuSearch;
            ResultSave rs = tabuSearch.runSearch(matrix,size,iterations,tabuSize,0,ttl,delta,print,killSwitch,optimal, firstMethod, startingNode);
            oneTimeTS  = rs.time;
            timeTS+=oneTimeTS;
            if(print)
                cout<<"Time: "<<oneTimeTS<<" ms"<<endl;
            outputFile<<"Time TS Rep"<<i+1<<"\t"<<fixed<<setprecision(4)<<oneTimeTS<<endl;
            cout<<endl<<endl;

//        } else if (method == "rts") {
//            TabuSearch tabuSearch;
//            oneTimeRTS = tabuSearch.runSearch(matrix,size,iterations,tabuSize,1,ttl,delta,print,killSwitch,optimal);
//            if(print)
//                cout<<"Time: "<<oneTimeRTS<<" ms"<<endl;
//            timeRandomTS+=oneTimeRTS;
//            outputFile<<"Time RTS Rep"<<i+1<<"\t"<<fixed<<setprecision(4)<<oneTimeRTS<<endl;
//            cout<<endl<<endl;

        }else if (method == "sa") {
            SimulatedAnnealing simulatedAnnealing;
            ResultSave rs = simulatedAnnealing.runAnnealing(matrix,size,temperature,coolingRate, print,killSwitch,optimal,firstMethod,startingNode,finalTemp,iterations,coolingMethod,neighbourMethod);
            oneTimeSA= rs.time;
            timeSA += oneTimeSA;
            if(print)
                cout<<"Time: "<<oneTimeSA<<" ms"<<endl;
            outputFile<<"Time SA Rep "<<i+1<<"\t"<<fixed<<setprecision(4)<<oneTimeSA<<endl;
            cout<<endl<<endl;
//        } else if (method == "all") {
//            Node *root = new Node;
//            oneTimeDfs = dfs.runDFS(root, matrix, size, initialBestCost, start, killSwitch,print);
//            timedfs+=oneTimeDfs;
//            outputFile<<"Time DFS Rep "<<i+1<<"\t"<<fixed<<setprecision(4)<<oneTimeDfs<<endl;
//            dfs.clean(root);
//            cout<<endl<<endl;
//
//            root = new Node;
//            BFS bfs;
//            oneTimeBfs = bfs.runBFS(root, matrix, size, initialBestCost, start, killSwitch,print);
//            timebfs+=oneTimeBfs;
//            outputFile<<"Time BFS Rep "<<i+1<<"\t"<<fixed<<setprecision(4)<<oneTimeBfs<<endl;
//            dfs.clean(root);
//            cout << endl << endl;
//
//            root = new Node;
//            BestFirst bf;
//            oneTimeBest= bf.runBestFirst(root, matrix, size, initialBestCost, start, killSwitch,print);
//            timebest+=oneTimeBest;
//            outputFile<<"Time Best Rep "<<i+1<<"\t"<<fixed<<setprecision(4)<<oneTimeBest<<endl;
//            dfs.clean(root);
//            cout << endl << endl;

        } else
            cout << "Wrong method! Check config file ! " << endl;
    }
    timeRandomTS/=reps;
    timeTS/=reps;
    timeSA/=reps;
//
    if(timeRandomTS!=0)
        outputFile<<"Avg Time RTS \t"<<fixed<<setprecision(4)<<timeRandomTS<<endl;
    if(timeTS !=0)
        outputFile<<"Avg Time TS \t"<<fixed<<setprecision(4)<<timeTS<<endl;
    if(timeSA !=0)
        outputFile<<"Avg Time SA \t"<<fixed<<setprecision(4)<<timeSA<<endl;


}



