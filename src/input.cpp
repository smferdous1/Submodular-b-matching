#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include "input.h"

#define DEBUG 1

bool static fileTypeCheck(std::string fn, std::string extension)
{
    if(fn.substr(fn.find_last_of(".") + 1) == extension) {
        return true;
    } else {
        return false;
    }
}

void Input::readGraphGen(std::string fileName, LightGraph &G,int add, int is_random,long seed, VAL_T minW, VAL_T maxW)
{
    if(fileTypeCheck(fileName,"mtx")==true)
    {
        readMtx(fileName,G,add,is_random,seed,minW,maxW);
    }
    else if(fileTypeCheck(fileName,"bin") == true)
    {
        readBinGen(fileName,G);
    }
    else
    {
        std::cout<<"file must be either mtx or bin."<<std::endl;
        std::exit(1);
    }
}

void Input::readGraphBpt(std::string fileName, BptGraph &G,int is_random,long seed, VAL_T minW, VAL_T maxW)
{
    if(fileTypeCheck(fileName,"mtx")==true)
    {
        readMtxBpt(fileName,G,is_random,seed,minW,maxW);
    }
    else if(fileTypeCheck(fileName,"bin") == true)
    {
        readBinBpt(fileName,G);
    }
    else
    {
        std::cout<<"file must be either mtx or bin."<<std::endl;
        std::exit(1);
    }
}

void Input::wrtGraphGen(std::string fileName, LightGraph &G)
{
    if(fileTypeCheck(fileName,"mtx")==true)
    {
        writeMtx(fileName,G);
    }
    else if(fileTypeCheck(fileName,"bin") == true)
    {
        wrtBinGen(fileName,G);
    }
    else
    {
        std::cout<<"file must be either mtx or bin."<<std::endl;
        std::exit(1);
    }
}

void Input::wrtGraphBpt(std::string fileName, BptGraph &G)
{
    if(fileTypeCheck(fileName,"bin") == true)
    {
        wrtBinBpt(fileName,G);
    }
    else
    {
        std::cout<<"file must be bin."<<std::endl;
        std::exit(1);
    }
}
//read symmetric graph
void Input::readMtx(std::string fileName, LightGraph &G,int add, int is_random,long seed, VAL_T minW, VAL_T maxW)
{
    //std::cout<<"start to read"<<std::endl;
    srand(seed);
    if(fileTypeCheck(fileName,"mtx")==false)
    {
        std::cout << "file type is not mtx"<<std::endl;
        std::exit(1);
    }
    std::ifstream fileread(fileName.c_str());
    if(fileread.is_open()==false)
    {
        std::cout << "No file named "<<fileName<<std::endl;
        std::exit(1);
    }
    //Ignore header and comments
    while (fileread.peek() == '%') fileread.ignore(2048, '\n');

    NODE_T nrow,ncol;
    EDGE_T nnz;
    fileread >> nrow >> ncol >> nnz;
    NODE_T n;
    if(add == 1)
        n = nrow + ncol;
    else
        n = nrow;

    std::vector<std::vector<NODE_T> > adjList;
    std::vector<std::vector<VAL_T> > adjWeight;
    adjList.resize(n);
    adjWeight.resize(n);

    NODE_T u;
    NODE_T v;
    VAL_T weight;
    EDGE_T nEdge = 0;
    for(EDGE_T i=0;i<nnz;i++)
    {
        fileread >> v >> u >> weight;
        v--;
        u--;
        if(u<v)
        {
            if(is_random==1)
            {
                weight =  minW + ((maxW-minW)*((VAL_T)rand()/(RAND_MAX)));
            }
            if(weight < 0)
                weight = (-1)*weight;
            //std::cout<<weight<<std::endl;
            adjList[u].push_back(v);
            adjWeight[u].push_back(weight);

            //reverse edge
            adjList[v].push_back(u);
            adjWeight[v].push_back(weight);
            nEdge++;
        }
    }
    G.setNumberEdges(nEdge);
    G.setNumberNodes(n);
    //would be helpful if the graph is bipartite but read as non-bpt
    G.setNumberNodesRow(nrow);
    G.setNumberNodesCol(ncol);

    G.A = new VAL_T[2*nEdge+5];
    G.IA = new NODE_T[n+5];
    G.JA = new NODE_T[2*nEdge];

    //second pass. Build the three arrays
    G.IA[0] = 0;
    EDGE_T k=0;
    for(NODE_T i=0;i<n;i++)
    {
        
        for(NODE_T j=0;j<adjList[i].size();j++)
        {

            G.A[k] = adjWeight[i][j];
            G.JA[k] = adjList[i][j];
            k++;
        }
        G.IA[i+1]= G.IA[i] + adjList[i].size();
        /*adjList[i].resize(0);
        adjList[i].shrink_to_fit();
        adjWeight[i].resize(0);
        adjWeight[i].shrink_to_fit();*/

        adjList[i].clear();
        std::vector<NODE_T>().swap(adjList[i]);

        adjWeight[i].clear();
        std::vector<VAL_T>().swap(adjWeight[i]);

    }
    ///std::cout<<"exiting.."<<std::endl;

    //delete the vectors
    /*for(NODE_T i=0;i<nrow;i++) 
    {
        adjList[i].clear();
        std::vector<NODE_T>().swap(adjList[i]);

        adjWeight[i].clear();
        std::vector<VAL_T>().swap(adjWeight[i]);

    }*/
    //close the file
    fileread.close();

}

void Input::writeMtx(std::string fileName, LightGraph &G)
{
    if(fileTypeCheck(fileName,"mtx")==false)
    {
        std::cout << "file type is not mtx"<<std::endl;
        std::exit(1);
    }
    std::ofstream myfile(fileName.c_str());

    if(myfile.is_open())
    {
        myfile << "%%MatrixMarket matrix coordinate real symmetric"<<std::endl; 
        myfile << G.numberOfNodes()<<" "<<G.numberOfNodes()<<" "<<G.numberOfEdges()<<std::endl;

        for(NODE_T i=0;i<G.numberOfNodes();i++)
        {
            for(EDGE_T j = G.IA[i];j<G.IA[i+1];j++)
            {
                if(i<G.JA[j])
                    myfile << G.JA[j]+1 << " "<< i+1 << " "<<G.A[j]<<std::endl;
            }
        }
        myfile.close();
    }
    else
        std::cout<<"unable to open file"<<std::endl;
}

//read non-symmetric graph
void Input::readMtxBpt(std::string fileName, BptGraph &G,int is_random,long seed, VAL_T minW, VAL_T maxW)
{
    srand(seed);

    if(fileTypeCheck(fileName,"mtx")==false)
    {
        std::cout << "file type is not mtx"<<std::endl;
        std::exit(1);
    }
    std::ifstream fileread(fileName.c_str());
    if(fileread.is_open()==false)
    {
        std::cout << "No file named "<<fileName<<std::endl;
        std::exit(1);
    }
    //Ignore header and comments
    while (fileread.peek() == '%') fileread.ignore(2048, '\n');

    NODE_T nrow,ncol;
    EDGE_T nnz;
    fileread >> nrow >> ncol >> nnz;
    if(DEBUG==1)
    {
        std::cout<<nrow<<" "<<ncol<<" "<<nnz<<std::endl;
    }


    //std::vector<NODE_T> adjList[nrow];
    //std::vector<VAL_T> adjWeight[nrow];
    std::vector<std::vector<NODE_T> > adjList(nrow);
    std::vector<std::vector<VAL_T> > adjWeight(nrow);


    NODE_T u;
    NODE_T v;
    VAL_T weight;
    EDGE_T nEdge = 0;
    for(EDGE_T i=0;i<nnz;i++)
    {
        fileread >> u >> v >> weight;
        //fileread >> u >> v;
        //weight = 1.0;
        v--;
        u--;
        if(DEBUG==2)
        {
            std::cout<<u<<" "<<v<<" "<<weight<<std::endl;
        }
        if(weight>0)
        {
            if(is_random==1)
            {
                weight =  minW + (int)((maxW-minW)*((double)rand()/(RAND_MAX)));
            }
            //std::cout<<weight<<std::endl;
            adjList[u].push_back(v);
            adjWeight[u].push_back(weight);
            nEdge++;
        }
    }
    G.setNumberEdges(nEdge);
    G.setNumberNodes(nrow,ncol);

    G.A = new VAL_T[nEdge+5];
    G.IA = new NODE_T[nrow+5];
    G.JA = new NODE_T[nEdge];

    //second pass. Build the three arrays
    G.IA[0] = 0;
    EDGE_T k=0;
    for(NODE_T i=0;i<nrow;i++)
    {
        for(NODE_T j=0;j<adjList[i].size();j++)
        {

            G.A[k] = adjWeight[i][j];
            G.JA[k] = adjList[i][j];
            k++;
        }
        G.IA[i+1]= G.IA[i] + adjList[i].size();
    }

    //delete the vectors
    for(NODE_T i=0;i<nrow;i++) 
    {
        adjList[i].clear();
        std::vector<NODE_T>().swap(adjList[i]);

        adjWeight[i].clear();
        std::vector<VAL_T>().swap(adjWeight[i]);

    }
    fileread.close();

}

void Input::readBinGen(std::string fileName, LightGraph &G)
{
    std::ifstream inf;
    NODE_T nA,nB;
    EDGE_T m;

    if(fileTypeCheck(fileName,"bin")==false)
    {
        std::cout << "file type is not bin"<<std::endl;
        std::exit(1);
    }
    inf.open(fileName.c_str(),std::ios::in|std::ios::binary);
    if(inf.is_open()!=1) {
        std::cout<<"Can not open file "<<fileName<<std::endl;
        exit(1);                                
    }
    inf.read((char*)&nA, sizeof(NODE_T));
    inf.read((char*)&nB, sizeof(NODE_T));
    inf.read((char*)&m, sizeof(EDGE_T));
    //of.write((char*)&max, sizeof(unsigned int));

    G.A = new VAL_T[2*m];
    G.IA = new NODE_T[nA+1];
    G.JA = new NODE_T[2*m];

    G.setNumberEdges(m);
    G.setNumberNodes(nA);

    inf.read((char*)&G.IA[0], sizeof(NODE_T) * (nA+1));
    inf.read((char*)&G.JA[0], sizeof(NODE_T) * 2*m);
    inf.read((char*)&G.A[0], sizeof(VAL_T) * 2*m);
    inf.close();

}

void Input::wrtBinGen(std::string fileName, LightGraph &G)
{
    std::ofstream of;
    NODE_T nA = G.numberOfNodes();
    EDGE_T m = G.numberOfEdges();

    if(fileTypeCheck(fileName,"bin")==false)
    {
        std::cout << "file type is not bin"<<std::endl;
        std::exit(1);
    }
    of.open(fileName.c_str(),std::ios::out|std::ios::binary);
    if(of.is_open()!=1) {
        std::cout<<"Can not open file "<<fileName<<std::endl;
        exit(1);                                
    }

    of.write((char*)&nA, sizeof(NODE_T));
    of.write((char*)&nA, sizeof(NODE_T));
    of.write((char*)&m, sizeof(EDGE_T));
    //of.write((char*)&max, sizeof(unsigned int));
    of.write((char*)&G.IA[0], sizeof(NODE_T) * (nA+1));
    of.write((char*)&G.JA[0], sizeof(NODE_T) *2*m);
    of.write((char*)&G.A[0], sizeof(VAL_T) * 2*m);
    of.close();

}
void Input::readBinBpt(std::string fileName, BptGraph &G)
{
    std::ifstream inf;
    NODE_T nA,nB;
    EDGE_T m;

    if(fileTypeCheck(fileName,"bin")==false)
    {
        std::cout << "file type is not bin"<<std::endl;
        std::exit(1);
    }
    inf.open(fileName.c_str(),std::ios::in|std::ios::binary);
    if(inf.is_open()!=1) {
        std::cout<<"Can not open file "<<fileName<<std::endl;
        exit(1);                                
    }

    inf.read((char*)&nA, sizeof(NODE_T));
    inf.read((char*)&nB, sizeof(NODE_T));
    inf.read((char*)&m, sizeof(EDGE_T));
    //of.write((char*)&max, sizeof(unsigned int));

    G.A = new VAL_T[m];
    G.IA = new NODE_T[nA+1];
    G.JA = new NODE_T[m];

    G.setNumberEdges(m);
    G.setNumberNodes(nA,nB);

    inf.read((char*)&G.IA[0], sizeof(NODE_T) * (nA+1));
    inf.read((char*)&G.JA[0], sizeof(NODE_T) * m);
    inf.read((char*)&G.A[0], sizeof(VAL_T) * m);
    inf.close();

}

void Input::wrtBinBpt(std::string fileName, BptGraph &G)
{
    std::ofstream of;
    NODE_T nA = G.numberOfNodesA();
    NODE_T nB = G.numberOfNodesB();
    EDGE_T m = G.numberOfEdges();


    if(fileTypeCheck(fileName,"bin")==false)
    {
        std::cout << "file type is not bin"<<std::endl;
        std::exit(1);
    }
    of.open(fileName.c_str(),std::ios::out|std::ios::binary);
    of.open(fileName.c_str(),std::ios::out|std::ios::binary);
    if(of.is_open()!=1) {
        std::cout<<"Can not open file "<<fileName<<std::endl;
        exit(1);                                
    }
    of.write((char*)&nA, sizeof(NODE_T));
    of.write((char*)&nB, sizeof(NODE_T));
    of.write((char*)&m, sizeof(EDGE_T));
    //of.write((char*)&max, sizeof(unsigned int));
    of.write((char*)&G.IA[0], sizeof(NODE_T) * (nA+1));
    of.write((char*)&G.JA[0], sizeof(NODE_T) * m);
    of.write((char*)&G.A[0], sizeof(VAL_T) * m);
    of.close();

}

//read non-symmetric graph
void Input::readMtxBptAsNonBpt(std::string fileName, LightGraph &G,int is_random,long seed, VAL_T minW, VAL_T maxW)
{
  //std::cout<<"start to read"<<std::endl;
  srand(seed);
  std::ifstream fileread(fileName.c_str());
  if(fileread.is_open()==false)
  {
    std::cout << "No file named "<<fileName<<std::endl;
    std::exit(1);
  }
  //Ignore header and comments
  while (fileread.peek() == '%') fileread.ignore(2048, '\n');
  
  NODE_T nrow,ncol;
  EDGE_T nnz;
  fileread >> nrow >> ncol >> nnz;
  NODE_T n = nrow + ncol;

  std::vector<std::vector<NODE_T> > adjList(n);
  std::vector<std::vector<VAL_T> > adjWeight(n);

  NODE_T u;
  NODE_T v;
  VAL_T weight;
  EDGE_T nEdge = 0;
  for(EDGE_T i=0;i<nnz;i++)
  {
    fileread >> u >> v >> weight;
    v = v + nrow;
    v--;
    u--;
  
    if (weight<0) weight = weight*(-1); 
    if(is_random==1)
    {
      weight =  minW + ((maxW-minW)*((double)rand()/(RAND_MAX)));
    }
    //std::cout<<weight<<std::endl;
    adjList[u].push_back(v);
    adjWeight[u].push_back(weight);

    //reverse edge
    //adjList[v].push_back(u);
    //adjWeight[v].push_back(weight);
    nEdge++;
    
  }
  G.setNumberEdges(nEdge);
  G.setNumberNodes(n);

  G.A = new VAL_T[nEdge+5];
  G.IA = new NODE_T[n+5];
  G.JA = new NODE_T[nEdge];
  std::cout<<"ok"<<std::endl;
  //second pass. Build the three arrays
  G.IA[0] = 0;
  EDGE_T k=0;
  for(NODE_T i=0;i<n;i++)
  {
      for(NODE_T j=0;j<adjList[i].size();j++)
      {
          
          G.A[k] = adjWeight[i][j];
          G.JA[k] = adjList[i][j];
          k++;
      }
      G.IA[i+1]= G.IA[i] + adjList[i].size();
  }
  
  //delete the vectors
  for(NODE_T i=0;i<n;i++) 
  {
    adjList[i].clear();
    std::vector<NODE_T>().swap(adjList[i]);

    adjWeight[i].clear();
    std::vector<VAL_T>().swap(adjWeight[i]);

  }

}
