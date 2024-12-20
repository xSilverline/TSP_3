//
// Created by Silver on 10.12.2024.
//

#ifndef TSP_3_STRUCTURES_H
#define TSP_3_STRUCTURES_H
#include <vector>
#include <algorithm>
#include <deque>
#include "TimeCounter.h"

using namespace std;

struct ResultSave
{
    double time=0;
    double absError = 0.0;
    double relError = 0.0;

    ResultSave(double time, double abs, double rel)
    {
        this->time = time;
        this->absError = abs;
        this->relError = rel;
    }
};

struct TabuPair
{
    int from = 0;
    int to = 0;
    int ttl =0;
};


struct CandidatePair
{
    int from;
    int to;
};

struct TabuList
{
    int tabuSize;
    int currentSize;
    int cadency;
    deque<TabuPair> pairList;

    TabuList(int tabuSize, int cadency)
    {
        this->currentSize = 0;
        this->tabuSize = tabuSize;
        this->cadency = cadency;
        pairList.resize(tabuSize);
    }
    ~TabuList()
    {
        pairList.clear();
    }

    void addPair(int from, int to)
    {
        TabuPair tp;
        tp.from = from;
        tp.to = to;
        tp.ttl = cadency;


        for(TabuPair t:pairList)
        {
            if(t.from == from && t.to == to)
            {
                t.ttl = cadency;
                return;
            }
        }

        pairList.push_back(tp);
        if(tabuSize<currentSize)
            pairList.pop_front();
    }

    void reduceLife()
    {
        int frontReduce = 0;
        for(TabuPair tp:pairList)
        {
            if(tp.ttl <= 1)
            {
                frontReduce++;
            }
            tp.ttl--;
        }
        while(frontReduce > 0)
        {
            pairList.pop_front();
            frontReduce--;
        }
    }

    bool searchForPair(int from,int to)
    {
        for(TabuPair tp:pairList)
        {
            if(tp.from == from && tp.to == to)
                return true;
        }
        return false;
    }
};


#endif //TSP_3_STRUCTURES_H
