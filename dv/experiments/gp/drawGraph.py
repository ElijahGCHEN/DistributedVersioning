import networkx as nx
import matplotlib.pyplot as plt


def drawGraph(G,edgeWeightRange):
    elarge = [(u, v) for (u, v, d) in G.edges(data=True) if d["weight"] > edgeWeightRange//2]
    esmall = [(u, v) for (u, v, d) in G.edges(data=True) if d["weight"] <= edgeWeightRange//2]

    pos = nx.spring_layout(G)  # positions for all nodes

    # nodes
    nx.draw_networkx_nodes(G, pos, node_size=700)

    # edges
    nx.draw_networkx_edges(G, pos, edgelist=elarge, width=3)
    nx.draw_networkx_edges(
        G, pos, edgelist=esmall, width=2, alpha=0.5, edge_color="b", style="dashed"
    )

    # labels
    labels = nx.get_edge_attributes(G,'weight')
    nx.draw_networkx_labels(G, pos, font_size=20, font_family="sans-serif")
    nx.draw_networkx_edge_labels(G,pos,edge_labels=labels)

    plt.axis("off")
    plt.show()

    #draw_2

    # print(G[1][2])
    # plt.subplot(111)
    # pos=nx.spring_layout(G)
    # nx.draw(G, with_labels=True, font_weight='bold')
    # labels = nx.get_edge_attributes(G,'weight')
    # nx.draw_networkx_edge_labels(G,pos,edge_labels=labels)
    # plt.show()