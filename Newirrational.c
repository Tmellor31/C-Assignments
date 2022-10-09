#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#define NUMERATOR 87
#define E 2.71828182845904523536
#define LIMIT 50

double numberfinder(void);
double distancefinder(int denom);


int main(void)
{
    int m = 0; 
    int x = numberfinder();
    printf("The SMALLEST NUMBER FOUND IS %i\n", x);
    m = distancefinder(32); 
    printf("%i", m); 
    return 0;
}

double numberfinder(void)
{
    double distancetoe;
    double previousdistancetoe; 
    int smallest;
    int denom;
    int smalldenom = 500;
    for (denom = 1; denom < LIMIT; denom++)
    {
        distancetoe = distancefinder(denom); 
        previousdistancetoe = distancefinder(smalldenom);
        //printf("%i/%i - E = %lf\n", NUMERATOR, denom, distancetoe);
        if (distancetoe < previousdistancetoe)
        {
            smalldenom = denom;
        }
    }
    return smalldenom;
}

double distancefinder (int denom)
{
    return fabs(NUMERATOR/denom - E);
}