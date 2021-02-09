#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<fstream>
#include "graph.h"
#include "matching.h"
#include "input.h"
#include "utility.h"

using namespace std;
int main(int argc,char *argv[])
{

    if(argc != 5)
    {
       cout<< "usage: ./progr_name filename outfile alpha nMachine"<<endl; 
       exit(1);
    }
    Input graphInput;
    
    std::string fileName = argv[1];
    std::string outfile = argv[2];
    float alpha = atof(argv[3]);
    int nMachine = atoi(argv[4]);


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

    currentTime(startTime);
    submodularGreedybMatching(G,cV,bV,alpha,nPartition,cW,outPut,totalWeight,matchingSize); 
    //localLazyGreedy(G,cV,bV,alpha,nPartition,cW,outPut,totalWeight,matchingSize); 
    currentTime(endTime);
    totalTime = totalTime + endTime-startTime;

    //open the output file and write down the allocation
    ofstream write_match;
    write_match.open(outfile);
    write_match<<"# of jobs: "<<nRow<<endl;
    write_match<<"# of machines: "<<nMachine<<endl;
    write_match<<"#job_id,machine_id,load"<<endl;
    for(NODE_T i =0;i<outPut.size();i++)
    {
        //job" "machine
        write_match<<outPut[i].e.u<<","<<outPut[i].e.v-nRow<<","<<outPut[i].weight<<endl;

    }

    //cout << std::fixed<<std::setprecision(0)<<totalWeight<< " "<<matchingSize<<" "<<b*n/2-matchingSize<<" ";
    //cout<< n <<","<<m<<","<<std::fixed<<std::setprecision(5)<<totalWeight<< ","<<matchingSize<<","<<b*n/2-matchingSize<<",";

    cout<< std::setprecision(4)<<2.0*matchingSize/n<<","<<totalTime<<endl;

    return 0;
}
