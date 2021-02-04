#!/bin/sh

#SBATCH -A csit
#SBATCH --nodes=1 
#SBATCH --time=24:30:00
#SBATCH --mem=60G
#SBATCH --job-name lg

PHOME="/home/sferdou/ResourceAllocation"
FD="/scratch/rice/s/sferdou/SubmodData"
cd $PHOME/build


make 
cp apps/lb $PHOME/job_submission/lb1
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
ubiquitin_1800.mtx
)

b=5
nrun=1
alpha=0.5

for d in "${files[@]}" ; do
    file="$FD/$d"
    #printf "$file"
    #./lb1 $file $b $nrun 0.5 > $PHOME/results/"$d"_alpha_0.5 
    ./lb1 $file $b $nrun 0.5 1800 > $PHOME/results/"$d"_alpha_0.5 
    #./lb $file $b $nrun  1 > $PHOME/results/$d_alpha_1 
done
