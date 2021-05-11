#include <iostream>
#include <fstream>
#include <string>
#include "graph.h"
#include "metis.h"
#include <map>
#include <random>

using namespace std;

map<int,float>* genResults(map<int,GraphRep*>& Graphs,int n,int step){
    map<int,float>* results=new map<int,float>;
    int count=Graphs[100]->getVertexCount();
    idx_t* fullarr;
    vector<GraphRep*>* fullvec=Graphs[100]->partGraph(n,fullarr);
    for (auto& it: Graphs){
        if (it.first==100)
            continue;
        int diff=0;
        idx_t* partarr;
        vector<GraphRep*>* pvec=it.second->partGraph(n,partarr);
        for (int i=0;i<count;i++){
            if (fullarr[i]!=partarr[i])
                diff++;
        }
        results->insert(make_pair(it.first,(count-diff)/(float)count));
    }
    return results;
}

map<int,GraphRep*>* parseFiles(vector<string>& fileNames){
    map<int,GraphRep*>* results=new map<int,GraphRep*>;
    for(string& name: fileNames){
        cout<<name<<endl;
        int found = name.find("_");
        if (found!=name.npos){
            int dot=name.find(".");
            string percent=name.substr(found+1,dot-found-1);
            cout<<percent<<endl;
            results->insert((make_pair(stoi(percent),new GraphRep(name))));
        }
        else
            results->insert(make_pair(100,new GraphRep(name)));
    }
    return results;
}
int main(int argc, char** argv){
    vector<string> filevec;
    for (int i=1;i<argc;i++){
        string name=argv[i];
        filevec.push_back(name);
    }
    map<int,GraphRep*>* graphMap=parseFiles(filevec);
    for (auto& it:*graphMap){
        cout<<it.first<<": "<<it.second->getEdgeCount()<<endl;
        // it.second->print();
    }
    // GraphRep G("s10_1.txt");
    // G.print();
    map<int,float>* resultMap=genResults(*graphMap,3,5);
    for (auto& it:*resultMap){
        cout<<it.first<<": "<<it.second<<endl;
    }
}