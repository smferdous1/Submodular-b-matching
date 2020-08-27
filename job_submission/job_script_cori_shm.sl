#!/bin/sh

#SBATCH --qos=regular 
#SBATCH --nodes=1 
#SBATCH --tasks-per-node=68
#SBATCH --time=01:30:00
#SBATCH --mem=50G
#SBATCH --constraint=knl

module load gcc/9.3.0
PHOME="/global/homes/s/sferdou/Resource-Allocation"
FD="/global/cscratch1/sd/sferdou/SubmodData/orignal_weight_data"
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

export OMP_NUM_THREADS=68
export OMP_PLACES=cores
export OMP_PROC_BIND=close

for d in "${files[@]}" ; do
    for nt in 1  4  8  12  16  20  24  28  32  36  40  44  48  52  56  60  64 67; do
        file="$FD/$d"
        #printf "$file"
        srun -n 1 -c 272 --cpu_bind=cores shmllg $file $b $nrun $alpha $nt
    done
done
