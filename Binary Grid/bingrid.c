#include "bingrid.h"

//need to rename the xcoord and ycoord to row and col respectively, and then make sure the ordering is consistent

// Retrieves the character from str that relates to xcoord and ycoord
char setcell(char *str, int grid_size, int xcoord, int ycoord)
{
    return str[xcoord + (grid_size * ycoord)];
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
    int x_coord, y_coord;
    for (x_coord = 0; x_coord < brd->sz; x_coord++)
    {
        for (y_coord = 0; y_coord < brd->sz; y_coord++)
        {
            brd->b2d[x_coord][y_coord] = setcell(str, brd->sz, x_coord, y_coord);
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
{
    return true;
}

void printboard(board *brd)
{
    int i, j;
    for (i = 0; i < brd->sz; i++)
    {
        for (j = 0; j < brd->sz; j++)
        {
            printf("i = %i, j = %i, %c\n", i, j, brd->b2d[i][j]);
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
