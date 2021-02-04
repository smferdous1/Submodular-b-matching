import networkx as nx
from scipy.io import mmread
from scipy.sparse import find

a = mmread("/scratch/snyder/s/sferdou/SubmodData/social-network-data/socfb-CMU.mtx")
CSR = find(a)
edgelist = list(zip(list(CSR[1]),list(CSR[0])))
G = nx.Graph()
G.add_edges_from(edgelist)
