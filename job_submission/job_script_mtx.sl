#!/bin/sh

#SBATCH -A highmem
#SBATCH --nodes=1 
#SBATCH --time=8:30:00
#SBATCH --mem=100G
#SBATCH --job-name mtx 

PHOME="/home/sferdou/Resource-Allocation"
module load anaconda/2020.02-py37

python $PHOME/src/createMtx.py
