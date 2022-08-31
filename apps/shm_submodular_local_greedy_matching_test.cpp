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
