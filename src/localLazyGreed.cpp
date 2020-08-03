#include "matching.h"
#include <iostream>

using std::cout;
using std::endl;
bool cmpbyFirst(const std::pair<VAL_T,WeightEdge> &T1,const std::pair<VAL_T,WeightEdge> &T2)
{
    if(T1.first == T2.first)
        return T1.second.e.v > T2.second.e.v;
        //return T1.second.id < T2.second.id;
    return T1.first < T2.first;
}

void localLazyGreedy(LightGraph &G, NODE_T cV[], int b,float alpha, int nPartition, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum)
{

    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();
    std::cout<<n<<" "<<m<<std::endl;
    std::vector<VAL_T> cW(n);
    //zeroing out cV
    //
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
        cW[i] = 0.0;
    }
    //WeightEdgeList edgeList;
    //edgeList.clear();
    //edgeList.reserve(m);

    //G.createEdgeList(edgeList);

    //Creating The priority queue is for each vertex: a pair of <marginal gain, Edge>
    std::vector<std::pair<VAL_T,WeightEdge> >pq[n];
    for(NODE_T i=0;i<n;i++)
    {
        for(EDGE_T j=G.IA[i];j<G.IA[i+1];j++)
        {
            NODE_T u = i;
            NODE_T v = G.JA[j]; 
            VAL_T w = G.A[j];

            if(u<v)
            {
                WeightEdge we ={{u,v},w,0,j,0};
                WeightEdge we1 ={{v,u},w,0,j,0};
                pq[u].push_back(std::make_pair(2*pow(w,alpha),we));
                pq[v].push_back(std::make_pair(2*pow(w,alpha),we1));

            }
        }

    }

    for(NODE_T i=0;i<n;i++)
    {
        std::make_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
    }

    matching.clear();

    bool terminate = false;
    int itn = 1;
    while(!terminate)
    {
        terminate = true;
        for(NODE_T i=0;i<n;i++)
        {
            if(cV[i]<b)
            {
                while(pq[i].empty() == false)
                {
                     
                     auto top = pq[i].front().second;
                     std::pop_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
                     pq[i].pop_back();

                     NODE_T u = top.e.u;
                     NODE_T v = top.e.v;
                     VAL_T w = top.weight;
                    

                     if(pq[v].empty()==true || cV[v]>=b) continue;

                     VAL_T topMargGain = pow(cW[u ]+w,alpha)-pow(cW[u ],alpha) + pow(cW[v ]+w,alpha)-pow(cW[v ],alpha);

                     if(pq[u].empty() || topMargGain >= pq[u].front().first)
                     {
                        pq[u].push_back(std::make_pair(topMargGain,top));
                        std::push_heap(pq[u].begin(),pq[u].end(),cmpbyFirst);
                        break;

                     }
                     else if(topMargGain < pq[u].front().first)
                     {
                        pq[u].push_back(std::make_pair(topMargGain,top));
                        std::push_heap(pq[u].begin(),pq[u].end(),cmpbyFirst);

                     }
                }

                 
            } 
        }
        for(NODE_T i=0;i<n;i++)
        {
            if(pq[i].empty() == false && cV[i]<b)
            {
                WeightEdge top = pq[i].front().second;
                NODE_T u = i;
                NODE_T v = top.e.v;

                if(v > u && cV[v]<b  )
                {
                    //cout<<u<< " "<< v<<" "<<top.weight<<endl;

                    WeightEdge topV = pq[v].front().second;
                    //cout<<v<< " "<< topV.e.v<<" "<<topV.weight<<endl;
                    if(topV.e.v == u)
                    {
                        cV[u]++;
                        cV[v]++; 
                        matching.push_back(top);
                        terminate = false;
                        matchingSize++;
                        
                        std::pop_heap(pq[u].begin(),pq[u].end(),cmpbyFirst);
                        pq[u].pop_back();
                        
                        std::pop_heap(pq[v].begin(),pq[v].end(),cmpbyFirst);
                        pq[v].pop_back();

                        //totalWeight = totalWeight + top.weight;
                        //you have to add the marginal gain not the actual weight
                        totalWeight = totalWeight + pq[i].front().first;
                        cW[u] += top.weight;
                        cW[v] += top.weight;
                    }
                } 
            } 
        } 
        //cout<<"Iteration: "<<itn<<" "<< matchingSize<<endl;
        itn++;
    } 

    /*


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
            else if(topMargGain<pq[0].first)
            {
                top.first = topMargGain;
                pq.push_back(top);
                std::push_heap(pq.begin(), pq.end(),cmpbyFirst);
            }
        }

    }
    */
}
