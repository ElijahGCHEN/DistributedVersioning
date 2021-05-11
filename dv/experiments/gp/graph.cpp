#include "graph.h"
#include "metis.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <string>

using namespace std;

GraphRep::GraphRep(): isDirected(false), isWeightedNode(false),vertexCount(0), edgeCount(0), adjCount(0) {}

GraphRep::GraphRep(const idx_t& vertexCount,const idx_t& edgeCount,const idx_t& adjCount,bool isDirected, bool isWeightedNode,
const idx_t* xadj, const idx_t* adjncy, const idx_t* adjwgt, const idx_t* vwgt):vertexCount(vertexCount),
edgeCount(edgeCount),adjCount(adjCount),isDirected(isDirected),isWeightedNode(isWeightedNode){
    this->xadj=new idx_t[vertexCount+1];
    this->adjncy=new idx_t[adjCount];
    this->adjwgt=new idx_t[adjCount];
    if (isWeightedNode)
        this->vwgt=new idx_t[vertexCount];
    else
        this->vwgt=nullptr;
    
    for (int i=0;i<vertexCount+1;i++){
        this->xadj[i]=xadj[i];
        if (isWeightedNode)
            this->vwgt[i]=vwgt[i];
    }
    for (int i=0;i<adjCount;i++){
        this->adjncy[i]=adjncy[i];
        this->adjwgt[i]=adjwgt[i];
    }
}

void GraphRep::parseGraphFile(std::string fileName){
    fstream fio;
    string line;
    vector<string> paramvec; 
    fio.open(fileName,ios::in);
    if (fio.is_open()){
        int lc=1;
        cout<<"Filename: "<<fileName<<endl;
        while (getline(fio,line)){
            cout<<line<<endl;
            stringstream sstream(line);
            switch (lc)
            {
            case 1:
                {
                    cout<<"Parsing line 1"<<endl;
                    while (sstream.good()){
                    string param;
                    getline(sstream,param,' ');
                    paramvec.push_back(param);
                    }
                    vertexCount=stoi(paramvec[0]);
                    adjCount=stoi(paramvec[1]);
                    isDirected=(paramvec[2].compare("1")==0? true:false);
                    isWeightedNode=(paramvec[3].compare("1")==0? true:false);
                    xadj= new idx_t[vertexCount+1];
                    adjncy=new idx_t[adjCount];
                    adjwgt=new idx_t[adjCount];
                    if (isWeightedNode)
                    vwgt=new idx_t[vertexCount];
                    else
                    vwgt=nullptr;
                    break;
                }
            case 2:
                {
                    cout<<"Parsing line 2"<<endl;
                    int i=0;
                    while (sstream.good()){
                    string j;
                    getline(sstream,j,' ');
                    xadj[i]=stoi(j);
                    i++;
                    }
                    break;
                }
            case 3:
                {   
                    cout<<"Parsing line 3"<<endl;
                    int i=0;
                    while (sstream.good()){
                    string j;
                    getline(sstream,j,' ');
                    adjncy[i]=stoi(j);
                    i++;
                    }
                    break;
                }
            case 4:
                {
                    cout<<"Parsing line 4"<<endl;                
                    int i=0;
                    while (sstream.good()){
                    string j;
                    getline(sstream,j,' ');
                    adjwgt[i]=stoi(j);
                    i++;
                    }
                    break;
                }
            case 5:
                {
                    if (isWeightedNode){
                    cout<<"Parsing line 5"<<endl;
                    int i=0;
                    while (sstream.good()){
                    string j;
                    getline(sstream,j,' ');
                    cout<<j<<endl;
                    vwgt[i]=stoi(j);
                    
                    i++;
                    }
                    }
                    break;
                }
            default:
                break;
            }
            lc++;
        }
        fio.close();
    }
    
}

GraphRep::GraphRep(string fileName){
    parseGraphFile(fileName);
    edgeCount=(isDirected? adjCount:adjCount/2);
}

GraphRep::GraphRep(const GraphRep& G): vertexCount(G.vertexCount), edgeCount(G.edgeCount), adjCount(G.adjCount),
isDirected(G.isDirected), isWeightedNode(G.isWeightedNode){}

GraphRep::GraphRep(vector<idx_t>& xadj, vector<idx_t>&adjncy,vector<idx_t>&adjwgt, vector<idx_t>&vwgt,
        idx_t& vertexCount,idx_t& edgeCount, bool isDirected, bool isWeightedNode):vertexCount(vertexCount),
        edgeCount(edgeCount),isDirected(isDirected),isWeightedNode(isWeightedNode) {
    adjCount=adjncy.size();
    this->xadj=new idx_t[vertexCount+1];
    this->adjncy=new idx_t[adjCount];
    this->adjwgt=new idx_t[adjCount];
    for (int i=0;i<vertexCount+1;i++){
        this->xadj[i]=xadj[i];
    }
    for (int i=0;i<adjCount;i++){
        this->adjncy[i]=adjncy[i];
        this->adjwgt[i]=adjwgt[i];
    }    
    if (isWeightedNode){
        for (int i=0;i<vertexCount;i++){
        this->vwgt=new idx_t[vertexCount];
        this->vwgt[i]=vwgt[i];
        }
    }
    else this->vwgt=nullptr;
}
vector<GraphRep*>* GraphRep::partGraph(idx_t n,idx_t*& parts){
    idx_t cut=0;
    parts= new idx_t[vertexCount];
    idx_t ncon=1;
    int result=METIS_PartGraphKway(&vertexCount,&ncon,xadj,adjncy,NULL,NULL,adjwgt,&n,NULL,NULL,NULL,&cut,parts);
    switch(result){
        case METIS_OK:{
            cout<<"partition successful"<<endl;
            unordered_set<idx_t> vset[n];
            for (idx_t i=0; i<vertexCount;i++){
                vset[parts[i]].insert(i);
            }
            vector<pair<idx_t,idx_t>>* edges=new vector<pair<idx_t,idx_t>>[vertexCount];
            for (idx_t i=0; i<vertexCount;i++){
                idx_t lim=xadj[i+1];
                for (idx_t j=xadj[i];j<lim;j++){
                        unordered_set<idx_t>::const_iterator itr=vset[parts[i]].find(adjncy[j]);
                        if (itr==vset[parts[i]].end())
                            continue;
                        else{
                            edges[i].push_back(make_pair(adjncy[j],adjwgt[j]));
                        }
                    }
            }
            vector<idx_t> newxadj[n];
            vector<idx_t> newadjncy[n];
            vector<idx_t> newadjwgt[n];
            vector<idx_t> newvwgt[n];
            idx_t newadjcount[n];
            for (int i=0;i<n;i++)
                newadjcount[i]=0;
            for (idx_t i=0;i<vertexCount;i++){
                for (idx_t j=0;j<edges[i].size();j++){
                    newadjncy[parts[i]].push_back(edges[i][j].first);
                    newadjwgt[parts[i]].push_back(edges[i][j].second);
                }
                newxadj[parts[i]].push_back(newadjcount[parts[i]]);
                newadjcount[parts[i]]+=edges[i].size(); 
                if (isWeightedNode)
                    newvwgt[parts[i]].push_back(vwgt[i]);
            }
            for (int i=0;i<n;i++)
                newxadj[i].push_back(newadjcount[i]);
            vector<GraphRep*>* results=new vector<GraphRep*>;
            for (int i=0;i<n;i++){
                idx_t vcount=vset[i].size();
                results->push_back(new GraphRep(newxadj[i],newadjncy[i],newadjwgt[i],newvwgt[i],
                vcount,newadjcount[i],false,isWeightedNode));
            }
            // if (out==nullptr)
                // delete[] parts;
            return results;
        }
        case METIS_ERROR_INPUT:{
            cout<<"input error"<<endl;
            return nullptr;
        }
        case METIS_ERROR_MEMORY:{
            cout<<"memory error"<<endl;
            return nullptr;
        }
        case METIS_ERROR:{
            cout<<"other error"<<endl;
            return nullptr;
        }
        default:
            return nullptr;
    }
}


void GraphRep::print(){
    cout<<"===Graph info==="<<endl<<endl;
    cout<<"#Vertices: "<<vertexCount<<" #Edges: "<<edgeCount<<(isDirected?" Directed, ":" Undirected, ")
    <<(isWeightedNode?"Weighted nodes":"Unweighted nodes")<<endl;
    cout<<"Printing xadj:\n";
    for (int i=0;i<vertexCount+1;i++){
        cout<<xadj[i]<<' ';
        if (!((i+1)%5))
            cout<<endl;
    }
    cout<<endl;
    cout<<"Printing adjncy:\n";
    for (int i=0;i<adjCount;i++){
        cout<<adjncy[i]<<' ';
        if (!((i+1)%5))
            cout<<endl;
    }
    cout<<endl;
    cout<<"Printing adjwgt:\n";
    for (int i=0;i<adjCount;i++){
        cout<<adjwgt[i]<<' ';
        if (!((i+1)%5))
            cout<<endl;
    }
    cout<<endl;
    if (isWeightedNode){
        cout<<"Printing vwgt:\n";
        for (int i=0;i<vertexCount;i++){
            cout<<vwgt[i]<<' ';
            if (!((i+1)%5))
            cout<<endl;
        }
        cout<<endl;
    }
}

idx_t GraphRep::getVertexCount() const{
    return vertexCount;
}

idx_t GraphRep::getEdgeCount() const{
    return edgeCount;
}
GraphRep::~GraphRep(){
    delete[] xadj;
    delete[] adjncy;
    delete[] vwgt;
    delete[] adjwgt;
}

Graph::Graph(): GraphRep(){}

// Graph::Graph(vector<Vertex*> vertices, bool isDirected, bool isWeightedNode): vertices(vertices),
// isDirected(isDirected), isWeightedNode(isWeightedNode){}

Graph::Graph(string fileName): GraphRep(fileName){
    cout<<"parse ok"<<endl;
    for (int i=0;i<vertexCount;i++){
        if (isWeightedNode){
            vertices.insert(make_pair(i,new Vertex(i,vwgt[i])));
        }
        else{
        vertices.insert(make_pair(i,new Vertex(i)));
        }
    }
    cout<<"num of vertice: "<<vertices.size()<<endl;
    cout<<"vmap ok"<<endl;
    for (int i=0;i<vertexCount;i++){
        cout<<"doing vertex "<<i<<endl;
        idx_t lim;
        if (i!=vertexCount-1)
            lim=xadj[i+1];
        else
            lim=adjCount;
        for (int j=xadj[i];j<lim;j++){
                // cout<<"adding index"<<j<<endl;
                vertices[i]->addAdjacent(vertices[adjncy[j]],adjwgt[j]);
            }
        cout<<endl;
    }
    cout<<"addAdjacent ok"<<endl;
}

void Graph::updateRep(){
    idx_t* newxadj=new idx_t[vertexCount];
    idx_t* newadjncy=new idx_t[adjCount];
    idx_t* newadjwgt=new idx_t[adjCount];
    idx_t* newvwgt=nullptr;
    if (isDirected)
        newvwgt=new idx_t[vertexCount];

    int xadjptr=0;
    int adjncyptr=0;
    for (const pair<idx_t,Vertex*>& it: vertices){
        const unordered_map<Vertex*,idx_t>* adjmap=it.second->getAdjMap();
        newxadj[it.first]=xadjptr;
        for (const pair<Vertex*,idx_t>& e: *adjmap){
            newadjncy[adjncyptr]=e.first->getId();
            newadjwgt[adjncyptr]=e.second;
            adjncyptr++;
        }
        xadjptr+=it.second->getAdjSize();
        if (isDirected){
            vwgt[it.first]=it.second->getWeight();
        }
    }
    delete[] xadj;
    delete[] adjncy;
    delete[] vwgt;
    delete[] adjwgt;
    xadj=newxadj;
    adjncy=newadjncy;
    adjwgt=newadjwgt;
    vwgt=newvwgt;
}

void Graph::print(){
    GraphRep::print();
    cout<<"Printing vertex info:"<<endl;
    for (int i=0;i<vertexCount;i++){
        Vertex* v=vertices[i];
        idx_t id=v->getId();
        cout<<"Vertex "<<id<<endl;
        if (isWeightedNode)
            cout<<"Weight: "<<v->getWeight()<<endl;
        const unordered_map<Vertex*,idx_t>* const map=v->getAdjMap();
        int count=1;
        // for (auto& it: map){
        //     cout<<it.second<<"\nsecond"<<endl;
        //     cout<<'{'<<id<<'-'<<typeid(*it.first)==typeid(Vertex)<<": "<<it.second<<'}';
        //     if (count%3)
        //         cout<<", ";
        //     else cout<<endl;
        //     count++;
        // }
        v->printAdjacent();
        cout<<endl;
    }
}

Graph::Graph(const Graph& G): GraphRep(G.vertexCount,G.edgeCount,G.adjCount,G.isDirected,
G.isWeightedNode,G.xadj,G.adjncy,G.adjwgt,G.vwgt){
    for (auto& it: G.vertices){
        vertices[it.first]=new Vertex(it.second->id,it.second->weight);
    }
    for (auto& it: vertices){
        idx_t vid=it.first;
        Vertex* oldv=G.vertices.at(vid);
        Vertex* v=it.second;
        for (auto& adj:oldv->adjacents){
            v->addAdjacent(vertices[adj.first->id],adj.second);
        }
    }
}

// int Graph::removeEdge(vector<pair<idx_t,idx_t>>& edges){
//     if (edges.size()==0)
//         return 0;
//     for (pair<idx_t,idx_t>& e: edges){
//         vertices[e.first]->removeEdge(vertices[e.second]);
//         if (!isDirected)
//             vertices[e.second]->removeEdge(vertices[e.second]);
//     }
//     return 0;
// }
Graph::~Graph(){
    for (auto& it: vertices){
        delete it.second;
    }
}

SimGraph::SimGraph(Graph& G,idx_t (*simFunc)(Vertex& u, Vertex& v)):Graph(G){
        unordered_set<pair<Vertex*,Vertex*>,boost::hash<pair<Vertex*,Vertex*>>>  processed;
        for (const pair<idx_t,Vertex*>& it: vertices){
            Vertex* vertex=it.second;
            unordered_map<Vertex*,idx_t>* adj=&vertex->adjacents;
            for ( auto& v: *adj){
                Vertex* dest=v.first;
                const unordered_map<Vertex*,idx_t>* destAdj=dest->getAdjMap();
                unordered_map<Vertex*,idx_t>::const_iterator itr=destAdj->find(vertex);
                if (itr==destAdj->end()){
                    dest->addAdjacent(vertex,0);
                    adjCount++;
                }
                if (processed.find(make_pair(dest,vertex))==processed.end()){
                    idx_t newWeight=simFunc(*vertex,*dest);
                    vertex->addAdjacent(dest,newWeight);
                    dest->addAdjacent(vertex,newWeight);
                    processed.insert(make_pair(vertex,dest));
                }
            }
        } 
        updateRep();
        edgeCount=adjCount/2;
        isDirected=false;
}