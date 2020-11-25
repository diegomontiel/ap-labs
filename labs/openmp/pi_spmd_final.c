#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main ()
{
    int i;
    double x, pi, sum = 0.0;
    double start_time, run_time;
    start_time = omp_get_wtime();
    step = 1.0/(double) num_steps;
    #pragma omp parallel for reduction(+:sum) private(x)
        for (i=1;i<= num_steps; i++){
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }
        pi = step * sum;
        run_time = omp_get_wtime() - start_time;
        printf("Pi with %ld steps equals: %lf. Obtained in %lf seconds\n ",num_steps,pi,run_time);
}