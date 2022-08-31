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

//for numerical comparing.
//if a and b are two real numbers comparison between them 
//is unstable so a == b may not provide the desired output
//hences EPS is declared
#define EPS 1e-10 

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
void localLazyGreedy(LightGraph &G, NODE_T cV[], NODE_T bV[],float alpha, int nPartition, std::vector<VAL_T> &cW, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum)
{

    //get the number of nodes and edges in the graph
    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();

    //This vector is for cumulative weights for each vertex
    //would be useful for calculating marginal gain
    //std::vector<VAL_T> cW(n);
    //to track the matched vertices in some iteration
    bool *exposed = new bool[n];
    //Creating The priority queue is for each vertex: a pair of <marginal gain, Edge>
    std::vector<std::vector<std::pair<VAL_T,WeightEdgeSim> > >pq(n);

    //zeroing out cV and cW
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
        cW[i] = 0.0;
        exposed[i] =  false;
        pq[i].reserve(G.IA[i+1] - G.IA[i]);
    }


    //initializing the  adjacency list
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
                pq[u].push_back(std::make_pair(2*pow(w,alpha),we));
                pq[v].push_back(std::make_pair(2*pow(w,alpha),we1));

            }
        }

    }
    //once the neighbor list is done create a heap by make_heap. 
    for(NODE_T i=0;i<n;i++)
    {
        std::make_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
    }

    //for holding the matching edges
    matching.clear();
    
    //stopping condition checker
    bool terminate = false;
    //iteration couter
    int itn = 1;
    //initialize matchingsize and matching weight variable
    matchingSize = 0;
    totalWeight  = 0.0;

    //check stopping condition 
    while(!terminate)
    {
        //set stopping condition as true; if there are no work inside the loop it will exit in next iterations 
        //condition checking
        terminate = true;

        //PHASE1: Suitor Selection
        //-------------------------
        //Each vertex will select its best suitor according to its best marginal gained adjacent vertex.
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
            if(cV[i]<bV[i])
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
                    
                    //if the other end-points of (u,v) i.e., v is saturated or the queue of v is empty then continue for
                    //the next vertex
                    if(pq[v].empty()==true || cV[v]>=bV[v] ) continue;
                    
                    //Calculate the marginal gain of the top edge
                    VAL_T topMargGain = pow(cW[u ]+w,alpha)-pow(cW[u ],alpha) + pow(cW[v ]+w,alpha)-pow(cW[v ],alpha);
                    
                    //If after popping out this edge the queue of v is alreay empty or 
                    //the marginal gain of the updated popped top is greater than the marginal gain of 
                    //current top then we know the popped top is actually the current top, so push it back again to be 
                    //the top 
                    if(pq[u].empty() || topMargGain >= pq[u].front().first)
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
        for(NODE_T i=0;i<n;i++)
        {
            //whether this vertex is eligible
            if(pq[i].empty() == false && cV[i]<bV[i])
            {
                //query the top edge of (u)
                WeightEdgeSim top = pq[i].front().second;
                //marginal gain of top(u)
                VAL_T margu = pq[i].front().first;
                NODE_T u = i;
                NODE_T v = top.v;
                
                //whether the other end-point is saturated
                if(u<v && cV[v]<bV[v])
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
                        //if they don't point to each other but the two top's marginal gain are equal we need to swap 
                        //the necessary edge to v's top. We will loop through v's heap and find out u and make it a top. 
                        //Note that We will always find such edge. Since v is u's top and v's top marginal gain is equal to u's top
                        //marginal gain
                        /*if(topV.v != u)
                        {
                            for(size_t k =0;k<pq[v].size();k++)
                            {
                                if(pq[v][k].second.v == u)
                                {
                                    auto tmp = pq[v][k].second;
                                    pq[v][k].second = pq[v][0].second;
                                    pq[v][0].second = tmp;
                                    break;
                                }
                            }
                        }*/
                        //increment saturation counter of u and v
                        cV[u]++;
                        cV[v]++; 

                        //add the edge into the matching
                        //matching.push_back(top);

                        //The terminate is set to false because we have added edge in this iteration
                        terminate = false;

                        //increment the matching size
                        matchingSize++;

                        //The exposed variable is set to true as u and v are part of matching in this 
                        //iteration
                        exposed[u] = true;
                        exposed[v] = true;

                        //you have to add the marginal gain not the actual weight
                        totalWeight = totalWeight + pq[i].front().first;
                        //for marginal gain calucation purpose
                        cW[u] += top.weight;
                        cW[v] += top.weight;
                    }
                } 
            } 
        } 
        cout<<"Iteration: "<<itn<<" "<< matchingSize<<endl;
        //increment iteration counter
        itn++;
    } 

}







//This function takes a graph and run the locally lazy greedy algorithm for maximizing a submodular function subject to 
//b-matching constraints. The submodular function is of the form (\sum(W_{i,j})^\alpha; a class of concave polynomial
//This algorithm is equivalent to the classic lazy greedy and thus provides 1/3 approximation guarantee
//This is the epsilon version
void localLazyGreedy(LightGraph &G, NODE_T cV[], NODE_T bV[],float alpha, int nPartition, std::vector<VAL_T> &cW, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,double epsilon,int maximum)
{

    //get the number of nodes and edges in the graph
    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();

    //This vector is for cumulative weights for each vertex
    //would be useful for calculating marginal gain
    //std::vector<VAL_T> cW(n);
    //to track the matched vertices in some iteration
    bool *exposed = new bool[n];
    //Creating The priority queue is for each vertex: a pair of <marginal gain, Edge>
    std::vector<std::vector<std::pair<VAL_T,WeightEdgeSim> > >pq(n);

    //zeroing out cV and cW
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
        cW[i] = 0.0;
        exposed[i] =  false;
        pq[i].reserve(G.IA[i+1] - G.IA[i]);
    }


    //initializing the  adjacency list
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
                pq[u].push_back(std::make_pair(2*pow(w,alpha),we));
                pq[v].push_back(std::make_pair(2*pow(w,alpha),we1));

            }
        }

    }
    //once the neighbor list is done create a heap by make_heap. 
    for(NODE_T i=0;i<n;i++)
    {
        std::make_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
    }

    //for holding the matching edges
    matching.clear();
    
    //stopping condition checker
    bool terminate = false;
    //iteration couter
    int itn = 1;
    //initialize matchingsize and matching weight variable
    matchingSize = 0;
    totalWeight  = 0.0;

    //check stopping condition 
    while(!terminate)
    {
        //set stopping condition as true; if there are no work inside the loop it will exit in next iterations 
        //condition checking
        terminate = true;

        //PHASE1: Suitor Selection
        //-------------------------
        //Each vertex will select its best suitor according to its best marginal gained adjacent vertex.
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
            if(cV[i]<bV[i])
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
                    
                    //if the other end-points of (u,v) i.e., v is saturated or the queue of v is empty then continue for
                    //the next vertex
                    if(pq[v].empty()==true || cV[v]>=bV[v] ) continue;
                    
                    //Calculate the marginal gain of the top edge
                    VAL_T topMargGain = pow(cW[u ]+w,alpha)-pow(cW[u ],alpha) + pow(cW[v ]+w,alpha)-pow(cW[v ],alpha);
                    
                    //If after popping out this edge the queue of v is alreay empty or 
                    //the marginal gain of the updated popped top is greater than the marginal gain of 
                    //current top then we know the popped top is actually the current top, so push it back again to be 
                    //the top 
                    if(pq[u].empty() || topMargGain >= epsilon*pq[u].front().first)
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
        for(NODE_T i=0;i<n;i++)
        {
            //whether this vertex is eligible
            if(pq[i].empty() == false && cV[i]<bV[i])
            {
                //query the top edge of (u)
                WeightEdgeSim top = pq[i].front().second;
                //marginal gain of top(u)
                VAL_T margu = pq[i].front().first;
                NODE_T u = i;
                NODE_T v = top.v;
                
                //whether the other end-point is saturated
                if(u<v && cV[v]<bV[v])
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
                        //if they don't point to each other but the two top's marginal gain are equal we need to swap 
                        //the necessary edge to v's top. We will loop through v's heap and find out u and make it a top. 
                        //Note that We will always find such edge. Since v is u's top and v's top marginal gain is equal to u's top
                        //marginal gain
                        /*if(topV.v != u)
                        {
                            for(size_t k =0;k<pq[v].size();k++)
                            {
                                if(pq[v][k].second.v == u)
                                {
                                    auto tmp = pq[v][k].second;
                                    pq[v][k].second = pq[v][0].second;
                                    pq[v][0].second = tmp;
                                    break;
                                }
                            }
                        }*/
                        //increment saturation counter of u and v
                        cV[u]++;
                        cV[v]++; 

                        //add the edge into the matching
                        //matching.push_back(top);

                        //The terminate is set to false because we have added edge in this iteration
                        terminate = false;

                        //increment the matching size
                        matchingSize++;

                        //The exposed variable is set to true as u and v are part of matching in this 
                        //iteration
                        exposed[u] = true;
                        exposed[v] = true;

                        //you have to add the marginal gain not the actual weight
                        totalWeight = totalWeight + pq[i].front().first;
                        //for marginal gain calucation purpose
                        cW[u] += top.weight;
                        cW[v] += top.weight;
                    }
                } 
            } 
        } 
        cout<<"Iteration: "<<itn<<" "<< matchingSize<<endl;
        //increment iteration counter
        itn++;
    } 

}
