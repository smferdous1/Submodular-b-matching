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

bool cmpbyFirst(const std::pair<VAL_T,WeightEdge> &T1,const std::pair<VAL_T,WeightEdge> &T2)
{
    if(T1.first == T2.first)
        return T1.second.e.v < T2.second.e.v;
    return T1.first < T2.first;
}

void submodularGreedyMatching(LightGraph &G, NODE_T cV[], float alpha, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize)
{
  
  NODE_T n = G.numberOfNodes();
  EDGE_T m = G.numberOfEdges();
  std::cout<<n<<" "<<m<<std::endl;
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
  /*std::vector<std::pair<VAL_T,int> >tmpSrtEdge;
  tmpSrtEdge.reserve(m);
  for(EDGE_T i=0;i<m;i++)
  {
    tmpSrtEdge.push_back(std::make_pair(edgeList[i].weight,i));
  }
  std::sort(tmpSrtEdge.begin(),tmpSrtEdge.end());
*/
  int nGroups;
  clusterEdges(n,edgeList,nGroups);
  std::cout<<nGroups<<std::endl;
  //The priority queue is a pair of <marginal gain, Edge>
  std::vector<std::pair<VAL_T,WeightEdge> >pq;
  pq.reserve(m);
  for(EDGE_T i=0;i<m;i++)
  {
    int index = i;
    NODE_T u = edgeList[index].e.u;
    NODE_T v = edgeList[index].e.v;
    VAL_T w = edgeList[index].weight;
    //edgeList[index].groupId = m-i-1;
    //std::cout<<edgeList[index].groupId<<std::endl;
    pq.push_back(std::make_pair(pow(w,alpha),edgeList[index]));
  }
  //create the heap
  std::make_heap(pq.begin(),pq.end(),cmpbyFirst);
  
  std::vector<VAL_T> cW;
  cW.reserve(nGroups+10);
  for(int i=0;i<nGroups+10;i++)
  {
    cW.push_back(0);
  }

  matching.clear();
  
  while(!pq.empty())
  {
    auto top = pq[0];
    std::pop_heap(pq.begin(),pq.end(),cmpbyFirst);
    pq.pop_back();
    
    NODE_T u = top.second.e.u;
    NODE_T v = top.second.e.v;
    VAL_T w = top.second.weight;
    int index = top.second.groupId;
    //VAL_T topMargGain = pow(cW[index /nPartition]+w,alpha)-pow(cW[index / nPartition],alpha) + pow(cW[index / nPartition]+w,alpha)-pow(cW[index / nPartition],alpha);
    VAL_T topMargGain = pow(cW[index]+w,alpha)-pow(cW[index],alpha);
    

    if(cV[u]<1 && cV[v]<1 )
    {
      if(pq.empty() || topMargGain>=pq[0].first)
      {
        matching.push_back(top.second);
        cV[u]++;
        cV[v]++;
        matchingSize++;
        totalWeight = totalWeight + top.second.weight;
        cW[index] += top.second.weight;
      }
      else if(topMargGain<pq[0].first)
      {
        top.first = topMargGain;
        pq.push_back(top);
        std::push_heap(pq.begin(), pq.end(),cmpbyFirst);
      }
    }


  }

}
