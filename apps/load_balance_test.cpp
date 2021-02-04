#include<iostream>
#include<iomanip>
#include<cstdlib>

#include "graph.h"
#include "matching.h"
#include "input.h"
#include "utility.h"

using namespace std;
int main(int argc,char *argv[])
{

    if(argc != 6)
    {
       cout<< "usage: ./progr_name filename b nrun alpha nMachine"<<endl; 
       exit(1);
    }
    Input graphInput;
    
    std::string fileName = argv[1];
    int b = atoi(argv[2]);
    int nRun = atoi(argv[3]);
    float alpha = atof(argv[4]);
    int nMachine = atoi(argv[5]);


    LightGraph G;
    //do not add
    graphInput.readMtx(fileName,G,0);

    NODE_T n;
    EDGE_T m;

    n = G.numberOfNodes();
    m = G.numberOfEdges();
    NODE_T nRow = n - nMachine;
    NODE_T nCol = nMachine;

    //cout<<n<<" "<<m<<" "<<nRow<<" "<<nCol<<endl;

    /*for(EDGE_T i=0;i<2*m;i++)
      {
      cout<<G.A[i]<<endl;
      }*/


    WeightEdgeList outPut;
    NODE_T *cV = new NODE_T[n];
    NODE_T *bV = new NODE_T[n];
    NODE_T b_prime = nRow/nCol;
    cout<<b_prime<<endl;
    NODE_T remainder = nRow % nCol;

    for(NODE_T i=0;i<n;i++)
    {
        if(i>=nRow)
            bV[i] = b_prime;
            //bV[i] = nRow;
        else 
            bV[i] = 1;
    }
    //settle the remainder
    for(NODE_T i=0;i<remainder;i++)
    {
        bV[nRow+i]++; 
    }
    //weights
    SUM_T totalWeight;
    NODE_T matchingSize;
    std::vector<VAL_T> cW(n);
    //timer
    double startTime,endTime;


    double totalTime = 0;
    int nPartition = 1;
    for(int i=0;i<nRun;i++)
    {
        currentTime(startTime);
        submodularGreedybMatching(G,cV,bV,alpha,nPartition,cW,outPut,totalWeight,matchingSize); 
        //localLazyGreedy(G,cV,bV,alpha,nPartition,cW,outPut,totalWeight,matchingSize); 
        currentTime(endTime);
        totalTime = totalTime + endTime-startTime;
    }
    for(NODE_T i=nRow;i<n;i++)
    {
        cout<<cW[i]<<endl; 
    }
    cout<<"%%%"<<endl;
    for(NODE_T i =0;i<outPut.size();i++)
    {
        //job" "machine
        cout<<outPut[i].e.u<<" "<<outPut[i].e.v<<endl;
    }

    //cout << std::fixed<<std::setprecision(0)<<totalWeight<< " "<<matchingSize<<" "<<b*n/2-matchingSize<<" ";
    //cout<< n <<","<<m<<","<<std::fixed<<std::setprecision(5)<<totalWeight<< ","<<matchingSize<<","<<b*n/2-matchingSize<<",";

    cout<< std::setprecision(4)<<2.0*matchingSize/n<<","<<totalTime/nRun<<endl;

    return 0;
}
