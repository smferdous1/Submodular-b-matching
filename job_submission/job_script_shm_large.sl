#!/bin/sh

#SBATCH -A highmem
#SBATCH --nodes=1 
#SBATCH --ntasks=128
#SBATCH --time=20:30:00
#SBATCH --mem=900G
#SBATCH --job-name shm-llg-large

PHOME="/home/sferdou/Resource-Allocation"
FD="/scratch/bell/sferdou/SubmodData/large-data-set/"
cd $PHOME/build

make 
cp apps/shmllg $PHOME/job_submission
cd $PHOME/job_submission

files=(
#AGATHA_2015.mtx
MOLIERE_2016.mtx
GAP-urand.mtx
com-Friendster.mtx
)


b=5
nrun=1
alpha=0.5

for d in "${files[@]}" ; do
    for nt in 1 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 126; do
        file="$FD/$d"
        #printf "$file"
        export OMP_PLACES=cores
        export OMP_PROC_BIND=close
        ./shmllg $file $b $nrun $alpha $nt
    done
done
