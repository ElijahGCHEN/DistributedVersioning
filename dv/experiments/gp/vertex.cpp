#include "vertex.h"
#include <iostream>
using namespace std;


Vertex::Vertex(int id,idx_t weight): id(id), weight(weight){}

Vertex::Vertex(const Vertex& V): id(V.id),weight(V.weight),adjacents(V.adjacents){}

void Vertex::addAdjacent(Vertex* newVertex, idx_t edgeWeight){
    if (newVertex==this)
        return;
    unordered_map<Vertex*,idx_t>::iterator dest=adjacents.find(newVertex);
    if (dest==adjacents.end()){
    // cout<<"adding "<<newVertex->getId()<<" with weight "<<weight<<endl;
    adjacents.insert(make_pair(newVertex,edgeWeight));
    }
    else{
        dest->second=edgeWeight;
    }
}

const unordered_map<Vertex*,idx_t>* Vertex::getAdjMap(){
    return &adjacents;
}

const int Vertex::getId(){
    return id;
}

const idx_t Vertex::getWeight(){
    return weight;
}

void Vertex::printAdjacent() const{
    if (adjacents.empty()){
        cout<<"adjacency list empty";
    }
    else{
        int count=0;
        for (const pair<Vertex* const,idx_t>& it: adjacents){
            if (!(count%3) ){
                if (count)
                    cout<<endl;
            }
            else cout<<", ";
            cout<<'{'<<id<<'-'<<it.first->getId()<<": "<<it.second<<'}';
            count++;
        }
    }
    cout<<endl;
}

size_t Vertex::getAdjSize(){
    return adjacents.size();
}
idx_t Vertex::getEdgeWeight(Vertex* target) const{
    unordered_map<Vertex*,idx_t>::const_iterator ptr=adjacents.find(target);
    if (ptr!=adjacents.end())
        return ptr->second;
    else
        return 0;
}