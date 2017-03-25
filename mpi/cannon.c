/** 
 * @file    cannon.c
 * @author  Kevin Gillespie
 * @brief   cannon's algorithm to calculate the shortest path between all pairs
 *          of a give graph. We're assuming symmetric matricies. 
 *
 */

/* Includes */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mpi.h"

/* Defines */
#define ARRAY_DIM   1024
#define MAX_VAL     ARRAY_DIM

#define MIN(a,b) (((a)<(b))?(a):(b))
#define ABS(a) (((a)<0)?((-(a))):(a))

/* Globals */
 

/**
 * @name     print_array
 * @brief    print the given 2d array
 * @param 
 *       @name   array
 *       @dir    I
 *       @type   float
 *       @brief  2d array to print
 *
 ******************************************************************************/
void print_array(float* array[])
{
    unsigned i, j;

    // Print the j labels
    printf(" j:");
    for(j = 0; j < ARRAY_DIM; j++) {
        printf("%d   ", j);
    }
    printf("\ni ");
    for(j = 0; j < ARRAY_DIM; j++) {
        printf("---");
    }

    // Print the array and i labels
    for(i = 0; i < ARRAY_DIM; i++) {
        printf("\n%d| ", i);
        for(j = 0; j < ARRAY_DIM; j++) {
            printf("%03d ", (int)array[i][j]);
        }
    }
    printf("\n\n");
}

/**
 * @name     main
 * @brief    main function for cannon.c
 * @param 
 *       @name   argc
 *       @dir    I
 *       @type   int 
 *       @brief  Number of arguments in argv.
 * @param 
 *       @name   argv
 *       @dir    I
 *       @type   char*[]
 *       @brief  Command line arguments.
 *
 * @returns 0 for success, error status otherwise
 *
 ******************************************************************************/
int main(int argc, char *argv[])
{
    int i,j,k,l;
    struct timeval start_time, stop_time, elapsed_time;
    double etime, flops;
    float sum;

    int id, p, a_pass, b_pass, a_recv, b_recv,dim;
    MPI_Request send_a_req, send_b_req;
    MPI_Status send_a_status, send_b_status, recv_a_status, recv_b_status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    dim = (int)sqrt(p);

    // Figure out which nodes we will pass A and B to.
    if(id % dim == 0) {
        a_pass = id + dim - 1;
    } else {
        a_pass = id - 1;
    }

    if(id < dim) {
        b_pass = id + dim*(dim-1);
    } else {
        b_pass = id - dim;
    }

    // Figure out which nodes we will receive A and B from.
    if((id+1)%dim == 0) {
        a_recv = id + 1 - dim;
    } else {
        a_recv = id + 1;
    }

    if(id >= (dim*(dim-1))) {
        b_recv = id - (dim*(dim-1));
    } else {
        b_recv = id + dim;
    }

    // Initialize the array
    printf("initializing array\n");

    float* matrix_A[ARRAY_DIM/dim];
    float* matrix_A_temp[ARRAY_DIM/dim];
    float* matrix_B[ARRAY_DIM/dim];
    float* matrix_B_temp[ARRAY_DIM/dim];
    float* matrix_C[ARRAY_DIM/dim];
    for(i = 0; i < ARRAY_DIM/dim; i++) {

        matrix_A[i] = (float*)malloc(ARRAY_DIM/dim * sizeof(float));
        matrix_A_temp[i] = (float*)malloc(ARRAY_DIM/dim * sizeof(float));
        matrix_B[i] = (float*)malloc(ARRAY_DIM/dim * sizeof(float));
        matrix_B_temp[i] = (float*)malloc(ARRAY_DIM/dim * sizeof(float));
        matrix_C[i] = (float*)malloc(ARRAY_DIM/dim * sizeof(float));

        int i_offset = i + (id%dim)*(ARRAY_DIM/dim);
        for(j = 0; j < ARRAY_DIM/dim; j++) {

            int j_offset = (id/dim)*(ARRAY_DIM/dim);
            matrix_A[i][j] = i_offset*ARRAY_DIM+j_offset;
            if(i_offset == j_offset) {
                matrix_B[i][j] = 1;                
            } else {
                matrix_B[i][j] = 0;
            }
            matrix_C[i][j] = 0;
        }
    }

    printf("array initialized\n");

    if(ARRAY_DIM < 32) {
        printf("Initial array\n");
        print_array(matrix_A);
    }
 
    MPI_Barrier(MPI_COMM_WORLD);

    gettimeofday(&start_time, NULL);
    
    for(l = 0; l < dim; l++) {
        // Execute the algorithm
        #pragma omp parallel for private(i,j,k) schedule(static)
        for(i = 0; i < ARRAY_DIM/dim; i++) {
            for(j = 0; j < ARRAY_DIM/dim; j++) {
                for(k = 0; k < ARRAY_DIM/dim; k++) {
                    matrix_C[i][j] = matrix_C[i][j] + matrix_A[i][k] * matrix_B[k][j];
                }
            }
        }

        if(l < (sqrt(p) - 1)) {
            // Send the blocks, nonblocking
            MPI_Isend(matrix_A, (ARRAY_DIM/dim)*(ARRAY_DIM/dim), MPI_FLOAT, a_pass, id, MPI_COMM_WORLD, &send_a_req);
            MPI_Isend(matrix_B, (ARRAY_DIM/dim)*(ARRAY_DIM/dim), MPI_FLOAT, b_pass, id, MPI_COMM_WORLD, &send_b_req);

            // Receive the blocks
            MPI_Recv(matrix_A_temp, (ARRAY_DIM/dim)*(ARRAY_DIM/dim), MPI_FLOAT, a_recv, id, MPI_COMM_WORLD, &recv_a_status);
            MPI_Recv(matrix_B_temp, (ARRAY_DIM/dim)*(ARRAY_DIM/dim), MPI_FLOAT, b_recv, id, MPI_COMM_WORLD, &recv_b_status);

            // Wait for the send to complete
            MPI_Wait(&send_a_req, &send_a_status);
            MPI_Wait(&send_b_req, &send_b_status);
        }        
    }
    
    MPI_Barrier(MPI_COMM_WORLD);

    gettimeofday(&stop_time, NULL);

    timersub(&stop_time, &start_time, &elapsed_time);    
    etime = elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0;

    if(ARRAY_DIM < 32) {
        printf("\nFinal array\n");
        print_array(matrix_C);
    }

    // Verify the results
    for(i = 0; i < ARRAY_DIM/dim; i++) {

        int i_offset = i + (id%dim)*(ARRAY_DIM/dim);
        for(j = 0; j < ARRAY_DIM/dim; j++) {

            int j_offset = (id/dim)*(ARRAY_DIM/dim);
            if(matrix_C[i][j] != i_offset*ARRAY_DIM+j_offset) {
                printf("Error computing matrix multiplication i=%d j=%d\n", i_offset, j_offset);
                return 1;
            }
        }
    }

    if(0 == id) {
        flops = ((double)2 * (double)ARRAY_DIM * (double)ARRAY_DIM * (double)ARRAY_DIM)/etime;
        printf("%d, %f, %f, %d\n", ARRAY_DIM, etime, flops, omp_get_num_threads());
    }

    MPI_Finalize(); 

    return 0;
}
