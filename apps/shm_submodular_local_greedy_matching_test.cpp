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
       cout<< "usage: ./progr_name filename b nrun alpha nt"<<endl; 
       exit(1);
    }
    Input graphInput;
    
    std::string fileName = argv[1];
    int b = atoi(argv[2]);
    int nRun = atoi(argv[3]);
    float alpha = atof(argv[4]);
    int nt = atoi(argv[5]);


    LightGraph G;
    graphInput.readMtx(fileName,G,0,1,5245213,1,5);

    NODE_T n;
    EDGE_T m;

    n = G.numberOfNodes();
    m = G.numberOfEdges();

    //cout<<n<<" "<<m<<endl;

    /*for(EDGE_T i=0;i<2*m;i++)
      {
      cout<<G.A[i]<<endl;
      }*/


    WeightEdgeList outPut;
    NODE_T *cV = new NODE_T[n];
    //weights
    SUM_T totalWeight;
    SUM_T linWeight;
    NODE_T matchingSize;

    //timeer
    double startTime,endTime;
    double mainTime;

    double totalTime = 0;
    double totalMainTime = 0;
    int nPartition = 1;

    for(int i=0;i<nRun;i++)
    {
        //currentTime(startTime);
        startTime = omp_get_wtime();
        shmLocalLazyGreedy(G,cV,b,alpha,nPartition,outPut,totalWeight,linWeight, matchingSize,mainTime,1,nt); 
        currentTime(endTime);
        endTime = omp_get_wtime();
        totalTime = totalTime + endTime-startTime;
        totalMainTime = totalMainTime + mainTime;
    }

    //cout << std::fixed<<std::setprecision(0)<<totalWeight<< " "<<matchingSize<<" "<<b*n/2-matchingSize<<" ";
    cout  <<nt<<","<<std::fixed<<std::setprecision(5)<<totalWeight<< ","<<linWeight<< ","<<matchingSize<<","<<b*n/2-matchingSize<<",";

    cout<< std::setprecision(4)<<2.0*matchingSize/n<<","<<totalTime/nRun<<","<<totalMainTime/nRun<<endl;

    return 0;
}
