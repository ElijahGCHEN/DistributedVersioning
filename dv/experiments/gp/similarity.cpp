#include "metis.h"
#include "similarity.h"
#include <iostream>
using namespace std;

Graph* similize(const Graph& G,void (*simFunc)(const Vertex u, const Vertex v)){
    Graph* simGraph=new Graph(G);
    
}