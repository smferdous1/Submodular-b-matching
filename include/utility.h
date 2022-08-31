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

#include<sys/time.h>
#include <algorithm>
#include <math.h>
#include <omp.h>

#include "graph.h"

#define VERY_LARGE_WEIGHT 99999


bool minHeapCompare(const WeightEdge&,const WeightEdge&);

bool ascendOrder(const std::pair <EDGE_T,VAL_T> &, const std::pair <EDGE_T,VAL_T> &);


bool descendOrder(const std::pair <EDGE_T,VAL_T> &, const std::pair <EDGE_T,VAL_T> &);

void pairSort(EDGE_T [],WeightEdgeList &,int=0);
void pairSort(EDGE_T [],WeightList &,int=0);

NODE_T calculateAndDegreek(LightGraph &, NODE_T [],NODE_T );

void findCheapestEdges(LightGraph &, WeightEdgeList &, EDGE_T []);

void findCheapestbEdges(LightGraph &, WeightEdgeList &, NODE_T [],EDGE_T []);


void currentTime(double &);
