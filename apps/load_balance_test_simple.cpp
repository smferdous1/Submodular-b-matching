#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<fstream>
#include "graph.h"
#include "matching.h"
#include "input.h"
#include "utility.h"
#include "load.h"

using namespace std;
int main(int argc,char *argv[])
{

    if(argc != 4)
    {
       cout<< "usage: ./progr_name filename outfile  nMachine"<<endl; 
       exit(1);
    }
    Input graphInput;
    
    std::string fileName = argv[1];
    std::string outfile = argv[2];
    float alpha = atof(argv[3]);
    int nMachine = atoi(argv[4]);


    Loads L;
    //do not add
    graphInput.readLoads(fileName,L);

    EDGE_T nLoads;
    
    nLoads = L.nLoads;
    NODE_T nRow = nLoads; 
    NODE_T nCol = nMachine;

    cout<<nLoads<<" "<<nRow<<" "<<nCol<<endl;

    /*for(EDGE_T i=0;i<nLoads;i++)
    {
      cout<<L.loadList[i].nTasks<<endl;
    }*/

    NODE_T *cV = new NODE_T[nMachine];
    NODE_T *bV = new NODE_T[nMachine];
    NODE_T b_prime = nRow/nCol;
    cout<<b_prime<<endl;
    NODE_T remainder = nRow % nCol;

    for(NODE_T i=0;i<nMachine;i++)
    {
            bV[i] = b_prime;
    }
    //settle the remainder
    for(NODE_T i=0;i<remainder;i++)
    {
        bV[i]++; 
    }
    //weights
    SUM_T totalWeight;
    NODE_T matchingSize;
    std::vector<VAL_T> cW(nMachine);
    //timer
    double startTime,endTime;


    double totalTime = 0;

    currentTime(startTime);
    simpleLoadBal(L,nMachine,cV,bV,alpha,cW,totalWeight,matchingSize); 
    //localLazyGreedy(G,cV,bV,alpha,nPartition,cW,outPut,totalWeight,matchingSize); 
    currentTime(endTime);
    totalTime = totalTime + endTime-startTime;

    //open the output file and write down the allocation
    ofstream write_match;
    write_match.open(outfile);
    write_match<<"%# of jobs: "<<nRow<<endl;
    write_match<<"%# of machines: "<<nMachine<<endl;
    write_match<<"%#job_id,machine_id,load"<<endl;
    for(NODE_T i =0;i<nLoads;i++)
    {
        //job" "machine
        write_match<<L.loadList[i].loadId<<","<<L.loadList[i].rank<<","<<L.loadList[i].nTasks<<endl;

    }
    /*
    for(NODE_T i=0;i<L.nLoads;i++)
    {
        cout<<L.loadList[i].rank<<endl; 
    }
    */
    //cout << std::fixed<<std::setprecision(0)<<totalWeight<< " "<<matchingSize<<" "<<b*n/2-matchingSize<<" ";
    //cout<< n <<","<<m<<","<<std::fixed<<std::setprecision(5)<<totalWeight<< ","<<matchingSize<<","<<b*n/2-matchingSize<<",";

    cout<< std::setprecision(4)<<2.0*matchingSize/nMachine<<","<<totalTime<<endl;

    return 0;
}
