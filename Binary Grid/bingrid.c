#include "bingrid.h"

bool applyPairsToRow(board *brd, int row, int col);
bool applyPairsToCol(board *brd, int row, int col);

// need to rename the row and col to row and col respectively, and then make sure the ordering is consistent - will change at end

void printboard(board *brd);
// Retrieves the character from str that relates to row and col
char setcell(char *str, int grid_size, int row, int col)
{
    return str[col + (grid_size * row)];
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

    brd->sz = sqrt(strlen(str));
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

bool solve_board(board *brd) //Should return true if board is solvable 
{
    bool haschanged = false;
    // Checks for 'Pairs' rule
    printboard(brd);
    int row, col;
    for (row = 0; row < brd->sz; row++) // Could use a dowhile loop instead of &&haschanged
    {
        for (col = 0; col < brd->sz; col++)
        {
            applyPairsToRow(brd, row, col);
            applyPairsToCol(brd, row, col);
        }
    }
    return true;
}

char oppositeNumber(char x)
{
    if (x == ONE)
    {
        return ZERO;
    }
    if (x == ZERO)
    {
        return ONE;
    }
    else
    {
        return UNK;
    }
}

bool applyPairsToRow(board *brd, int row, int col)
{
    /*These functions should eventually return true if cells have changed and false if not*/

    if (brd->b2d[row][col] == brd->b2d[row][col + 1])
    {
        if ((col > 0) && (brd->b2d[row][col - 1] == UNK))
        {
            brd->b2d[row][col - 1] = oppositeNumber(brd->b2d[row][col]);
        }
        if ((col < brd->sz) && (brd->b2d[row][col + 2] == UNK))
        {
            brd->b2d[row][col + 2] = oppositeNumber(brd->b2d[row][col]);
        }
    }
    return true;
}

bool applyPairsToCol(board *brd, int row, int col) // sort this outttttttt
{
    if (brd->b2d[row][col] == brd->b2d[row + 1][col])
    {
        if ((row > 0) && (brd->b2d[row - 1][col] == UNK))
        {
            brd->b2d[row - 1][col] = oppositeNumber(brd->b2d[row][col]);
        }
        if ((row < brd->sz) && (brd->b2d[row + 2][col] == UNK))
        {
            brd->b2d[row + 2][col] = oppositeNumber(brd->b2d[row][col]);
            printf("Jay\n"); 
        }
    }
    return true;
}

void printboard(board *brd)
{
    int row, col;
    for (row = 0; row < brd->sz; row++)
    {
        for (col = 0; col < brd->sz; col++)
        {
            printf("row = %i, col = %i, %c\n", row, col, brd->b2d[row][col]);
        }
    }
}

void test(void)
{
    //assert str2board with a string of 12 characters, it should fail due to sqrt. b.sz should equal 0. 
    assert(oppositeNumber(ONE) == ZERO);
    assert(oppositeNumber(ZERO) == ONE);
    assert(oppositeNumber(UNK) == UNK);
    board b;
    b.sz = 0;
    char str[0];
    assert(str2board(&b, ".11.00..0......."));
    assert(b.sz == 4); 
    assert(applyPairsToCol(&b, 1, 0) == true);
    assert(b.b2d[0][0] == ONE);
    assert(b.b2d[3][0] == ONE);
    printboard(&b);
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
