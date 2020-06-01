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
  Input graphInput;
  
  std::string fileName = argv[1];
  int nRun = atoi(argv[2]);


  LightGraph G;
  graphInput.readMtxBptAsNonBpt(fileName,G,0,5245213,1,5);

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
  NODE_T matchingSize;

  //timeer
  double startTime,endTime;
  

  double totalTime = 0;
  for(int i=0;i<nRun;i++)
  {
    currentTime(startTime);
    greedyPerturbedMatching(G,cV,outPut,totalWeight,matchingSize); 
    currentTime(endTime);
    totalTime = totalTime + endTime-startTime;
  }
  cout << std::fixed<<std::setprecision(3)<<totalWeight<< " "<<matchingSize<<" ";

  cout<<std::setprecision(4)<<totalTime/nRun<<endl;

  

  
  return 0;
}
