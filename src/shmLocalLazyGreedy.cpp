//Author: S M Ferdous
//CS@Purdue
//August 2020

#include "matching.h"
#include <iostream>


//for numerical comparing.
//if a and b are two real numbers comparison between them 
//is unstable so a == b may not provide the desired output
//hences EPS is declared
//#define EPS 1e-10 
#define EPS 0 
#define CHUNK 512
#define DEBUG 0

//declaring  this most used namespace entity
using std::cout;
using std::endl;
//A comparator function for heap
bool cmpbyFirst(const std::pair<VAL_T,WeightEdgeSim> &T1,const std::pair<VAL_T,WeightEdgeSim> &T2)
{
    if(T1.first == T2.first)
        return T1.second.v > T2.second.v;
    return T1.first < T2.first;
}
//This function takes a graph and run the locally lazy greedy algorithm for maximizing a submodular function subject to 
//b-matching constraints. The submodular function is of the form (\sum(W_{i,j})^\alpha; a class of concave polynomial
//This algorithm is equivalent to the classic lazy greedy and thus provides 1/3 approximation guarantee
void shmLocalLazyGreedy(LightGraph &G, NODE_T cV[], int b,float alpha, int nPartition, WeightEdgeList &matching, SUM_T &totalWeight, SUM_T &linWeight, NODE_T &matchingSize, double &mainTime, int maximum, int nt)
{
    omp_set_num_threads(nt);

    //get the number of nodes and edges in the graph
    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();

    //This vector is for cumulative weights for each vertex
    //would be useful for calculating marginal gain
    std::vector<VAL_T> cW(n);
    //A lock array defined on each vertex; assigned on stack
    omp_lock_t vlock[n];
    //to track the matched vertices in some iteration; assigned on stack
    bool exposed[n];
    //Creating The priority queue is for each vertex: a pair of <marginal gain, Edge>; 
    std::vector<std::vector<std::pair<VAL_T,WeightEdgeSim> > >pq(n);
    
    double t_zero = omp_get_wtime();
    //zeroing out cV and cW
    #pragma omp parallel 
    {
        #pragma omp for schedule(static,CHUNK) nowait
        for(NODE_T i =0;i<n;i++)
        {
            cV[i] = 0;
            cW[i] = 0.0;
        }
        #pragma omp for  schedule(static,CHUNK) nowait
        for(NODE_T i=0;i<n;i++)
        {
            exposed[i] =  false;
            omp_init_lock(&vlock[i]);
        }


    }
    //This loop does not parallelize at all so do it in serial
    for(NODE_T i=0;i<n;i++)
    {
        pq[i].reserve((G.IA[i+1] - G.IA[i]));
    }
    if(DEBUG==1)
        cout<<"Time for zeroing "<<omp_get_wtime()-t_zero<<endl;

    double t_init = omp_get_wtime();
    //initializing the  adjacency list
#pragma omp parallel for schedule(static,CHUNK)
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
                WeightEdgeSim we ={u,v,w};
                WeightEdgeSim we1 ={v,u,w};
                omp_set_lock(&vlock[u]);
                pq[u].push_back(std::make_pair(2*pow(w,alpha),we));
                omp_unset_lock(&vlock[u]);
                omp_set_lock(&vlock[v]);
                pq[v].push_back(std::make_pair(2*pow(w,alpha),we1));
                omp_unset_lock(&vlock[v]);

            }
        }

    }
    if(DEBUG==1)
        cout<<"Time for initializing "<<omp_get_wtime()-t_init<<endl;

    double t_heap = omp_get_wtime();
    //once the neighbor list is done create a heap by make_heap. 
    #pragma omp parallel for schedule(static,CHUNK)
    for(NODE_T i=0;i<n;i++)
    {
        std::make_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
    }
    
    if(DEBUG==1)
        cout<<"Time for heapify "<<omp_get_wtime()-t_heap<<endl;
    //for holding the matching edges
    matching.clear();

    //stopping condition checker
    bool terminate = false;
    //iteration couter
    int itn = 1;
    //initialize matchingsize and matching weight variable
    matchingSize = 0;
    totalWeight  = 0.0;
    linWeight = 0.0;
    
    double t_main = omp_get_wtime();
    //check stopping condition 
    while(!terminate)
    {
        //set stopping condition as true; if there are no work inside the loop it will exit in next iterations 
        //condition checking
        terminate = true;

        //PHASE1: Suitor Selection
        //-------------------------
        //Each vertex will select its best suitor according to its best marginal gained adjacent vertex.
        #pragma omp parallel //for schedule(static,CHUNK)
        {
            #pragma omp for schedule(static,CHUNK)
            for(NODE_T i=0;i<n;i++)
            {
                //If in PHASE 2 of previous iteration, some vertices were exposed i.e., were part of a matching, we
                //need to pop the top edge, since this edge is already part of matching and should be deleted
                if(exposed[i] == true)
                {
                    std::pop_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
                    pq[i].pop_back();
                }
                //resetting the exposed variable for the vertex
                exposed[i] = false;
                //check whether the vertex i is already saturated or not
                if(cV[i]<b)
                {
                    //check whether the queue is already empty in that case we could not
                    //add any edges incident of the vertex i
                    while(pq[i].empty() == false)
                    {
                        //save the top edge and then pop it
                        auto top = pq[i].front().second;
                        std::pop_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
                        pq[i].pop_back();


                        //The edge (u,v) with weight w
                        NODE_T u = top.u;
                        NODE_T v = top.v;
                        VAL_T w = top.weight;

                        //if the other end-points of (u,v) i.e., v is saturated then continue for
                        //the next vertex
                        //We can not test for emptyiness in v's queue. That might create race condition
                        if(cV[v]>=b ) continue;

                        //Calculate the marginal gain of the top edge
                        VAL_T topMargGain = pow(cW[u]+w,alpha)-pow(cW[u],alpha) + pow(cW[v]+w,alpha)-pow(cW[v],alpha);

                        //If after popping out this edge the queue of v is alreay empty or 
                        //the marginal gain of the updated popped top is greater than the marginal gain of 
                        //current top then we know the popped top is actually the current top, so push it back again to be 
                        //the top 
                        bool flag = false;

                        /*if(itn>65)
                          {
                          cout << topMargGain<<" "<<pq[u].front().first<<endl;
                          }*/
                        if(pq[u].empty() || (topMargGain - pq[u].front().first) >= EPS)
                        {
                            pq[u].push_back(std::make_pair(topMargGain,top));
                            std::push_heap(pq[u].begin(),pq[u].end(),cmpbyFirst);
                            break;

                        }
                        //else if the marginal gain is less then the current top's marginal gain then reinsert the updated popped
                        //top into appropriate position
                        else
                        {
                            pq[u].push_back(std::make_pair(topMargGain,top));
                            std::push_heap(pq[u].begin(),pq[u].end(),cmpbyFirst);

                        }
                    }

                } 
            }

            //cout<<"Iteration: "<<itn<<endl;
            //PHASE 2: Matching Phase
            //--------------------------
            //This phase is for matching the appropriate edge.
            #pragma omp for schedule(static,CHUNK) reduction(+:matchingSize) reduction(+:totalWeight) reduction(+:linWeight)
            for(NODE_T i=0;i<n;i++)
            {
                omp_set_lock(&vlock[i]);
                //whether this vertex is eligible
                if(pq[i].empty() == false && cV[i]<b)
                {
                    //query the top edge of (u)
                    WeightEdgeSim top = pq[i].front().second;
                    //marginal gain of top(u)
                    VAL_T margu = pq[i].front().first;
                    NODE_T u = i;
                    NODE_T v = top.v;

                    //whether the other end-point is saturated
                    if(u<v && cV[v]<b)
                    {
                        omp_set_lock(&vlock[v]);

                        if(cV[v]<b)
                        {
                            //the top edge of the other end point (v)
                            WeightEdgeSim topV = pq[v].front().second;
                            //marginal gain of the top(v)
                            VAL_T margv = pq[v].front().first;
                            //Whether one of u or v already matched in this iteration
                            //otherwise whether u and v both point to each other
                            //or both of the top's marginal gain is equivalent
                            if(exposed[u] == false && exposed[v] == false && (topV.v == u ))
                            {
                                //increment saturation counter of u and v
                                cV[v]++; 
                                cW[v] += top.weight;
                                exposed[v] = true;

                                //add the edge into the matching
                                //matching.push_back(top);
                                //The terminate is set to false because we have added edge in this iteration
                                if(terminate == true) terminate = false;

                                omp_unset_lock(&vlock[v]);

                                //The exposed variable is set to true as u and v are part of matching in this 
                                //iteration
                                cV[u]++;
                                exposed[u] = true;
                                cW[u] += top.weight;
                                omp_unset_lock(&vlock[u]);

                                //you have to add the marginal gain not the actual weight
                                totalWeight = totalWeight + margu;
                                linWeight = linWeight + top.weight;
                                //increment the matching size
                                matchingSize++;
                            }
                            else
                            {
                                omp_unset_lock(&vlock[v]); 
                                omp_unset_lock(&vlock[u]); 
                            }
                        }
                        else
                        {

                            omp_unset_lock(&vlock[v]); 
                            omp_unset_lock(&vlock[u]); 
                        }
                    } 
                    else
                    {
                        omp_unset_lock(&vlock[u]); 
                    }
                } 
                else
                {
                    omp_unset_lock(&vlock[i]); 
                }

            } 
        }
        //cout<<"Iteration: "<<itn<<" "<< matchingSize<<endl;
        //increment iteration counter
        itn++;
    } 
    
    mainTime = omp_get_wtime() -t_init;

    //freeing the reserved priority queue
    for(NODE_T i=0;i<n;i++)
    {
        pq[i].resize(0);
        pq[i].shrink_to_fit();
    }
}
