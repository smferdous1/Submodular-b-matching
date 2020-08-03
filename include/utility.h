#ifndef UTILITY_H
#define UTILITY_H
#include "graph.h"
#include<sys/time.h>
#include <algorithm>
#include <math.h>
#include <omp.h>
#define VERY_LARGE_WEIGHT 99999


bool minHeapCompare(const WeightEdge&,const WeightEdge&);

bool ascendOrder(const std::pair <EDGE_T,VAL_T> &, const std::pair <EDGE_T,VAL_T> &);


bool descendOrder(const std::pair <EDGE_T,VAL_T> &, const std::pair <EDGE_T,VAL_T> &);

void pairSort(EDGE_T [],WeightEdgeList &,int=0);
void pairSort(EDGE_T [],WeightList &,int=0);

NODE_T calculateAndDegreek(LightGraph &, NODE_T [],NODE_T );
void findCheapestEdges(LightGraph &, WeightEdgeList &, EDGE_T []);

void findCheapestbEdges(LightGraph &, WeightEdgeList &, NODE_T [],EDGE_T []);


void currentTime(double &);

#endif
