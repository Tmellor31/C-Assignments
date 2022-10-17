#include "bingrid.h"

//need to rename the row and col to row and col respectively, and then make sure the ordering is consistent - will change at end

void printboard(board *brd);
// Retrieves the character from str that relates to row and col
char setcell(char *str, int grid_size, int row, int col)
{
    return str[row + (grid_size * col)];
}

bool str2board(board *brd, char *str)
{
    if (strlen(str) == 0)
    {
        return false;
    }
    if (strlen(str) % 2 != 0) // Checks if string length is even, returns false otherwise
    {
        return false;
    }

    brd->sz = strlen(str) / 2;
    // set the size of b2d
    // populate b2d
    int row, col;
    for (row = 0; row < brd->sz; row++)
    {
        for (col = 0; col < brd->sz; col++)
        {
            brd->b2d[row][col] = setcell(str, brd->sz, row, col);
        }
    }
    return true;
}

void board2str(char *str, board *brd)
{
    if (brd->sz == 0)
    {
        strcpy(str, "");
    }
}

bool solve_board(board *brd)
{   bool haschanged = false;  
    //Checks for 'Pairs' rule
    printboard(brd);  
    int row, col; 
    for (row = 0; row < brd->sz; row++) // Could use a dowhile loop instead of &&haschanged
    {
        for (col = 0; col < brd->sz; col++)
        {
          if((brd->b2d[row][col] ==  1) && (brd->b2d[row][col+1] == 1)) 
          {
            brd->b2d[row+1][col] = 0;
            brd->b2d[row+1][col+1] = 0;
            haschanged = true;  
            return true; 
          }
          if((brd->b2d[row+1][col] ==  1) && (brd->b2d[row+1][col+1] == 1) && (haschanged == false))  
          {
            brd->b2d[row+1][col] = 0;
            brd->b2d[row+1][col+1] = 0;
            haschanged = true;  
            return true; 
          } 
          if((brd->b2d[row][col] ==  1) && (brd->b2d[row+1][col] == 1) && (haschanged == false))  
          {
            brd->b2d[row][col+1] = 1;
            brd->b2d[row+1][col+1] = 1;
            haschanged = true;  
            return true; 
          }
          if((brd->b2d[row][col] ==  0) && (brd->b2d[row][col+1] == 0) && (haschanged == false))  
          {
            brd->b2d[row+1][col] = 1;
            brd->b2d[row+1][col+1] = 1;
            haschanged = true;  
            return true;
          }  
          if((brd->b2d[row+1][col] ==  0) && (brd->b2d[row+1][col+1] == 0) && (haschanged == false))  
          {
            brd->b2d[row][col] = 1;
            brd->b2d[row][col+1] = 1;
            haschanged = true;  
            return true;
          }  
          
return false;    
        } 
    }   
}

void printboard(board *brd)
{
    int i, j;
    for (i = 0; i < brd->sz; i++)
    {
        for (j = 0; j < brd->sz; j++)
        {
            printf("row = %i, col = %i, %c\n", i, j, brd->b2d[i][j]);
        }
    }
}

void test(void)
{
    board b;
    b.sz = 0;
    char str[0];
    assert(str2board(&b, ".0.."));
    assert(b.sz == 2);
    assert(b.b2d[0][0] == UNK);
    assert(b.b2d[0][1] == UNK);
    assert(b.b2d[1][0] == ZERO);
    assert(b.b2d[1][1] == UNK);
    assert(solve_board(&b) == true);
    board2str(str, &b);
    assert(strcmp(str, "1001") == 0);
}

// int main(void)
// {
//     printf("Hi\n");
//     board board;
//     printf("%i\n", board.sz);
//     printf("%i\n", board.b2d[0][MAX - 1]);
//     return 0;
// }
