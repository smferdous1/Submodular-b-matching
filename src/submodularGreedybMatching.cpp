#include "matching.h"
#include <iostream>

bool cmpbyFirst(const std::pair<VAL_T,WeightEdge> &T1,const std::pair<VAL_T,WeightEdge> &T2)
{
    if(T1.first == T2.first)
    {
        if(T1.second.e.u == T2.second.e.u) 
            return T1.second.e.v > T2.second.e.v;
        return T1.second.e.u > T2.second.e.u;
    }
        //return T1.second.id < T2.second.id;
    return T1.first < T2.first;
}

void submodularGreedybMatching(LightGraph &G, NODE_T cV[], int b,float alpha, int nPartition, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum)
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

    //The priority queue is a pair of <marginal gain, Edge>
    std::vector<std::pair<VAL_T,WeightEdge> >pq;
    pq.reserve(m);
    for(EDGE_T i=0;i<m;i++)
    {
        NODE_T u = edgeList[i].e.u;
        NODE_T v = edgeList[i].e.v;
        VAL_T w = edgeList[i].weight;

        pq.push_back(std::make_pair(2*pow(w,alpha),edgeList[i]));
    }

    //create the heap
    std::make_heap(pq.begin(),pq.end(),cmpbyFirst);

    std::vector<VAL_T> cW;
    cW.reserve(n+10);
    for(int i=0;i<n;i++)
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

        VAL_T topMargGain = pow(cW[u ]+w,alpha)-pow(cW[u ],alpha) + pow(cW[v ]+w,alpha)-pow(cW[v ],alpha);

        if(cV[u]<b && cV[v]<b )
        {
            if(pq.empty() || topMargGain>=pq[0].first)
            {
                matching.push_back(top.second);
                cV[u]++;
                cV[v]++;
                matchingSize++;
                totalWeight = totalWeight + top.second.weight;
                cW[u ] += top.second.weight;
                cW[v ] += top.second.weight;
            }
            /*else if (topMargGain == pq[0].first)
            {
                if(u<pq[0].second.e.u || (u==pq[0].second.e.u && v < pq[0].second.e.v))
                {
                    
                    matching.push_back(top.second);
                    cV[u]++;
                    cV[v]++;
                    matchingSize++;
                    totalWeight = totalWeight + top.second.weight;
                    cW[u ] += top.second.weight;
                    cW[v ] += top.second.weight;
                     
                }
                else
                {
                    
                    top.first = topMargGain;
                    pq.push_back(top);
                    std::push_heap(pq.begin(), pq.end(),cmpbyFirst);
                }

            }*/
            else if(topMargGain<pq[0].first)
            {
                top.first = topMargGain;
                pq.push_back(top);
                std::push_heap(pq.begin(), pq.end(),cmpbyFirst);
            }
        }

    }
}
