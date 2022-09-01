# Parallel Local Lazy Greedy Algorithm for submodular $b$-matching
This is the companiion codebase of the following publication.
```
S M Ferdous, Alex Pothen, Arif Khan, Ajay Panyala, and Mahantesh Halappanavar, “A parallel approximation algorithm 
for submodular b-matching,” in Proceedings of the First SIAM Conference on Applied and Computational Discrete Algorithms (ACDA),
SIAM, 2021. doi: 10.1137/1.9781611976830.5.

```

## Abstract

We design  new serial and parallel approximation algorithms for computing a maximum weight $b$-matching in an edge-weighted graph with a submodular objective function. This problem is NP-hard; the new algorithms have approximation ratio $1/3$, and are relaxations of the Greedy algorithm that rely only on local information in the graph, making them parallelizable. We have designed and implemented Local Lazy Greedy algorithms for both serial and parallel computers. We have applied the approximate submodular 
$b$-matching algorithm to assign tasks to processors in the computation of Fock matrices in quantum chemistry on parallel computers. The assignment seeks to reduce the run time by  balancing the computational load  on the processors and bounding the number of messages that each processor  sends. 
We show that the new assignment of tasks to processors provides a four fold speedup over the currently used assignment in the NWChemEx software on $8000$ processors on the Summit supercomputer at Oak Ridge National Lab.

# Usage of the Software

## Building Steps

```
clone the repo
cd repo
mkdir build
cd build
cmake ..
make
```

All the executable files in build/apps directory.
The default compiler is gnu if you want to compile with intel or other compiler you can set the CMAKE_CXX_COMPILER varialbe at the time of cmake ..
That is,
```
cmake -DCMAKE_CXX_COMPILER="icpc" 
```
would compile everything with intel icpc.


**This project required OpenMP**
