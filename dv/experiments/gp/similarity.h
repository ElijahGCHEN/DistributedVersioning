#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <iostream>
#include "graph.h"

Graph* similize(const Graph* G,void (*simFunc)(const Vertex u, const Vertex v));



#endif