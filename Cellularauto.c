#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define COLUMNS 34
#define ROWS 30
#define GRIDSIZE 1020

int size[COLUMNS];

typedef enum CellState{OFF, ON} CellState;

void test(void);
CellState cellgenerator(CellState x, CellState y, CellState z);
void rowcreator(void);

int main(void)
{//need to create the grid via this method and run the digits array[] through cellgenerator. Excluding top line for user input. 
 test();
 rowcreator(); 

return 0;
}

void rowcreator(void)
{
  
 int firstline[GRIDSIZE] = {0}; // 34x30 grid
 int secondline[GRIDSIZE+ROWS]; //firstline + 30 = first character of second line 
 int row1;
 int row2; 

 for(row1 = 0; row1 < GRIDSIZE; row1++)
  {  
   row2 = row1 + ROWS; 
    if(cellgenerator((secondline[row2] - 31),(secondline[row2] -30),(secondline[row2] -29)) == ON)
      {
       printf("1");//prints either 0 or 1 based on firstline 
      } 
    else 
      {
       printf("0"); 
      } 
     if (secondline[row2] % 30 == 0 && secondline[row2] != 0) //makes a new line at 30 
     {
       printf("\n");
     } 	    
  }
}

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
