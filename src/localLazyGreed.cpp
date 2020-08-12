//Author: S M Ferdous
//CS@Purdue
//August 2020

#include "matching.h"
#include <iostream>

//for numerical comparing.
//if a and b are two real numbers comparison between them 
//is unstable so a == b may not provide the desired output
//hences EPS is declared
#define EPS 1e-10 

//declaring  this most used namespace entity
using std::cout;
using std::endl;
//A comparator function for heap
bool cmpbyFirst(const std::pair<VAL_T,WeightEdge> &T1,const std::pair<VAL_T,WeightEdge> &T2)
{
    return T1.first < T2.first;
}
//This function takes a graph and run the locally lazy greedy algorithm for maximizing a submodular function subject to 
//b-matching constraints. The submodular function is of the form (\sum(W_{i,j})^\alpha; a class of concave polynomial
//This algorithm is equivalent to the classic lazy greedy and thus provides 1/3 approximation guarantee
void localLazyGreedy(LightGraph &G, NODE_T cV[], int b,float alpha, int nPartition, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum)
{

    //get the number of nodes and edges in the graph
    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();

    std::cout<<n<<" "<<m<<std::endl;
    //This vector is for cumulative weights for each vertex
    //would be useful for calculating marginal gain
    std::vector<VAL_T> cW(n);
    //to track the matched vertices in some iteration
    bool *exposed = new bool[n];

    //zeroing out cV and cW
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
        cW[i] = 0.0;
        exposed[i] =  false;
    }


    //Creating The priority queue is for each vertex: a pair of <marginal gain, Edge>
    std::vector<std::vector<std::pair<VAL_T,WeightEdge> > >pq(n);

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
                WeightEdge we ={{u,v},w,0,j,0};
                WeightEdge we1 ={{v,u},w,0,j,0};
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
                    NODE_T u = top.e.u;
                    NODE_T v = top.e.v;
                    VAL_T w = top.weight;
                    
                    //if the other end-points of (u,v) i.e., v is saturated or the queue of v is empty then continue for
                    //the next vertex
                    if(pq[v].empty()==true || cV[v]>=b) continue;
                    
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
                    else if(topMargGain < pq[u].front().first)
                    {
                        pq[u].push_back(std::make_pair(topMargGain,top));
                        std::push_heap(pq[u].begin(),pq[u].end(),cmpbyFirst);

                    }
                }


            } 
        }
        cout<<"Iteration: "<<itn<<endl;
        //PHASE 2: Matching Phase
        //--------------------------
        //This phase is for matching the appropriate edge.
        for(NODE_T i=0;i<n;i++)
        {
            //whether this vertex is eligible
            if(pq[i].empty() == false && cV[i]<b)
            {
                //query the top edge of (u)
                WeightEdge top = pq[i].front().second;
                //marginal gain of top(u)
                VAL_T margu = pq[i].front().first;
                NODE_T u = i;
                NODE_T v = top.e.v;
                
                //whether the other end-point is saturated
                if(cV[v]<b)
                {
                    //the top edge of the other end point (v)
                    WeightEdge topV = pq[v].front().second;
                    //marginal gain of the top(v)
                    VAL_T margv = pq[v].front().first;
                    //Whether one of u or v already matched in this iteration
                    //otherwise whether u and v both point to each other
                    //or both of the top's marginal gain is equivalent
                    if(exposed[u] == false && exposed[v] == false && (topV.e.v == u || abs(margu-margv) <= EPS))
                    {
                        //if they don't point to each other but the two top's marginal gain are equal we need to swap 
                        //the necessary edge to v's top. We will loop through v's heap and find out u and make it a top. 
                        //Note that We will always find such edge. Since v is u's top and v's top marginal gain is equal to u's top
                        //marginal gain
                        if(topV.e.v != u)
                        {
                            for(size_t k =0;k<pq[v].size();k++)
                            {
                                if(pq[v][k].second.e.v == u)
                                {
                                    auto tmp = pq[v][k].second;
                                    pq[v][k].second = pq[v][0].second;
                                    pq[v][0].second = tmp;
                                    break;
                                }
                            }
                        }
                        //increment saturation counter of u and v
                        cV[u]++;
                        cV[v]++; 

                        //add the edge into the matching
                        matching.push_back(top);

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
