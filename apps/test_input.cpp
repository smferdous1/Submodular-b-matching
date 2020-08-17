#include<iostream>
#include<string>
#include "input.h"
#include "graph.h"


using namespace std;

int main(int argc, char *argv[])
{
    string infile = string(argv[1]);
    string outfile = string(argv[2]);
    
    Input graphInput;
    LightGraph G;
    graphInput.readMtx(infile,G,1,5245213,1,5);
    graphInput.writeMtx(outfile,G);
    return 0;
}
