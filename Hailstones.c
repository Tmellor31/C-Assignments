#include <stdio.h>
#include <stdlib.h>
#define LIMIT 100000000
int main(void)
{
    int number;
    int i;
    int count = 0;
    int largest = 0;
    int total = 0;

    for (i = 0; i < LIMIT; i++)
    {

        number = i;

        if (number % 2 != 0)
        {
            number = (3 * number) + 1;
            count = count + 1;
                if (count > largest)
                {
                total = i;
                }
        }

        else
        {
            number = number / 2;
            count = count + 1;
                if (count > largest)
                {
                total = i;
                }
        }
    }
    printf("%i\n", total);
    return 0;
}
