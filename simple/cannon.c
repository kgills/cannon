/** 
 * @file    cannon.c
 * @author  Kevin Gillespie
 * @brief   cannon's algorithm to calculate the shortest path between all pairs
 *          of a give graph. We're assuming symmetric matricies. 
 *
 */

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* Defines */
#define ARRAY_DIM   8
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
    int i,j,k;
    struct timeval start_time, stop_time, elapsed_time;
    double etime, flops;
    float sum;

    // Initialize the array
    float* matrix_A[ARRAY_DIM];
    float* matrix_B[ARRAY_DIM];
    float* matrix_C[ARRAY_DIM];
    for(i = 0; i < ARRAY_DIM; i++) {

        matrix_A[i] = (float*)malloc(ARRAY_DIM * sizeof(float));
        matrix_B[i] = (float*)malloc(ARRAY_DIM * sizeof(float));
        matrix_C[i] = (float*)malloc(ARRAY_DIM * sizeof(float));

        for(j = 0; j < ARRAY_DIM; j++) {
            matrix_A[i][j] = i*ARRAY_DIM+j;
            if(i == j) {
                matrix_B[i][j] = 1;                
            } else {
                matrix_B[i][j] = 0;
            }
            matrix_C[i][j] = 0;
        }
    }

    if(ARRAY_DIM < 32) {
        printf("Initial array\n");
        print_array(matrix_A);
    }
 
    gettimeofday(&start_time, NULL);

    // Execute the algorithm
    for(i = 0; i < ARRAY_DIM; i++) {
        for(j = 0; j < ARRAY_DIM; j++) {
            for(k = 0; k < ARRAY_DIM; k++) {
                matrix_C[i][j] = matrix_C[i][j] + matrix_A[i][k] * matrix_B[k][j];
            }
        }
    }   

    gettimeofday(&stop_time, NULL);

    timersub(&stop_time, &start_time, &elapsed_time);    
    etime = elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0;

    if(ARRAY_DIM < 32) {
        printf("\nFinal array\n");
        print_array(matrix_C);
    }

    // Verify the results
    for(i = 0; i < ARRAY_DIM; i++) {
        for(j = 0; j < ARRAY_DIM; j++) {
            if(matrix_C[i][j] != i*ARRAY_DIM+j) {
                printf("Error computing matrix multiplication i=%d j=%d\n", i, j);
                return 1;
            }
        }
    }

    flops = ((double)2 * (double)ARRAY_DIM * (double)ARRAY_DIM * (double)ARRAY_DIM)/etime;
    printf("%d, %f, %f, %d\n", ARRAY_DIM, etime, flops, 1);
 

    return 0;
}
