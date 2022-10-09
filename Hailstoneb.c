#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int number;
    printf("Enter the number you wish to see the hailstone sequence of!\n");
    scanf("%i", &number);

    while (number > 1)
    {

        if (number % 2 != 0)
        {
            number = (3 * number) + 1;
            printf("%i\n", number);
        }

        else
        {
            number = number / 2;
            printf("%i\n", number);
        }
    }
    printf("%i\n", number);
    return 0;
}