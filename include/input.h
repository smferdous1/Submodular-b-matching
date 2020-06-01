#ifndef INPUT_H
#define INPUT_H
#include <string>
#include "graph.h"

class Input
{
  std::string fileName;
  public:
  void readMtx(std::string ,LightGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  void readMtxBpt(std::string ,BptGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  void readMtxBptAsNonBpt(std::string ,LightGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  //default constructor
  
  void writeMtx(std::string, LightGraph &);
  void readBinBpt(std::string , BptGraph &);
  void wrtBinBpt(std::string , BptGraph &);
  void readBinGen(std::string , LightGraph &);
  void wrtBinGen(std::string , LightGraph &);
  void readGraphGen(std::string ,LightGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  void readGraphBpt(std::string ,BptGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  void wrtGraphGen(std::string, LightGraph&);
  void wrtGraphBpt(std::string, BptGraph&);
  Input()
  {
  
  }
  
  Input(std::string givenFile)
  {
    fileName = givenFile;
  }
};


#endif
