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
#include "utility.h"

struct timeval tz;
struct timezone tx;


//comparator function to make min heap
//extracted and modified from https://www.geeksforgeeks.org/stdmake_heap-c-stl/ 
bool minHeapCompare(const WeightEdge& a,const WeightEdge& b) { 
  return a.weight>b.weight; 
} 

bool maxHeapCompare(const RefEdge& a,const RefEdge& b) { 
  return a.we.weight<b.we.weight; 
}

bool ascendOrder(const std::pair <EDGE_T,VAL_T> &a, const std::pair <EDGE_T,VAL_T> &b)
{
  return (a.second < b.second);
}

bool descendOrder(const std::pair <EDGE_T,VAL_T> &a, const std::pair <EDGE_T,VAL_T> &b)
{
  return (a.second > b.second);
}


//sort an edgelist with respect to its weight. The functin takes an array pointer where it 
//will pu the sorted order
void pairSort(EDGE_T sortedOrder[],WeightEdgeList &edgeList,int order)
{
  EDGE_T m = edgeList.size();

  //holding index to the array to be sorted
  for(EDGE_T i=0;i<m;i++)
  {
    sortedOrder[i] = i;
  }
  std::vector<std::pair<EDGE_T,VAL_T> > pairedEdge;
  pairedEdge.clear();
  pairedEdge.reserve(m);

  //making the pair
  for(EDGE_T i=0;i<m;i++)
  {
    pairedEdge.push_back(std::make_pair(sortedOrder[i],edgeList[i].weight));
  }
  
  if(order==0)
  {
    std::sort(pairedEdge.begin(),pairedEdge.end(),ascendOrder);
  }
  else if(order = 1)
  {
    std::sort(pairedEdge.begin(),pairedEdge.end(),descendOrder);
  }

  //copy back to sortedOrder array
  for(EDGE_T i =0;i<m;i++)
  {
    sortedOrder[i] = pairedEdge[i].first;
  }
}


//overloaded method for pairsort
void pairSort(EDGE_T sortedOrder[],WeightList &weightList,int order)
{
  EDGE_T m = weightList.size();

  //holding index to the array to be sorted
  for(EDGE_T i=0;i<m;i++)
  {
    sortedOrder[i] = i;
  }
  std::vector<std::pair<EDGE_T,VAL_T> > pairedEdge;
  pairedEdge.clear();
  pairedEdge.reserve(m);

  //making the pair
  for(EDGE_T i=0;i<m;i++)
  {
    pairedEdge.push_back(std::make_pair(sortedOrder[i],weightList[i]));
  }
  
  if(order==0)
  {
    std::sort(pairedEdge.begin(),pairedEdge.end(),ascendOrder);
  }
  else if(order = 1)
  {
    std::sort(pairedEdge.begin(),pairedEdge.end(),descendOrder);
  }

  //copy back to sortedOrder array
  for(EDGE_T i =0;i<m;i++)
  {
    sortedOrder[i] = pairedEdge[i].first;
  }
}
//calculate degree of each vertex v
NODE_T calculateAndDegreek(LightGraph &G, NODE_T dV[], NODE_T k)
{
  NODE_T n = G.numberOfNodes();
  NODE_T count = 0;
  for(NODE_T i=0;i<n;i++)
  {
    dV[i] = G.IA[i+1]-G.IA[i]; 
    if(G.IA[i+1]-G.IA[i]==k) count++; 
  }
  return count;
}

/*This function takes u the vertex id for which we calcualte the kth smallest edge.
 * This function return the index to the adjacency list of that vertex for which the 
 * kth smallest edge was found
 */
RefEdge findCheapestkEdge(NODE_T u, std::vector<RefEdge> &AdjList,NODE_T k)
{
  std::vector<RefEdge> temp_max_heap;
  temp_max_heap.reserve(k);

  for(NODE_T i=0;i<k;i++)
  {
    temp_max_heap.push_back(AdjList[i]);
  }

  //make heap
  make_heap(temp_max_heap.begin(),temp_max_heap.end(),maxHeapCompare);

  for(NODE_T i=k;i<AdjList.size();i++)
  {
    RefEdge re = AdjList[i];

    if(re.we.weight<temp_max_heap.front().we.weight)
    {
      //pop the top;
         
      std::pop_heap(temp_max_heap.begin(),temp_max_heap.end(),maxHeapCompare);
      temp_max_heap.pop_back();  
      //push re
      //
      temp_max_heap.push_back(re);
      std::push_heap(temp_max_heap.begin(),temp_max_heap.end(),maxHeapCompare); 
    }
  }
  RefEdge top_edge = temp_max_heap.front();
  temp_max_heap.clear();

  return top_edge;

}



//finding cheapest edge incident per vertex
void findCheapestEdges( LightGraph &G, WeightEdgeList &edgeList, EDGE_T mu[])
{
  NODE_T n = G.numberOfNodes();

  VAL_T *temp_mu = new VAL_T[n];
  //initialize mu to very large number
  for(NODE_T i=0;i<n;i++)
  {
    mu[i] = -1;
    temp_mu[i] = VERY_LARGE_WEIGHT;
  }

  //scan throgh the edge list and update mu[v]
  EDGE_T m = edgeList.size();
  for(EDGE_T i=0;i<m;i++)
  {
    WeightEdge we = edgeList[i];

    if(we.weight < temp_mu[we.e.u])
    {
      temp_mu[we.e.u] = we.weight;
      mu[we.e.u] = i;

    }
    if(we.weight < temp_mu[we.e.v])
    {
      
      temp_mu[we.e.v] = we.weight;
      mu[we.e.v] = i;

    }
  }
   
  delete []temp_mu;
}



//finding the k(v)th cheapest edge incident per vertex
/*The algorithm is for each vertex we create a max heap of size 
 * k(v). We iterate through the adjacency list and add the edges 
 * in max heap of size k. The top edge is the kth cheapest edge
 */
/*void findCheapestbEdges( LightGraph &G, WeightEdgeList &edgeList, NODE_T kV[], EDGE_T mu[])
{
  NODE_T n = G.numberOfNodes();

  //VAL_T *temp_mu = new VAL_T[n];
  //initialize mu to very large number
  for(NODE_T i=0;i<n;i++)
  {
    mu[i] = -1;
    //temp_mu[i] = VERY_LARGE_WEIGHT;
  }

  //scan throgh the G.IA,G.JA and G.RA and create adjacency list for each vertex
  std::vector<RefEdge> AdjList[n];
  for(NODE_T i=0;i<n;i++)
  {
    if(kV[i]>0)
    {
      AdjList[i].reserve(G.IA[i+1]-G.IA[i]);
      for(EDGE_T j=G.IA[i];j<G.IA[i+1];j++)
      {
        RefEdge re;
        re.we = edgeList[G.RA[j]];
        re.ref = G.RA[j];
        AdjList[i].push_back(re);
      }
      RefEdge kth_edge = findCheapestkEdge(i,AdjList[i],kV[i]);
      mu[i] = kth_edge.ref;
      AdjList[i].clear();
    }
  }

}*/

//using n_th element
void findCheapestbEdges( LightGraph &G, WeightEdgeList &edgeList, NODE_T kV[], EDGE_T mu[])
{
  NODE_T n = G.numberOfNodes();

  //VAL_T *temp_mu = new VAL_T[n];
  //initialize mu to very large number
  for(NODE_T i=0;i<n;i++)
  {
    mu[i] = -1;
    //temp_mu[i] = VERY_LARGE_WEIGHT;
  }

  //scan throgh the G.IA,G.JA and G.RA and create adjacency list for each vertex
  std::vector<RefEdge> AdjList[n];
  for(NODE_T i=0;i<n;i++)
  {
    if(kV[i]>0)
    {
      AdjList[i].reserve(G.IA[i+1]-G.IA[i]);
      for(EDGE_T j=G.IA[i];j<G.IA[i+1];j++)
      {
        RefEdge re;
        std::cout<<G.RA[j]<<std::endl;
        re.we = edgeList[G.RA[j]];
        re.ref = G.RA[j];
        AdjList[i].push_back(re);
      }
      //RefEdge kth_edge = findCheapestkEdge(i,AdjList[i],kV[i]);
      std::nth_element(AdjList[i].begin(),AdjList[i].begin()+kV[i]-1,AdjList[i].end(),maxHeapCompare);
      mu[i] = AdjList[i][kV[i]-1].ref;
      AdjList[i].clear();
    }
  }

}

/*
//mu is refernce to G.A
void findCheapestEdges( LightGraph &G, EDGE_T mu[])
{
  NODE_T n = G.numberOfNodes();

  VAL_T *temp_mu = new VAL_T[n];
  //initialize mu to very large number
  for(NODE_T i=0;i<n;i++)
  {
    mu[i] = -1;
    temp_mu[i] = VERY_LARGE_WEIGHT;
  }

  //scan throgh the edge list and update mu[v]
  EDGE_T m = edgeList.size();
  
  for(NODE_T i =0;i<n;i++)
  {
    mu[i] = -1;
    minm = VERY_LARGE_WEIGHT;

    for(EDGE_T j=IA[i];j<IA[i+1];j++)
    {
      if(G.A[j]<minm)
      {
        minm = G.A[j]
      }
    }
  }
  
  
  for(EDGE_T i=0;i<m;i++)
  {
    WeightEdge we = edgeList[i];

    if(we.weight < temp_mu[we.e.u])
    {
      temp_mu[we.e.u] = we.weight;
      mu[we.e.u] = i;

    }
    if(we.weight < temp_mu[we.e.v])
    {
      
      temp_mu[we.e.v] = we.weight;
      mu[we.e.v] = i;

    }
  }
   
  delete []temp_mu;
}
*/
//finding current time
void currentTime(double &nowTime)
{
  gettimeofday(&tz,&tx);
  nowTime = (double) tz.tv_sec+(double) tz.tv_usec/1000000.0;
}
