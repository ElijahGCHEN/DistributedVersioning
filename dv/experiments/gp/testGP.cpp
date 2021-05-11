#include <iostream>
#include "graph.h"
#include <string>
#include "metis.h"
#include <cstdlib>

using namespace std;

idx_t add(Vertex& u,Vertex& v){
        return (u.getEdgeWeight(&v)+v.getEdgeWeight(&u));
}
int main(int argc, char* argv[]){
    string fileName=string(argv[1]);
    GraphRep G(fileName);
    cout<<endl;
    G.print();
    // // Graph G2(G);
    // // G2.updateRep();
    // // cout<<endl;
    // // G2.print();
    // SimGraph S(G,add);
    // S.print();


    // Vertex v1(1,1);
    // Vertex v2(2,1);
    // v1.addAdjacent(&v2,1);
    // v1.printAdjacent();
    idx_t* parts;
    vector<GraphRep*>* partitions=G.partGraph(3);
    for (GraphRep* it: *partitions)
        it->print();
    
    // idx_t a[7]={0,2,4,6,8,10,12};
    // idx_t b[12]={2,3,0,2,5,3,4,0,1,2,1,0};
    // idx_t c[12]={3,4,2,6,4,1,3,6,3,2,5,3};
    
    // idx_t* xadj=a;
    // idx_t* adjncy=b;
    // idx_t* adjwgt=c;

    // idx_t v=6;
    // idx_t ncon=1;
    // idx_t n=4;
    // idx_t cut=0;
    // idx_t* parts=new idx_t[6];
    // idx_t* xadj=new idx_t[7];
    // for (int i=0;i<7;i++)
    //     xadj[i]=5*(idx_t)i;
    // idx_t* adjncy=new idx_t[30];
    // for (int i=0;i<6;i++){
    //     int c=0;
    //     for (int j=0;j<6;j++){
    //         if (i==j) continue;
    //         adjncy[i*5+c]=(idx_t)j;
    //         cout<<i*5+c<<": "<<j<<endl;
    //         c++;
    //     }
    // }
    // idx_t* adjwgt=new idx_t[30];
    // srand(time(0));
    // for (int i=0;i<30;i++){
    //     adjwgt[i]=rand()%10+1;
    //     cout<<adjncy[i]<<": "<<adjwgt[i]<<" ";
    // }
    // cout<<"asd"<<endl;
    
    // for (int i=0;i<6;i++)
    //     parts[i]=0;
    // int result=METIS_PartGraphKway(&v,&ncon,xadj,adjncy,NULL,NULL,adjwgt,&n,NULL,NULL,NULL,&cut,parts);
    // cout<<result<<endl;
    // for (int i=0;i<6;i++){
    //     cout<<i<<" :"<<parts[i]<<endl;
    // }
    
//     idx_t a=15;
// idx_t b=1;
// idx_t c[16]={0,2,5,8,11,13,16,20,24,28,31,33,36,39,42,44};
// idx_t d[44]={1,5,0,2,6,1,3,7,2,4,8,3,9,0,6,10,1,5,7,11,2,6,8,12,3,7,9,13,4,8,14,5,11,6,10,12,7,11,13,8,12,14,9,13};
// idx_t e=4;
// idx_t *nvtxs=&a;
// idx_t *ncon=&b;
// idx_t *xadj=c;
// idx_t *adjncy=d;
// idx_t *nparts=&e;
// idx_t objval;
// idx_t *part;
// METIS_PartGraphRecursive (nvtxs,ncon,xadj,adjncy,NULL,NULL,NULL,nparts,NULL,NULL,NULL,&objval,part);
    return 0;
}