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

#include "graph.h"
#include "utility.h"
#include "load.h"


void simpleLoadBal(Loads &L, NODE_T, NODE_T cV[], NODE_T bV[],float alpha, std::vector<VAL_T> &cW, SUM_T &totalWeight, NODE_T &matchingSize);

void greedyMatching(NODE_T ,WeightEdgeList &, WeightList &, NODE_T [], WeightEdgeList &, SUM_T &, NODE_T &,int=1);

void greedyMatching(NODE_T ,WeightEdgeList &, NODE_T [], WeightEdgeList &, SUM_T &, NODE_T &,int = 1);

void greedyMatching(LightGraph &, NODE_T [], WeightEdgeList &, SUM_T &, NODE_T &,int = 1);

void greedyPerturbedMatching(LightGraph &, NODE_T [], WeightEdgeList &, SUM_T &, NODE_T &,int = 1);

void submodularGreedyMatching(LightGraph &, NODE_T [], float, int, WeightEdgeList &, SUM_T &, NODE_T &,int = 1);

void submodularGreedybMatching(LightGraph &, NODE_T [], NODE_T [], float, int, std::vector<VAL_T> &, WeightEdgeList &, SUM_T &, NODE_T &,int = 1);
void localLazyGreedy(LightGraph &, NODE_T [], NODE_T [], float, int, std::vector<VAL_T> &, WeightEdgeList &, SUM_T &, NODE_T &,int = 1);
void localLazyGreedy(LightGraph &, NODE_T [], NODE_T [], float, int, std::vector<VAL_T> &, WeightEdgeList &, SUM_T &, NODE_T &, double epsilon = 1.0, int = 1);
void shmLocalLazyGreedy(LightGraph &, NODE_T [], int, float, int, WeightEdgeList &, SUM_T &, SUM_T &, NODE_T &,double &, int = 1,int = 1);

void clusterEdges(NODE_T, WeightEdgeList&,int &);
