#!/bin/bash
#SBATCH -N 4
#SBATCH -p RM
#SBATCH --ntasks-per-node=1
#SBATCH -t 04:00:00
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=kgills@gmail.com

ITERS=$(seq 1 1)
SIZE=1024
FILE_NAME="cannon_omp_mpi"
NODES=4

export OMP_NUM_THREADS=28
touch "${FILE_NAME}_${NODES}_${SIZE}.txt"
echo "matrix_dim, etime, flops, cores">>"${FILE_NAME}_${NODES}_${SIZE}.txt"
for ITER in ${ITERS}
do
    mpirun -n ${NODES} ./cannon.out>>"${FILE_NAME}_${NODES}_${SIZE}.txt"
done
