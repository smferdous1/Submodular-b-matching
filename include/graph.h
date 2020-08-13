#ifndef GRAPH_H
#define GRAPH_H

#include<vector>
#include<algorithm>
//defining types. 
//
//Change this to float/double if the weight of the graph is real  
typedef float VAL_T;
//Change this to long if number of nodes does not fit into integer
typedef  long NODE_T;
//Change this to long if number of edges does not fit into integer
typedef  long EDGE_T;
typedef double SUM_T;

struct Edge
{
  NODE_T u;
  NODE_T v;
};


struct WeightEdge
{

  Edge e;
  VAL_T weight;
  bool flag;
  NODE_T id;
  int groupId;
};
//weight edge simple
struct WeightEdgeSim
{

  NODE_T u;
  NODE_T v;
  VAL_T weight;
};
struct RefEdge
{
  WeightEdge we;
  EDGE_T ref;
};
//adjacent edge
struct Adj
{
  //adjacent vertex id
  NODE_T adjV;
  VAL_T adjW;
  Adj(){}
  Adj(NODE_T adjVa,VAL_T adjWa)
    :adjV(adjVa),adjW(adjWa){}
  bool operator==(const Adj& adj) const
  { 
    return (this->adjV == adj.adjV); 
  } 
  bool operator<(const Adj& adj) const
  { 
    return (this->adjV < adj.adjV); 
  } 
};
//to hold list of edges.
typedef std::vector<Edge> EdgeList;
typedef std::vector<WeightEdge> WeightEdgeList;
typedef std::vector<WeightEdgeSim> WeightEdgeSimList;
typedef std::vector<VAL_T> WeightList;

typedef struct Adj Adjacent;

class LightGraph
{
  
  //number of nodes in the graph
  NODE_T n; 
  //number of edges in the graph
  EDGE_T m;
  
  public:  
  //default constructor
  LightGraph()
  {
    A = 0;
    IA = 0;
    JA = 0;
    RA = 0;
  }
  //constructor
  LightGraph(NODE_T n1, EDGE_T m1)
  {
    n = n1;
    m = m1;

    //allocate memory for A,IA and JA
    //graph is recorded as symmetric matrix. thats why we need 2*m
    A = new VAL_T[2*m];
    IA = new NODE_T[n+1];
    JA = new NODE_T[2*m];
    //RA is a reference array. Given a undirected graph as an edgeList. it create the references needed to go back and forth to the three arrays and edgeList.
    RA = new EDGE_T[2*m];
  }
  //delete the graph
  ~LightGraph()
  {
    n = 0;
    m = 0;
    if(A!=0) delete []A;
    if(IA!=0) delete []IA;
    if(JA!=0)delete []JA;
    //if(RA != 0) delete []RA;
  }
  
  
  //Graph is always in CSR format
  VAL_T *A;
  NODE_T *IA;
  NODE_T *JA;
  EDGE_T *RA;
  EDGE_T *invRA;
  void setNumberEdges(EDGE_T m1)
  {
    m = m1;
  }
  EDGE_T numberOfEdges()
  {
    return m;
  }
 

  void setNumberNodes(NODE_T n1)
  {
    n = n1;
  }
  NODE_T numberOfNodes()
  {
    return n;
  }
  
  //void createEdgeList(EdgeList &);
  //this create edgelist and if bool=1 it also create the G.RA array
  void createEdgeList(WeightEdgeList &,bool=0);
  void printGraph();
  void printGraph(NODE_T);
  void createRefArray();
  void createRefArray(int);
  void sortGraph();
  
};

class BptGraph
{
  //number of nodes in the graph
  NODE_T nA;
  NODE_T nB;
  //number of edges in the graph
  EDGE_T m;
  
  public:  
  //default constructor
  BptGraph()
  {
    A= 0;
    IA = 0;
    JA = 0;
  }
  //constructor
  BptGraph(NODE_T n1, NODE_T n2, EDGE_T m1)
  {
    nA = n1;
    nB = n2;
    m = m1;

    //allocate memory for A,IA and JA
    A = new VAL_T[m+5];
    IA = new NODE_T[nA+5];
    JA = new NODE_T[m];
    //RA is a reference array. Given a undirected graph as an edgeList. it create the references needed to go back and forth to the three arrays and edgeList.
    //RA = new EDGE_T[m];
  }
  //delete the graph
  ~BptGraph()
  {
    nA = 0;
    nB = 0;
    m = 0;
    //if(A!=0) delete []A;
    //if(IA!=0) delete []IA;
    //if(JA!=0)delete []JA;
  }
  
  
  //Graph is always in CSR format
  VAL_T *A;
  NODE_T *IA;
  NODE_T *JA;
  //EDGE_T *RA;
  void setNumberEdges(EDGE_T m1)
  {
    m = m1;
  }
  EDGE_T numberOfEdges()
  {
    return m;
  }
 

  void setNumberNodes(NODE_T n1,NODE_T n2)
  {
    nA = n1;
    nB = n2;
  }
  NODE_T numberOfNodesA()
  {
    return nA;
  }

  NODE_T numberOfNodesB()
  {
    return nB;
  }
  void printGraph();
  void sortGraph();
};




#endif
