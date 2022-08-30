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
    int nMachine = atoi(argv[3]);
    float alpha = 0.5;


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
