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

using std::cout;
using std::endl;

bool cmpbyFirst(const std::pair<VAL_T,WeightEdgeSim> &T1,const std::pair<VAL_T,WeightEdgeSim> &T2)
{
    if(T1.first == T2.first)
    {
        if(T1.second.u == T2.second.u)
           return T1.second.v > T2.second.v; 
        else if(T1.second.v == T2.second.v)
            return T1.second.u > T2.second.u;
    }

    return T1.first < T2.first;
}

void submodularGreedybMatching(LightGraph &G, NODE_T cV[], NODE_T bV[],float alpha,  std::vector<VAL_T> &cW,WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize)
{

    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();
    //std::cout<<n<<" "<<m<<std::endl;
    //zeroing out cV
    //
    //std::vector<VAL_T> cW(n);
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
        cW[i] = 0.0;
    }
    /*WeightEdgeSimList edgeList;
    edgeList.clear();
    edgeList.reserve(m);

    G.createEdgeList(edgeList);*/

    //The priority queue is a pair of <marginal gain, Edge>
    std::vector<std::pair<VAL_T,WeightEdgeSim> >pq;

    pq.reserve(m);
    /*for(EDGE_T i=0;i<m;i++)
    {
        NODE_T u = edgeList[i].e.u;
        NODE_T v = edgeList[i].e.v;
        VAL_T w = edgeList[i].weight;

        pq.push_back(std::make_pair(2*pow(w,alpha),edgeList[i]));
    }*/
    for(NODE_T i=0;i<n;i++)
    {
        for(EDGE_T j=G.IA[i];j<G.IA[i+1];j++)
        {
            NODE_T u = i;
            NODE_T v = G.JA[j]; 
            VAL_T w = G.A[j];

            if(u<v)
            {
                //create a weighted edge for (u,v) and (v,u) and insert it to
                //corresponding vector. j is the index of the common index of the edge
                //cout<<u<<" "<<v<<" "<<w<<endl;
                WeightEdgeSim we ={u,v,w};
                pq.push_back(std::make_pair(2*pow(w,alpha),we));

            }
        }

    }

    //create the heap
    std::make_heap(pq.begin(),pq.end(),cmpbyFirst);

    matching.clear();
    totalWeight = 0.0;
    matchingSize = 0;

    while(!pq.empty())
    {
        auto top = pq.front();
        std::pop_heap(pq.begin(),pq.end(),cmpbyFirst);
        pq.pop_back();

        NODE_T u = top.second.u;
        NODE_T v = top.second.v;
        VAL_T w = top.second.weight;

        VAL_T topMargGain = pow(cW[u ]+w,alpha)-pow(cW[u ],alpha) + pow(cW[v ]+w,alpha)-pow(cW[v ],alpha);

        if(cV[u]<bV[u] && cV[v]<bV[v] )
        {
            if(pq.empty() || topMargGain >= pq.front().first)
            {
                WeightEdge we;
                we.e.u = u;
                we.e.v = v;
                we.weight = w;
                matching.push_back(we);
                cV[u]++;
                cV[v]++;
                matchingSize++;
                //totalWeight = totalWeight + top.second.weight;
                totalWeight = totalWeight + top.first;
                cW[u ] += top.second.weight;
                cW[v ] += top.second.weight;
            }
            else
            {
                top.first = topMargGain;
                pq.push_back(top);
                std::push_heap(pq.begin(), pq.end(),cmpbyFirst);
            }
        }

    }
}
