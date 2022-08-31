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
    greedyMatching(G,cV,outPut,totalWeight,matchingSize); 
    currentTime(endTime);
    totalTime = totalTime + endTime-startTime;
  }
  
  cout << n<<"<"<<m<<","<<std::fixed<<std::setprecision(3)<<totalWeight<< ","<<matchingSize<<",";

  cout<<std::setprecision(4)<<totalTime/nRun<<endl;

  

  
  return 0;
}
