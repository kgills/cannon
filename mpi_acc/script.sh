#!/bin/bash
#SBATCH -N 4
#SBATCH -p GPU
#SBATCH --gres=gpu:p100:2
#SBATCH --ntasks-per-node=1
#SBATCH -t 01:00:00
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=kgills@gmail.com

ITERS=$(seq 1 10)
SIZE=16384
FILE_NAME="cannon_acc_mpi"
NODES=4

export OMP_NUM_THREADS=28
touch "${FILE_NAME}_${NODES}_${SIZE}.txt"
echo "matrix_dim, etime, flops, cores">>"${FILE_NAME}_${NODES}_${SIZE}.txt"
for ITER in ${ITERS}
do
    mpirun -np ${NODES} --mca mpi_cuda_support 0 ./cannon.out>>"${FILE_NAME}_${NODES}_${SIZE}.txt"
done
