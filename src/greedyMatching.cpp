/*
 * Copyright (C) 2022  Ferdous,S M <ferdous.csebuet@egmail.com>
 * Author: Ferdous,S M <ferdous.csebuet@egmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream>

#include "matching.h"
bool cmpbyFirst(const std::pair<VAL_T,EDGE_T> &T1,const std::pair<VAL_T,EDGE_T> &T2)
{
  return T1.first > T2.first;
}

// This function construct a greedy 1/2 approximate matching. This is a sorting based method
void greedyMatching(NODE_T n,WeightEdgeList &edgeList, WeightList &transformedWeight,NODE_T cV[], WeightEdgeList &matching, SUM_T &totalWeight,NODE_T &matchingSize,int maximum)
{
  //zeroing out cV
  //
  /*for(NODE_T i =0;i<n;i++)
  {
    cV[i] = 0;
  }*/

  EDGE_T m = edgeList.size();

  EDGE_T *sortedOrder = new EDGE_T[m];

  pairSort(sortedOrder,transformedWeight,maximum);

  //matching.clear();
  for(EDGE_T i=0;i<m;i++)
  {
    WeightEdge we = edgeList[sortedOrder[i]];
    NODE_T u = we.e.u;
    NODE_T v = we.e.v;

    if(cV[u]<1 && cV[v]<1)
    {
      matching.push_back(we);
      cV[u]++;
      cV[v]++;
      matchingSize++;
      totalWeight = totalWeight + we.weight;
    }
  }
  
  

}

void greedyMatching(NODE_T n,WeightEdgeList &edgeList, NODE_T cV[], WeightEdgeList &matching, SUM_T &totalWeight,NODE_T &matchingSize,int maximum)
{
  //zeroing out cV
  //
  for(NODE_T i =0;i<n;i++)
  {
    cV[i] = 0;
  }

  EDGE_T m = edgeList.size();

  EDGE_T *sortedOrder = new EDGE_T[m];

  pairSort(sortedOrder,edgeList,maximum);

  matching.clear();
  for(EDGE_T i=0;i<m;i++)
  {
    WeightEdge we = edgeList[sortedOrder[i]];
    NODE_T u = we.e.u;
    NODE_T v = we.e.v;

    if(cV[u]<1 && cV[v]<1)
    {
      matching.push_back(we);
      cV[u]++;
      cV[v]++;
      matchingSize++;
      totalWeight = totalWeight + we.weight;
    }
  }
  
  

}

void greedyMatching(LightGraph &G, NODE_T cV[], WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum)
{
  
  NODE_T n = G.numberOfNodes();
  EDGE_T m = G.numberOfEdges();
  //zeroing out cV
  std::cout<<m<<std::endl;
  for(NODE_T i =0;i<n;i++)
  {
    cV[i] = 0;
  }
  WeightEdgeList edgeList;
  edgeList.clear();
  edgeList.reserve(m);
  
  G.createEdgeList(edgeList);

  std::vector<std::pair<VAL_T,EDGE_T> >pertEdge;
  pertEdge.reserve(m);

  for(EDGE_T i=0;i<m;i++)
  {
    VAL_T w = edgeList[i].weight;

    pertEdge.push_back(std::make_pair(w,i));
  }
  std::sort(pertEdge.begin(), pertEdge.end(),cmpbyFirst);
  matching.clear();
  totalWeight = 0;
  for(EDGE_T i=0;i<m;i++)
  {
    WeightEdge we = edgeList[pertEdge[i].second];
    NODE_T u = we.e.u;
    NODE_T v = we.e.v;
    VAL_T w = we.weight;

    if(cV[u]<1 && cV[v]<1)
    {
      matching.push_back(we);
      cV[u]++;
      cV[v]++;
      totalWeight = totalWeight + w;
      matchingSize++;
    }
  }
  

}
void greedyPerturbedMatching(LightGraph &G, NODE_T cV[], WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum)
{
  
  NODE_T n = G.numberOfNodes();
  EDGE_T m = G.numberOfEdges();
  //zeroing out cV
  //
  for(NODE_T i =0;i<n;i++)
  {
    cV[i] = 0;
  }
  WeightEdgeList edgeList;
  edgeList.clear();
  edgeList.reserve(m);
  
  G.createEdgeList(edgeList);
  
  EDGE_T *sortedOrder = new EDGE_T[m];

  std::vector<std::pair<VAL_T,EDGE_T> >pertEdge;
  pertEdge.reserve(m);

  for(EDGE_T i=0;i<m;i++)
  {
    VAL_T w = edgeList[i].weight;
    VAL_T psi = 1-exp((double)rand()/(RAND_MAX)-1);
    //std::cout << psi<<std::endl;
    pertEdge.push_back(std::make_pair(w*psi,i));
  }
  std::sort(pertEdge.begin(), pertEdge.end(),cmpbyFirst);
  matching.clear();
  totalWeight = 0;
  for(EDGE_T i=0;i<m;i++)
  {
    int index = pertEdge[i].second;
    WeightEdge we = edgeList[index];
    NODE_T u = we.e.u;
    NODE_T v = we.e.v;
    VAL_T w = we.weight;
    
    //std::cout << pertEdge[i].first<<std::endl;
    if(cV[u]<1 && cV[v]<1)
    {
      matching.push_back(we);
      cV[u]++;
      cV[v]++;
      totalWeight = totalWeight + w;
      matchingSize++;
    }
  }
  
}
