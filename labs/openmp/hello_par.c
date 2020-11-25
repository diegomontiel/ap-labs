#include <stdio.h>
#include <omp.h>
int main() {
    #pragma  omp parallel num_threads(4)
    {
        printf("Hello World, Thread -> %d\n",omp_get_thread_num());
    }
    return 0;
}