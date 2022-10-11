#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define Columns 34
#define Rows 30

int size[Columns];

typedef enum CellState
{
    OFF,
    ON
} CellState;

void test(void);
CellState cellgenerator(CellState x, CellState y, CellState z);
CellState rowcreator(CellState x, CellState y, CellState z);

int main(void)
{
    test();
    int array[20] = {0}; // 5x4 grid
    int x;
    array[2] = 1;
    array[6] = 1;
    array[8] = 1;

    for (x = 0; x < 20; x++)
    {
        if (x % 5 == 0 && x != 0)
        {
            printf("\n");
        }
        printf("%i", array[x]);
    }
//need to create the grid via this method and run the digits array[] through cellgenerator. Excluding top line for user input. 
    return 0;
}

// void rowcreator(CellState x, CellState y, CellState z)
// {

// }

CellState cellgenerator(CellState x, CellState y, CellState z)
{
    if (x + y + z == 2)
    {
        return ON;
    }
    else
    {
        return OFF;
    }
}

void test(void)
{
    assert(cellgenerator(1, 1, 1) == OFF);
    assert(cellgenerator(1, 1, 0) == ON);
    assert(cellgenerator(OFF, OFF, OFF) == OFF);
}