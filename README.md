# Submodular Matching
To build:

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
