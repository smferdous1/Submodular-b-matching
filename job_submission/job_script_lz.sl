#!/bin/sh

#SBATCH -A csit
#SBATCH --nodes=1 
#SBATCH --time=10:30:00
#SBATCH --mem=40G
#SBATCH --job-name lg

PHOME="/home/sferdou/ResourceAllocation"
FD="/scratch/snyder/s/sferdou/SubmodData"
cd $PHOME/build


make 
cp apps/greedySubMatching $PHOME/job_submission
cd $PHOME/job_submission
files=(
Fault_639.mtx
mouse_gene.mtx
Serena.mtx
bone010.mtx
dielFilterV3real.mtx
Flan_1565.mtx
kron_g500-logn21.mtx
hollywood-2011.mtx
G500_21.mtx
SSA21.mtx
eu-2015.mtx
)
b=5
nrun=1
alpha=0.5

for d in "${files[@]}" ; do
    file="$FD/$d"
    #printf "$file"
    ./greedySubMatching $file $b $nrun $alpha
done
