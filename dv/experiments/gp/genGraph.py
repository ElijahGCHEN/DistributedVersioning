import networkx as nx
import matplotlib.pyplot as plt
import random
import argparse
import os
import numpy as np
from drawGraph import drawGraph

parser=argparse.ArgumentParser()
parser.add_argument("numOfNodes", help="number of nodes in the graph",type=int)
parser.add_argument("edgeWeightRange",help="upper limit of random edge weight",type=int)
parser.add_argument("relationship",help="relationship between vertices")
parser.add_argument("-di","--isDirected",action="store_true",help="relationship between vertices")
parser.add_argument("fileName", help="name of output file")
parser.add_argument("format",help="format of output file")
parser.add_argument("-nw","--nodeWeight", type=int, help="Upper limit of node weight")
# parser.add_argument("-i","--incomplete",type=float, help="node degree of incomplete graph")
parser.add_argument("-l","--list", action="store_true",help="list the details of the graph")
parser.add_argument("-d","--draw",action="store_true",help="draw the generated graph")
parser.add_argument("-s","--sequence",type=int,help="generate a sequence of graphs with a percentage less edges")

args=parser.parse_args()

def genMETISFile(G,fileName):
    print(fileName)
    numOfNodes=nx.number_of_nodes(G)
    numOfEdges=nx.number_of_edges(G)
    isWeightedEdge=nx.is_weighted(G)
    nodeWeights=nx.get_node_attributes(G,"weight")
    isWeightedNode=bool(nodeWeights)
    nodes=nx.nodes(G)

    file=open(os.getcwd()+"/"+fileName,"w+")
    print(os.getcwd()+"/"+fileName)
    file.write("{} {} 0{}{}\n".format(numOfNodes,numOfEdges,int(isWeightedNode),int(isWeightedEdge)))
    for u in nodes:
        if (isWeightedNode):
            file.write("{} ".format(G.nodes[u]["weight"]))
        if (isWeightedEdge):
            for v in G.adj[u]:
                file.write("{} {} ".format(v+1,G.edges[u,v]["weight"]))
        file.write("\n")
    file.close()
    
def genCSRFile(G,fileName):
    numOfNodes=nx.number_of_nodes(G)
    adjCount=0
    xadj=[]
    adjncy=[]
    vwgt=[]
    adjwgt=[]
    isWeightedNode=bool(nx.get_node_attributes(G,"weight"))
    for n,nbrs in G.adjacency():
        xadj.append(adjCount)
        if isWeightedNode:
            vwgt.append(G.nodes[n]["weight"])
        for nbr, attr in nbrs.items():
            adjncy.append(nbr)
            adjwgt.append(attr["weight"])
            adjCount+=1
    xadj.append(adjCount)
    file=open(os.getcwd()+"/"+fileName,"w+")
    file.write("{} {} {} {}\n".format(numOfNodes,adjCount,int(args.isDirected),int(isWeightedNode)))

    for u in range(len(xadj)):
        if (u!=len(xadj)-1):
            file.write("{} ".format(xadj[u]))
        else:
            file.write("{}\n".format(xadj[u]))
    for u in range(len((adjncy))):
        if (u!=adjCount-1):
            file.write("{} ".format(adjncy[u]))
        else:
            file.write("{}\n".format(adjncy[u]))
    for u in range(len(adjwgt)):
        if (u!=adjCount-1):
            file.write("{} ".format(adjwgt[u]))
        else:
            file.write("{}".format(adjwgt[u]))
    if isWeightedNode:
        file.write("\n")
        for u in range(len(vwgt)):
            if (u!=numOfNodes-1):
                file.write("{} ".format(vwgt[u]))
            else:
                file.write("{}".format(vwgt[u]))

def randomAdd(H,G,n,order_dict):
    added=0
    for u in G.nodes():
        for i in range(n):
            target=order_dict[u].pop()
            # print(target)
            weight=G.edges[u,target]['weight']
            # print(weight)
            H.add_edge(u,target,weight=weight)
            added+=1
    # print(added)

def genSequence(G,n):
    H=nx.DiGraph()
    H.add_nodes_from(range(args.numOfNodes))
    order_dict ={}
    for u in H.nodes():
        # print(G.edges[u,(u+1)%args.numOfNodes()]['weight'])
        H.add_edge(u,(u+1)%args.numOfNodes,weight=G.edges[u,(u+1)%args.numOfNodes]['weight'])
        seq=list(range(args.numOfNodes))
        seq.remove(u)
        seq.remove((u+1)%args.numOfNodes)
        random.shuffle(seq)
        order_dict[u]=seq
    writeFile(H,f"_{int(100/args.numOfNodes)}")
    num=round((args.numOfNodes-1)*n/100)
    print(num)
    base=((100/(args.numOfNodes-1))//n)+1 
    for i in range(int(base*n),100,n):
        # print (i)
        if i==int(base*n):
            # print((1-(100/args.numOfNodes)/n))
            # print(f"{int((1-(100/args.numOfNodes)/n)*num)*args.numOfNodes} !")
            randomAdd(H,G,round((1-(100/(args.numOfNodes-1))/n)*num),order_dict)
        else:
            randomAdd(H,G,num,order_dict)
        print(H.number_of_edges())
        writeFile(H,f"_{i}")

def writeFile(G,extend):
    index=args.fileName.find('.txt')
    if (args.format.lower()=="metis"):
        genMETISFile(G,args.fileName[:index]+extend+args.fileName[index:])
    elif (args.format.lower()=="csr"):
        genCSRFile(G,args.fileName[:index]+extend+args.fileName[index:])
    elif (args.format.lower()=="both"):
        genMETISFile(G,args.fileName[:index]+"_METIS"+args.fileName[index:])
        genCSRFile(G,args.fileName[:index]+"_CSR"+args.fileName[index:])
    else:
        print("Unknown format")
        exit

# if args.incomplete:
#     if args.incomplete<=0 or args.incomplete>float(args.numOfNodes) :
#         print("invalid degree, must be greater than 0 and smaller or equal to {}".format(args.numOfNodes))
#         exit
#     elif args.incomplete<1:
#         degree=int(round(args.numOfNodes*args.incomplete))
#     else: 
#         degree=int(round(args.incomplete))
#     sequence=[degree]*(args.numOfNodes+1)
#     G=nx.random_degree_sequence_graph(sequence)
# else:
#     G = nx.complete_graph(args.numOfNodes+1)
G = nx.complete_graph(args.numOfNodes)
if (args.relationship=="r"):
    for (u,v) in G.edges():
        print(f"{u},{v}")
        G.edges[u,v]['weight']=random.randint(1,args.edgeWeightRange)
    if args.nodeWeight:
        for u in G.nodes():
            G.nodes[u]['weight']=random.randint(1,args.nodeWeightRange)
    nodes=nx.nodes(G)
    print(G.number_of_edges())
elif (args.relationship=="d"):
    for u in list(G.adj[0]):
        G.edges[0,u]['weight']=max(1,int(random.gauss(args.edgeWeightRange/2,args.edgeWeightRange/5)))
        # print(G.edges[0,u]['weight'])
    for (u,v) in G.edges():
        if (u==0):
            continue
        minWeight=0
        maxWeight=args.edgeWeightRange
        for d in list(range(u)):
            diff=abs(G.edges[d,v]['weight']-G.edges[d,u]['weight'])
            wsum=G.edges[d,v]['weight']+G.edges[d,u]['weight']
            if (diff>minWeight):
                # print(G.edges[d,v]['weight'])
                minWeight=diff
                # print(f"doing {d}-{v}: {G.edges[d,v]['weight']} ")
            if (wsum<maxWeight):
                maxWeight=wsum
        G.edges[u,v]['weight']=int(random.uniform(minWeight,maxWeight))
        # print(f"{u}-{v}: {G.edges[u,v]['weight']} min:{minWeight} max:{maxWeight}")
if (args.isDirected):
    G=nx.DiGraph(G)
writeFile(G,"")

if (bool(args.sequence)):
    genSequence(G,args.sequence)


if args.list:
    print(nx.info(G))
    for n, nbrs in G.adj.items():
        print("\nNode {}:".format(n))
        if (args.nodeWeight):
            print("weight {}".format(G[n]['weight']))
        for nbr, eattr in nbrs.items():
            wt = eattr['weight']
            print(f"({n}-{nbr}: {wt})")
        

#draw
if args.draw:
    drawGraph(G,args.edgeWeightRange)