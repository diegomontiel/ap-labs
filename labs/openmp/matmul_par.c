#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define ORDER 1000
#define AVAL 3.0
#define BVAL 5.0
#define TOL  0.001

int main(int argc, char **argv)
{
    int Ndim, Pdim, Mdim;   /* A[N][P], B[P][M], C[N][M] */
    int i,j;
    double *A, *B, *C, cval, tmp, err, errsq;
    double dN, mflops;
    double start_time, run_time;


    Ndim = ORDER;
    Pdim = ORDER;
    Mdim = ORDER;

    A = (double *)malloc(Ndim*Pdim*sizeof(double));
    B = (double *)malloc(Pdim*Mdim*sizeof(double));
    C = (double *)malloc(Ndim*Mdim*sizeof(double));

    /* Initialize matrices */

    for (i=0; i<Ndim; i++)
	for (j=0; j<Pdim; j++)
	    *(A+(i*Ndim+j)) = AVAL;

    for (i=0; i<Pdim; i++)
	for (j=0; j<Mdim; j++)
	    *(B+(i*Pdim+j)) = BVAL;

    for (i=0; i<Ndim; i++)
	for (j=0; j<Mdim; j++)
	    *(C+(i*Ndim+j)) = 0.0;

    /* Do the matrix product */
    start_time = omp_get_wtime();
    
    #pragma omp parallel for private(tmp) collapse(2)
        for (i=0; i<Ndim; i++){
            for ( j=0; j<Mdim; j++){
                tmp = 0.0;
                for(int k=0;k<Pdim;k++){
                    /* C(i,j) = sum(over k) A(i,k) * B(k,j) */
                    tmp += *(A+(i*Ndim+k)) *  *(B+(k*Pdim+j));
                }
                *(C+(i*Ndim+j)) = tmp;
            }
        }
    
    
    /* Check the answer */

    run_time = omp_get_wtime() - start_time;

    printf(" Order %d multiplication in %f seconds \n", ORDER, run_time);

    dN = (double)ORDER;
    mflops = 2.0 * dN * dN * dN/(1000000.0* run_time);

    printf(" Order %d multiplication at %f mflops\n", ORDER, mflops);

    cval = Pdim * AVAL * BVAL;
    errsq = 0.0;
    for (i=0; i<Ndim; i++){
	for (j=0; j<Mdim; j++){
	    err = *(C+i*Ndim+j) - cval;
	    errsq += err * err;
	}
    }

    if (errsq > TOL)
	printf("\n Errors in multiplication: %f",errsq);
    else
	printf("\n Hey, it worked");

    printf("\n all done \n");
}