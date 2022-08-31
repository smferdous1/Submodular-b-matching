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
    if(argc != 7)
    {
        std::cout<<"./program filename b nrun alpha write outfile"<<std::endl;
        exit(1); 
    }

    Input graphInput;

    std::string fileName = argv[1];
    int b = atoi(argv[2]);
    int nRun = atoi(argv[3]);
    float alpha = atof(argv[4]);
    int write = atoi(argv[5]);
    std::string outfile = argv[6];


    LightGraph G;
    graphInput.readMtx(fileName,G);

    //cout<<"read successful"<<endl;
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
    NODE_T *bV = new NODE_T[n];

    for(NODE_T i=0;i<n;i++)
        bV[i] = b;
    //weights
    SUM_T totalWeight;
    NODE_T matchingSize;
    std::vector<VAL_T> cW(n);

    //timeer
    double startTime,endTime;


    double totalTime = 0;
    int nPartition = 1;
    for(int i=0;i<nRun;i++)
    {
        currentTime(startTime);
        submodularGreedybMatching(G,cV,bV,alpha,nPartition,cW,outPut,totalWeight,matchingSize); 
        currentTime(endTime);
        totalTime = totalTime + endTime-startTime;
    }

    cout<< n <<","<<m<<","<<std::fixed<<std::setprecision(5)<<totalWeight<< ","<<matchingSize<<","<<b*n/2-matchingSize<<",";

    cout<< std::setprecision(4)<<2.0*matchingSize/n<<","<<totalTime/nRun<<endl;

    if(write == 1)
    {
        graphInput.writeMtx(outfile,n,outPut); 
    }

    return 0;
}
