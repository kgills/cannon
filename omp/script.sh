#!/bin/bash
#SBATCH -N 1
#SBATCH -p RM
#SBATCH --ntasks-per-node=28
#SBATCH -t 04:00:00
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=kgills@gmail.com

ITERS=$(seq 1 3)
SIZE=1024
FILE_NAME="cannon_omp"
NODES=1
CORES=$(seq 1 28)
for CORE in ${CORES}
do
    export OMP_NUM_THREADS=${CORE}
    touch "${FILE_NAME}_${NODES}_${SIZE}_${CORE}.txt"
    echo "matrix_dim, etime, flops, cores">>"${FILE_NAME}_${NODES}_${SIZE}_${CORE}.txt"
    for ITER in ${ITERS}
    do
        ./cannon.out>>"${FILE_NAME}_${NODES}_${SIZE}_${CORE}.txt"
    done
done
