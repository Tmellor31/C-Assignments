#include <stdio.h>
#include <stdlib.h>

int main(void)

{
    int A = 9;
    int C = 5;
    int M = 11;
    int seed = 0;
    int i = 0;
    int LOOPS = 15;
    int firstnumber;

    /* Linear Congruential Generator */
    for (i = 0; i < LOOPS; i++)
    {
        seed = (A * seed + C) % M;

        if (i == 0)
        {
            firstnumber = seed;
        }

        if ((i > 0) && (firstnumber == seed))
        {
            break;
        }
    }
    // Print the period of the LCG or firstnumber
    printf("The period of the LCG issssssss %i\n", firstnumber);
    return 0;
}