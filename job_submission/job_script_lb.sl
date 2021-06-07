#!/bin/sh

#SBATCH -A highmem
#SBATCH --nodes=1 
#SBATCH --time=8:30:00
#SBATCH --mem=500G
#SBATCH --job-name lb

PHOME="/home/sferdou/Resource-Allocation"
FD="/scratch/bell/sferdou/Load-balance-data"
cd $PHOME/build


make 
cp apps/lb $PHOME/job_submission
cd $PHOME/job_submission
files=(
#Fault_639.mtx
#mouse_gene.mtx
#Serena.mtx
#bone010.mtx
#dielFilterV3real.mtx
#Flan_1565.mtx
#kron_g500-logn21.mtx
#hollywood-2011.mtx
#G500_21.mtx
#SSA21.mtx
#eu-2015.mtx
#nlpkkt240.mtx
#c60_1200.mtx
#ubiquitin_900.mtx
#ubi_sto3g_1000.mtx
#ubi_sto3g_2000.mtx
#ubi_sto3g_3000.mtx
#ubi_sto3g_4000.mtx
#ubi_sto3g_6000.mtx
#ubi_sto3g_8000.mtx
ubi_6-31g_1000.mtx
#ubi_6-31g_6000.mtx
#ubi_6-31g_8000.mtx
)
b=5
nrun=1
alpha=0.5
nMachine=1000

outfile="$PHOME/data/ubi_6-31g_1000.match"
for d in "${files[@]}" ; do
    file="$FD/$d"
    #printf "$file"
    ./lb $file $outfile 0.5 $nMachine  
    #./lb $file $b $nrun  1 > $PHOME/results/$d_alpha_1 
done
