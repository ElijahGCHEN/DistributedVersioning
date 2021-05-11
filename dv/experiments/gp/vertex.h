#ifndef VERTEX_H
#define VERTEX_H

#include "metis.h"
#include <unordered_map>

using namespace std;

class Vertex{
    private:
        int id;
        idx_t weight;
        unordered_map<Vertex*,idx_t> adjacents;
        friend class Graph;
        friend class SimGraph;
    public:
        Vertex(int id, idx_t weight=1);
        Vertex(const Vertex& V);
        const unordered_map<Vertex*,idx_t>* getAdjMap();
        void addAdjacent(Vertex* newVertex, idx_t edgeWeight);
        const int getId();
        const idx_t getWeight();
        void printAdjacent() const;
        size_t getAdjSize();
        idx_t getEdgeWeight(Vertex* target) const;
        int removeEdge(Vertex*& v);
        // const unordered_map<idx_t,idx_t> getAdjList();
};

#endif