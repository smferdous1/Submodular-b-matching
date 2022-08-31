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


#pragma once

#include <string>

#include "graph.h"
#include "load.h"

class Input
{
  std::string fileName;
  public:
  void readMtx(std::string ,LightGraph& ,int=0,int=0, long int=0,VAL_T=0,VAL_T=0);
  void readMtxBpt(std::string ,BptGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  void readMtxBptAsNonBpt(std::string ,LightGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  //default constructor
  
  void writeMtx(std::string, LightGraph &);
  void writeMtx(std::string, NODE_T, WeightEdgeList &);
  void readBinBpt(std::string , BptGraph &);
  void wrtBinBpt(std::string , BptGraph &);
  void readBinGen(std::string , LightGraph &);
  void wrtBinGen(std::string , LightGraph &);
  void readGraphGen(std::string ,LightGraph& ,int =0,int=0, long int=0,VAL_T=0,VAL_T=0);
  void readGraphBpt(std::string ,BptGraph& ,int=0, long int=0,VAL_T=0,VAL_T=0);
  void wrtGraphGen(std::string, LightGraph&);
  void wrtGraphBpt(std::string, BptGraph&);
    void readLoads(std::string , Loads &);
  Input()
  {
  
  }
  
  Input(std::string givenFile)
  {
    fileName = givenFile;
  }
};
