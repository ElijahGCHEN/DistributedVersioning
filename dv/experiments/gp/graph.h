#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "metis.h"
#include <string>
#include "vertex.h"
using namespace std;
// class Edge{
//     private:
//         Vertex* start,end;
//         idx_t weight;
//     public:
//         Edge(int start, int end, idx_t weight);
//         Edge(Vertex start, Vertex end, idx_t weight);
// };

// class Vertex{
//     private:
//         int id;
//         idx_t weight;
//         unordered_map<Vertex*,idx_t> adjacents;
//         friend class Graph;
//         friend class SimGraph;
//     public:
//         Vertex(int id, idx_t weight=1);
//         Vertex(const Vertex& V);
//         const unordered_map<Vertex*,idx_t>* getAdjacents();
//         void addAdjacent(Vertex* newVertex, idx_t edgeWeight);
//         const int getId();
//         const idx_t getWeight();
//         void printAdjacent() const;
//         size_t getAdjSize();
//         idx_t getEdgeWeight(Vertex* target) const;
// };

class GraphRep{
    protected:
        bool isDirected;
        bool isWeightedNode;
        idx_t vertexCount;
        idx_t edgeCount;
        idx_t adjCount;
        idx_t* xadj;
        idx_t* adjncy;
        idx_t* vwgt;
        idx_t* adjwgt;
        void parseGraphFile(string fileName);
        
    public:
        GraphRep();
        GraphRep(std::string fileName);
        GraphRep(const GraphRep& G);
        GraphRep(vector<idx_t>& xadj, vector<idx_t>&adjncy,vector<idx_t>&adjwgt, vector<idx_t>&vwgt,
        idx_t& vertexCount,idx_t& edgeCount, bool isDirected, bool isWeightedNode);
        GraphRep(const idx_t& vertexCount,const idx_t& edgeCount,const idx_t& adjCount, bool isDirected, bool isWeightedNode,
        const idx_t* xadj, const idx_t* adjncy, const idx_t* adjwgt, const idx_t* vwgt);
        vector<GraphRep*>* partGraph(idx_t n,idx_t*& parts);
        virtual void print();
        idx_t getVertexCount() const;
        idx_t getEdgeCount() const;
        ~GraphRep();
};
class Graph: public GraphRep{
    protected:
        friend class Vertex;
        // friend class Edge;
        unordered_map<idx_t,Vertex*> vertices;
        // vector<Edge*> edges;
        void updateRep();
        // void updateXadj();
        // void updateAdjncy();
        // void updateAdjwgt();
        // void updateVwgt();
    public:
        Graph();
        Graph(string fileName);
        Graph(const Graph& G);
        // Graph(vector<Vertex*> vertices, bool isDirected, bool isWeightedNode);
        void print() override;
        int removeEdge(vector<pair<idx_t,idx_t>>& e);
        ~Graph();
};
class SimGraph: public Graph{
        friend class Vertex;
    public:
        SimGraph(Graph& G,idx_t (*simFunc)(Vertex& u, Vertex& v));
};





#endif