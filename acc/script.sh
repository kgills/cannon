#!/bin/bash
#SBATCH -N 1
#SBATCH -p GPU
#SBATCH --gre=gpu:p100:2
#SBATCH --ntasks-per-node=28
#SBATCH -t 04:00:00
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=kgills@gmail.com

ITERS=$(seq 1 10)
SIZE=16384
FILE_NAME="cannon_acc"
NODES=1

export OMP_NUM_THREADS=28
touch "${FILE_NAME}_${NODES}_${SIZE}.txt"
echo "matrix_dim, etime, flops, cores">>"${FILE_NAME}_${NODES}_${SIZE}.txt"
for ITER in ${ITERS}
do
    ./cannon.out>>"${FILE_NAME}_${NODES}_${SIZE}.txt"
done
