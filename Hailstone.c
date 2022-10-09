#include <stdio.h>
#include <stdlib.h>

int main(void){
    int number;
    int i;
    int count = 0;
    
    for (i = 0; i < 10000, i++)
    {

        number = i;

        if (number % 2 != 0)
        {
            number = (3 * number) + 1;
        }

        else
        {
            number = number / 2;
        }

        printf("%i\n", number);
        count + 1; 
    }

    return 0;
}
