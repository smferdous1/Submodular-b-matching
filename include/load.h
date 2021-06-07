#pragma once

#include "graph.h"

struct Load
{
    NODE_T rank;
    NODE_T s1;
    NODE_T s2;
    VAL_T nTasks;
};

class Loads
{
    public:
        EDGE_T nLoads;
        std::vector<Load> loadList;

        Loads()
        {
        
        }
         
        Loads(EDGE_T nloads)
        {
            nLoads = nloads; 
            loadList.reserve(nLoads);
        }
};

