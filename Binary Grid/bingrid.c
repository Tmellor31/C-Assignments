#include "bingrid.h"

char setcell(char *str, int xcoord, int ycoord)
{
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
    char newboard[2][2];
    int x_coord, y_coord;
    for (x_coord = 0; x_coord < 2; x_coord++)
    {
        for (y_coord = 0; y_coord < 2; y_coord++)
        {
            newboard[x_coord][y_coord] = setcell(str, x_coord, y_coord);
        }
    }
    brd->b2d return true;
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
}

void test(void)
{
    board b;
    b.sz = 0;
    char str[0];
    assert(str2board(&b, ".0.."));
    assert(b.sz == 2);
    assert(b.b2d[0][0] == UNK);
    assert(b.b2d[0][1] == ZERO);
    assert(b.b2d[1][0] == UNK);
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
