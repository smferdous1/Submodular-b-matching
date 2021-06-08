#include "matching.h"
#include <iostream>

using std::cout;
using std::endl;

bool cmpbyFirst(const std::pair<VAL_T,NODE_T> &T1,const std::pair<VAL_T,NODE_T> &T2)
{

    return T1.first > T2.first;
}

void simpleLoadBal(Loads &L, NODE_T nMachine, NODE_T cV[], NODE_T bV[],float alpha, std::vector<VAL_T> &cW, SUM_T &totalWeight, NODE_T &matchingSize)
{
    //sort Loads array w.r.t to ntasks
    sort(L.loadList.begin(),L.loadList.end(),[](Load a, Load b)
            {
                return a.nTasks > b.nTasks;
            }
    );
    
    cout<<"sorting completede..."<<endl; 
    //create a pq with nMachine size.
    std::vector< std::pair<VAL_T,NODE_T> > pq;
    for(NODE_T i=0;i<nMachine;i++)
    {
        pq.push_back(std::make_pair(0,i)); 
        //L.loadList[i].rank = i;
        cW[i] = 0;
        cV[i] = 0;
    } 
    std::make_heap(pq.begin(),pq.end(),cmpbyFirst);
    cout<<"heap creation completed..."<<endl;
    
    //scan over the loadList array in high to low and place it in the least load availalble queue.
    for(EDGE_T i=0;i<L.nLoads;i++)
    {
        while(1)
        {
            
            auto top = pq.front();
            NODE_T mId = top.second;
            std::pop_heap(pq.begin(),pq.end(),cmpbyFirst);
            pq.pop_back();
            //cout<<"Top machine, mId: "<<top.second<<" total load: "<<cW[mId]<<endl;
            //VAL_T topMargGain = pow(cW[mId]+L.loadList[i].nTasks, alpha) - pow(cW[mId], alpha); 
            if(cV[mId] < bV[mId] )
            {
                L.loadList[i].rank =mId;
                cV[mId]++;
                top.first = top.first + L.loadList[i].nTasks;
                cW[mId] = cW[mId] + L.loadList[i].nTasks;
                if(cV[mId] < bV[mId])
                {
                    pq.push_back(top); 
                    std::push_heap(pq.begin(),pq.end(),cmpbyFirst);
                    
                }
                cout<<"Load "<<i<<": "<<L.loadList[i].nTasks<<" assigned machine "<<mId<<": "<<cW[mId]<<endl;
                break;
            }
        } 
    }

}
