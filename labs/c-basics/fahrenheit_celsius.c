#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

float getCelsius(int f){
	float c = ((f-32)*5)/(float)9;
	return c;
}

int main(int argc, char* argv[])
{
    if(argc==2){
	    char *q = argv[1];
	    int n = atoi(q);
	    printf("Fahrenheit: %d, Celcius: %61.f\n", n, getCelsius(n));
    } else if(argc==4){
	   char *startC = argv[1];
	   char *endC = argv[2];
	   char *incC = argv[3];

	   int startI = atoi(startC);
	   int endI = atoi(endC);
	   int incI = atoi(incC);

	   for(int i = startI; i <= endI; i = i+incI){
		   printf("Fahrenheit: %3d, Celcius: %6.1f\n", i, getCelcius(i));
	   }
    }
	   
    return 0;
}
