#include <iostream>
#include<vector>
#include<algorithm>
#include "matching.h"

using std::vector;
using std::sort;
using std::cout;
using std::endl;

bool cmpbyFirst(const std::pair<VAL_T,EDGE_T> &T1,const std::pair<VAL_T,EDGE_T> &T2)
{
  return T1.first > T2.first;
}


void repeatedGreedyMatchingPhase(NODE_T n,WeightEdgeList &edgeListOrig, WeightEdgeList &edgeList,WeightEdgeList &edgeListNext, int groupId, WeightEdgeList &matching, SUM_T &totalWeight,NODE_T &matchingSize, int doSort)
{
    //zeroing out cV
    NODE_T *cV = new NODE_T[n];
    for(NODE_T i =0;i<n;i++)
    {
        cV[i] = 0;
    }

    EDGE_T m = edgeList.size();

  vector<std::pair<VAL_T,EDGE_T> > pairedEdges;
  pairedEdges.reserve(m);
  
  for(EDGE_T i=0;i<m;i++)
  {
    VAL_T w = edgeList[i].weight;

    pairedEdges.push_back(std::make_pair(w,edgeList[i].id));
  }
  if(doSort==1) std::sort(pairedEdges.begin(), pairedEdges.end(),cmpbyFirst);

  matching.clear();
  for(EDGE_T i=0;i<m;i++)
  {
    WeightEdge we = edgeListOrig[pairedEdges[i].second];
    NODE_T u = we.e.u;
    NODE_T v = we.e.v;

    if(groupId>2 || (cV[u]<1 && cV[v]<1))
    {
      matching.push_back(we);
      cV[u]++;
      cV[v]++;
      matchingSize++;
      edgeListOrig[pairedEdges[i].second].groupId = groupId;
      totalWeight = totalWeight + we.weight;
    }
    else
    {
      edgeListNext.push_back(we);
    }
  }
}


void clusterEdges(NODE_T n, WeightEdgeList &edgeList,int &nGroups)
{
    
  EDGE_T m = edgeList.size();

  WeightEdgeList edgeListCur,edgeListNext;
  edgeListNext.reserve(m);

  edgeListCur.reserve(m);
  //std::copy(edgeList.begin(),edgeList.end(),edgeListCur);
  edgeListCur = edgeList;

  WeightEdgeList matching;
  int group;
  for(group=0;;group++)
  {
    edgeListNext.clear();

    SUM_T totalWeight;
    NODE_T matchingSize;
    totalWeight = 0;
    matchingSize = 0;
    repeatedGreedyMatchingPhase(n,edgeList, edgeListCur,edgeListNext , group, matching, totalWeight,matchingSize,1-group);
    cout<<totalWeight<<" "<<matchingSize<<endl;
    if(edgeListNext.size()==0) break;

    edgeListCur.clear();
    //std::copy(edgeListNext.begin(),edgeListNext.end(),edgeListCur);
    edgeListCur = edgeListNext;
  }
  nGroups = group+1;
}
