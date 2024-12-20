//#include <iostream>
#include <fstream>

#include "../Headers/Structures.h"
#include "../Headers/Run.h"
#include "../Libraries/INIReader.h"

using namespace std;



string inputFileName = "data2.txt";
string outputFileName = "result.xlm";

int repeats=1;
int killSwitch = 50000;
int startingNode = 0;
int iterations = 1000;
int tabuSize = 10;
int ttl = 10;
int delta = 0;

int temperature = 1000;
double coolingRate = 0.91;
int alpha = 0;

string method;

int dataSize = 0;
int optimalVal = 0;
int ** graphMatrix = nullptr;

bool print = true;
bool test = false;

void createData()
{
    fstream dataInput;
    dataInput.open(inputFileName);

    dataInput >> dataSize;
    dataInput>>optimalVal;

    graphMatrix = new int * [dataSize];

    int temp=0;
    for(int i = 0; i< dataSize; i++)
    {
        graphMatrix[i] = new int [dataSize];
        for(int j = 0; j<dataSize;j++)
        {
            dataInput >> temp;

            graphMatrix[i][j] = temp;
        }
    }
}

int main()
{

    INIReader configReader("config.ini");

    if(configReader.ParseError() < 0)
    {
        cout<<"Cannot read config file! "<<endl;
        return 1;
    }

    inputFileName = configReader.Get("Settings","inputFileName","NONE");
    outputFileName = configReader.Get("Settings","outputFileName","DefaultOutput.txt");
    repeats = configReader.GetInteger("Settings","numberOfRepeats",1);
    killSwitch = configReader.GetInteger("Settings","killSwitch",30000);
    startingNode = configReader.GetInteger("Settings","startingNode",0);
    method = configReader.Get("Settings","chooseMethod","all");
    print = configReader.GetBoolean("Settings","printInConsole",true);
    test = configReader.GetBoolean("Settings","test",false);
    iterations = configReader.GetInteger("Settings","iterations",1000);
    tabuSize = configReader.GetInteger("Settings","tabuSize",10);
    ttl = configReader.GetInteger("Settings","cadencyTime",10);
    delta = configReader.GetInteger("Settings","aspiration",0);
    temperature = configReader.GetInteger("Settings","temperature",1000);
    coolingRate = configReader.GetReal("Settings","coolingRate",0.99);
//    alpha = configReader.GetInteger("Settings","alpha",0);


    cout<<endl<<endl;
    Run run{};
    if(test)
    {
        print = false;
        cout<<"OK";
        for(int i = 6;i<16;i++)
        {
            if(i == 7 || i == 9)
                i++;
            string file = "matrix_";
            file.append(to_string(i));
            file.append(".txt");
            inputFileName = file;
            cout << "Input File: "<<inputFileName<<endl<<endl;
            string outputf = "result_";
            outputf.append(to_string(i));
            outputf.append(".xls");
            outputFileName = outputf;

            createData();
            run.run(graphMatrix,dataSize,repeats,startingNode,killSwitch,method,outputFileName,inputFileName,optimalVal,print,iterations,tabuSize,ttl,delta,temperature,coolingRate,alpha);
            cout<<"Output File: "<<outputFileName<<endl<<endl;
        }
    }
    else
    {
        cout<<" ---- INPUT FILES ----"<<endl<<endl;
        cout<<"Config File: "<<"config.ini"<<endl;
        cout << "Input File: "<<inputFileName<<endl<<endl;
        cout<<" ---- OUTPUT FILES ----"<<endl<<endl;
        cout<<"Output File: "<<outputFileName<<endl<<endl;
        // Read Done, Ini file works properly :)

        createData();

        cout<<" ---- PROGRAM SETTINGS ----"<<endl<<endl;
        cout<<"Time limit :"<<killSwitch<<" ms"<<endl;
        cout<<"Repeats: "<<repeats<<endl;
        cout<<"Method: "<<method<<endl;
        cout<<"First node: "<<startingNode<<endl;
        cout<<"Instance size: "<<dataSize<<endl<<endl;



        cout<<" ---- KNOWN OPTIMAL ----"<<endl<<endl;
        cout<<"Optimal value: "<<optimalVal<<endl<<endl;

        cout<<" ---- RESULTS ----"<<endl<<endl;
        run.run(graphMatrix,dataSize,repeats,startingNode,killSwitch,method,outputFileName,inputFileName,optimalVal,print,iterations,tabuSize,ttl,delta,temperature,coolingRate,alpha);
    }



    if(!test)
        system("pause");

    return 0;
}
