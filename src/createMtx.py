#import matplotlib.pyplot as plt
import numpy as np

n_machine = 1000
#data_file = "GC_BP/gc_bp_"+str(n_machines)+".csv"
phome = "/home/sferdou/Resource-Allocation/"
#data_file = phome+"data/ubi_sto3g/ubi_sto3g_4k_taskinfo.csv"
data_file = phome+"data/ubi_6-31g/ubiquitin_full.6-31g_taskinfo.csv"
data = np.genfromtxt(data_file,dtype=int,delimiter=',',skip_header=1)
n_row = data.shape[0]
nnz = np.count_nonzero(data[:,3])

#graph = np.zeros((n_row*n_machine,3))
c = 0
f = open("/scratch/bell/sferdou/Load-balance-data/ubi_6-31g_"+str(n_machine)+".mtx",'w')
f.write("%%MatrixMarket matrix coordinate real symmetric\n%\n")
f.write(str(n_row+n_machine)+" "+str(n_row+n_machine)+" "+str(nnz*n_machine)+"\n")

for i in range(n_row):
    for j in range(n_machine):
        #graph[c,:] = [n_row+j+1, i+1, data[i,3]]
        if(data[i,3]>0):
            f.write(str(n_row+j+1)+" "+str(i+1)+" "+str(data[i,3])+"\n")
            c = c + 1
