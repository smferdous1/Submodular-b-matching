#!/bin/sh

#SBATCH -A csit
#SBATCH --nodes=1 
#SBATCH --ntasks=20
#SBATCH --time=50:30:00
#SBATCH --mem=50G
#SBATCH --job-name llg

PHOME="/home/sferdou/ResourceAllocation"
FD="/scratch/snyder/s/sferdou/SubmodData"
cd $PHOME/build

make 
cp apps/shmllg $PHOME/job_submission
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
nlpkkt240.mtx
)
b=5
nrun=1
alpha=0.5

for d in "${files[@]}" ; do
    for nt in 1 2 4 6 8 10 12 14 16 18 ; do
        file="$FD/$d"
        #printf "$file"
        export OMP_PLACES=cores
        export OMP_PROC_BIND=close
        ./shmllg $file $b $nrun $alpha $nt
    done
done
