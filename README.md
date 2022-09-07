# Parallel Local Lazy Greedy Algorithm for submodular $b$-matching
This is the companion codebase of the publication:

>S M Ferdous, Alex Pothen, Arif Khan, Ajay Panyala, and Mahantesh Halappanavar, “A parallel approximation algorithm 
for submodular b-matching,” in Proceedings of the First SIAM Conference on Applied and Computational Discrete Algorithms (ACDA),
SIAM, 2021. DOI: 10.1137/1.9781611976830.5.


Here is the [arXiv Link](https://arxiv.org/abs/2107.05793) of the publication.
## Abstract

We design new serial and parallel approximation algorithms for computing a maximum weight $b$-matching in an edge-weighted graph with a submodular objective function. This problem is NP-hard; the new algorithms have an approximation ratio $1/3$, and are relaxations of the Greedy algorithm that rely only on local information in the graph, making them parallelizable. We have designed and implemented Local Lazy Greedy algorithms for serial and parallel computers. We have applied the approximate submodular 
$b$-matching algorithm to assign tasks to processors in computing Fock matrices in quantum chemistry on parallel computers. The assignment seeks to reduce the run time by balancing the computational load on the processors and bounding the number of messages each processor sends. 
We show that the new assignment of tasks to processors provides a four-fold speedup over the currently used assignment in the NWChemEx software on $8000$ processors on the Summit supercomputer at Oak Ridge National Lab.

# Installation and Usages
The default compiler is GNU. Other compilers may also be used using standard CMAKE options. This software requires OpenMP support. 

## Building Steps


 - clone the repository to a local directory (say submodular-matching)
```
git clone https://github.com/smferdous1/Submodular-b-matching.git submodular-matching
```
- move to the local repository
```
cd submodular-matching
```
- create a "build" directory and move to the directory 
```
mkdir build && cd build
```
- run cmake to the upper directory
```
cmake ..
```
- run make
```
make
```

## Usages
This software package implements the serial and parallel local lazy greedy algorithm for maximizing submodular functions subject to b-matching constraints. A b-matching is a
subgraph where each vertex has degree at most an integer $b$. We implement here a particular submodular function as follows.
 
>$Z = \sum_{v \in V} \left(\sum_{e \in \delta(v)}{W(e)x(e)}\right)^\alpha.$
Here $x(e)$ is the characteristic function of the matching, i.e., $x(e) = 1$ if the edge, $e$ belongs to the b-matching. $\alpha$ is between 0 to 1 and controls the concavity. If $\alpha = 1$, the function becomes linear.

The "src" directory contains necessary library functions for the lazy greedy, serial and shared memory local lazy greedy, and load balancing functions. The "tests" directory contains the test and application files.

### Lazy Greedy
>src/submodularGreedybMatching.cpp

### Serial Local Lazy Greedy
>src/localLazyGreed.cpp

### Parallel Local Lazy Greedy
>src/shmLocalLazyGreedy.cpp

### Application: Load balancing Fock matrix construction
One application of submodular b-matching is to generate load balancing assignment of blocks of Fock matrices to the multiprocessors. We provide two implementations of such assignments. In "tests/load_balance_test.cpp" files, we use the default lazy greedy to generate the assignments. Since the input graph here is a complete graph, this implementation is slow. Hence, we provide a direct implementation of the load balancing assignment that generates the same matching as using the lazy greedy in "src/loadBalSimple.cpp" and the application file in"tests/load_balance_test_simple.cpp".
