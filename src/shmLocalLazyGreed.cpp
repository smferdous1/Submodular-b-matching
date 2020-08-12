#include "matching.h"
#include <iostream>

using std::cout;
using std::endl;
static bool cmpbyFirst(const std::pair<VAL_T,WeightEdge> &T1,const std::pair<VAL_T,WeightEdge> &T2)
{
    if(T1.first == T2.first)
        return T1.second.e.v > T2.second.e.v;
        //return T1.second.id < T2.second.id;
    return T1.first < T2.first;
}

void shmLocalLazyGreedy(LightGraph &G, NODE_T cV[], int b,float alpha, int nPartition, WeightEdgeList &matching, SUM_T &totalWeight, NODE_T &matchingSize,int maximum,int nt)
{
    omp_set_num_threads(nt);

    NODE_T n = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();
    std::cout<<n<<" "<<m<<std::endl;
    //std::vector<VAL_T> cW(n);
    VAL_T *cW = new VAL_T[n];
    //zeroing out cV
    //
    double sinit1 = omp_get_wtime();
    omp_lock_t vlock[n];
    #pragma omp parallel for schedule(static,512) 
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
        cW[i] = 0;
    }
    
    double einit1 = omp_get_wtime();
    cout<<"Init1: "<<einit1 - sinit1<<endl;
    //WeightEdgeList edgeList;
    //edgeList.clear();
    //edgeList.reserve(m);

    //G.createEdgeList(edgeList);

    //Creating The priority queue is for each vertex: a pair of <marginal gain, Edge>
    std::vector< std::vector<std::pair<VAL_T,WeightEdge> > >pq(n);
    for(NODE_T i =0;i<n;i++)
    {
        pq[i].reserve(2*(G.IA[i+1] - G.IA[i]));
        omp_init_lock(&vlock[i]);
    }
    double sinit = omp_get_wtime();
    int work[nt];
    #pragma omp parallel   /*default(none) shared(G,pq) firstprivate(n,alpha)*/
    {
        int tid = omp_get_thread_num();
        work[tid] = 0;
        double start = omp_get_wtime();
        #pragma omp for schedule(static,512) 
        for(NODE_T i=0;i<n;i++)
        {
            for(EDGE_T j=G.IA[i];j<G.IA[i+1];j++)
            {
                NODE_T u = i;
                NODE_T v = G.JA[j]; 
                VAL_T w = G.A[j];

                if(u<v)
                {

                    work[tid]++;
                    WeightEdge we ={{u,v},w,0,j,0};
                    WeightEdge we1 ={{v,u},w,0,j,0};

                    //#pragma omp critical
                    omp_set_lock(&vlock[u]);
                    pq[u].push_back(std::make_pair(2*pow(w,alpha),we));
                    omp_unset_lock(&vlock[u]);
                    omp_set_lock(&vlock[v]);
                    pq[v].push_back(std::make_pair(2*pow(w,alpha),we1));
                    omp_unset_lock(&vlock[v]);

                }
            }

        }
        cout<<omp_get_wtime() - start<<" "<<work[tid]<<endl;
        #pragma omp for schedule(static,512)
        for(NODE_T i=0;i<n;i++)
        {
            std::make_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
        }
    }

    double einit = omp_get_wtime();
    cout<<"Init2: "<<einit - sinit<<endl;

    matching.clear();

    bool *exposed = new bool[n];
    bool terminate = false;
    int itn = 1;
    matchingSize = 0;
    totalWeight = 0.0;
    while(!terminate)
    {
        double mstart = omp_get_wtime();
        terminate = true;
        #pragma omp parallel for schedule(static,512)
        for(NODE_T i=0;i<n;i++)
        {
            if(exposed[i] == true)
            {

                std::pop_heap(pq[i].begin(),pq[i].end(),cmpbyFirst);
                pq[i].pop_back();
            }
            exposed[i] = false;
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
        double mend = omp_get_wtime();
        cout<<"main1 "<<mend - mstart<<endl;
        double mstart1 = omp_get_wtime();
        #pragma omp parallel for reduction(+:matchingSize) reduction(+:totalWeight)
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
        double mend1 = omp_get_wtime();
        cout<<"main2 "<<mend1 - mstart1<<endl;
        //cout<<"Iteration: "<<itn<<" "<< matchingSize<<endl;
        itn++;
    } 
}
