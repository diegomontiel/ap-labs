/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include "omp.h"
#include <stdio.h>
#include <stdlib.h>

#define N        10000

/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;
int flag = 0;


/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
    int i;
    for (i=0;i<length;i++) {
        randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
        *(a+i) = ((double) randy)/((double) RAND_MOD);
    }
    flag = 1;
    #pragma omp flush (flag)
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
        while(!flag);
        int i;  double sum = 0.0;
        for (i=0;i<length;i++){
            sum += *(a+i);
        }
        return sum;    
}

int main()
{
    double *A, sum, runtime;
    A = (double *)malloc(N*sizeof(double));
    runtime = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
                fill_rand(N, A);        // Producer: fill an array of data
            #pragma omp section
                sum = Sum_array(N, A);  // Consumer: sum the array
        }
    }
     
    runtime = omp_get_wtime() - runtime;
	
    printf(" In %lf seconds, The sum is %lf \n",runtime,sum);
}