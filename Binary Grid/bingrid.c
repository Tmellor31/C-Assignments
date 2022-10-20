#include "bingrid.h"

#define BOARDSTR (MAX * MAX + 1)

// While loop that stops when haschanged = false.
// In solve board - need a 'has there been an overall change, and has there been a change in this run. Stop when there hasnt been a change and return when there has been a change.
// At the end of board to string need to cut off the end(truncate) to make the string a certain size.

bool applyPairsToRow(board *brd, int row, int col);
bool applyPairsToCol(board *brd, int row, int col);
bool applyOXOToRow(board *brd, int row, int col);
bool applyOXOToCol(board *brd, int row, int col);
bool applyCountingToRow(board *brd, int row);
bool applyCountingToCol(board *brd, int col);

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
{ // for loop that turns board into string, and then pops a null character on the end.
    int row, col;
    int k = 0;
    for (row = 0; row < brd->sz; row++)
    {
        for (col = 0; col < brd->sz; col++)
        {
            str[k] = brd->b2d[row][col];
            k++;
            str[k] = '\0';
            printf("%s\n", str);
        }
    }
    printf("%d\n", brd->sz);
    str[k] = '\0';
    printf("%s", str);
}

bool solve_board(board *brd) // Once functions stop returning true, can cancel the third for loop.
{
    bool haschanged = false;
    // Checks for 'Pairs' rule
    // printboard(brd);
    for (int row = 0; row < brd->sz; row++) // Could use a dowhile loop instead of &&haschanged
    {
        for (int col = 0; col < brd->sz; col++)
        {
            haschanged |= applyPairsToRow(brd, row, col);
            haschanged |= applyPairsToCol(brd, row, col);
            haschanged |= applyOXOToRow(brd, row, col);
            haschanged |= applyOXOToCol(brd, row, col);
            haschanged |= applyCountingToCol(brd, col);
        }
        haschanged |= applyCountingToRow(brd, row);
    }
    return haschanged;
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
    bool haschanged = false;
    /*These functions should eventually return true if cells have changed and false if not*/

    if (brd->b2d[row][col] == brd->b2d[row][col + 1])
    {
        if ((col > 0) && (brd->b2d[row][col - 1] == UNK))
        {
            brd->b2d[row][col - 1] = oppositeNumber(brd->b2d[row][col]);
            haschanged = true;
        }
        if ((col < brd->sz) && (brd->b2d[row][col + 2] == UNK))
        {
            brd->b2d[row][col + 2] = oppositeNumber(brd->b2d[row][col]);
            haschanged = true;
        }
    }
    return haschanged;
}

bool applyPairsToCol(board *brd, int row, int col) // sort this outttttttt
{
    bool haschanged = false;
    if (brd->b2d[row][col] == brd->b2d[row + 1][col])
    {
        if ((row > 0) && (brd->b2d[row - 1][col] == UNK))
        {
            brd->b2d[row - 1][col] = oppositeNumber(brd->b2d[row][col]);
            haschanged = true;
        }
        if ((row < brd->sz) && (brd->b2d[row + 2][col] == UNK))
        {
            brd->b2d[row + 2][col] = oppositeNumber(brd->b2d[row][col]);
            haschanged = true;
        }
    }

    return haschanged;
}

bool applyOXOToRow(board *brd, int row, int col)
{
    if ((brd->b2d[row][col] == brd->b2d[row][col + 2]) && (brd->b2d[row][col + 1] == UNK) && (row < brd->sz - 1))
    {
        brd->b2d[row][col + 1] = oppositeNumber(brd->b2d[row][col + 2]);
        return true;
    }
    return false;
}

bool applyOXOToCol(board *brd, int row, int col)
{
    if ((brd->b2d[row][col] == brd->b2d[row + 2][col]) && (brd->b2d[row + 1][col] == UNK) && (row < brd->sz - 1))
    {
        brd->b2d[row + 1][col] = oppositeNumber(brd->b2d[row + 1][col]);
        return true;
    }
    return false;
}

bool applyCountingToCol(board *brd, int col)
{
    int onecounter = 0;
    int zerocounter = 0;
    bool haschanged = false;

    for (int i = 0; i < brd->sz; i++)
    {
        if (brd->b2d[i][col] == ONE)
        {
            onecounter++;
        }
        if (brd->b2d[i][col] == ZERO)
        {
            zerocounter++;
        }
    }

    for (int i = 0; i < brd->sz; i++)
    {
        if (onecounter >= brd->sz / 2)
        {
            if (brd->b2d[i][col] == UNK)
            {
                brd->b2d[i][col] = ZERO;
                haschanged = true;
            }
        }
        if (zerocounter >= brd->sz / 2)
        {
            if (brd->b2d[i][col] == UNK)
            {
                brd->b2d[i][col] = ONE;
                haschanged = true;
            }
        }
    }
    return haschanged;
}

bool applyCountingToRow(board *brd, int row)
{
    int onecounter = 0;
    int zerocounter = 0;
    bool haschanged = false;

    for (int col = 0; col < brd->sz; col++)
    {
        if (brd->b2d[row][col] == ONE)
        {
            onecounter++;
        }
        if (brd->b2d[row][col] == ZERO)
        {
            zerocounter++;
        }
    }

    for (int i = 0; i < brd->sz; i++)
    {
        if (onecounter >= brd->sz / 2)
        {
            if (brd->b2d[row][i] == UNK)
            {
                brd->b2d[row][i] = ZERO;
                haschanged = true;
            }
        }
        if (zerocounter >= brd->sz / 2)
        {
            if (brd->b2d[row][i] == UNK)
            {
                brd->b2d[row][i] = ONE;
                haschanged = true;
            }
        }
    }
    return haschanged;
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
    // assert str2board with a string of 12 characters, it should fail due to sqrt. b.sz should equal 0.
    // assert(str2board(&b, "111111111111"));
    assert(oppositeNumber(ONE) == ZERO);
    assert(oppositeNumber(ZERO) == ONE);
    assert(oppositeNumber(UNK) == UNK);
    board b;
    b.sz = 4;
    char str[BOARDSTR];

    assert(str2board(&b, ".11.00..0......."));
    assert(b.sz == 4);
    assert(applyPairsToCol(&b, 1, 0) == true);
    assert(applyOXOToRow(&b, 1, 1) == false);
    assert(b.b2d[0][0] == ONE);
    assert(b.b2d[3][0] == ONE);
    assert(str2board(&b, ".0.."));
    assert(b.sz == 2);
    assert(b.b2d[0][0] == UNK);
    assert(b.b2d[0][1] == ZERO);
    assert(b.b2d[1][0] == UNK);
    assert(b.b2d[1][1] == UNK);
    // assert(solve_board(&b) == true);
    // printboard(&b);
    assert(b.b2d[0][0] == UNK);
    assert(b.b2d[0][1] == ZERO);
    assert(b.b2d[1][0] == UNK);
    assert(b.b2d[1][1] == UNK);
    // board2str(str, &b);
    assert(applyCountingToRow(&b, 0) == true);
    // assert(solve_board(&b) == true);
    assert(b.b2d[0][0] == ONE);
    assert(b.b2d[0][1] == ZERO);
    assert(b.b2d[1][0] == UNK);
    assert(b.b2d[1][1] == UNK);
    assert(solve_board(&b) == true);
    assert(solve_board(&b) == false);
    (board2str(str, &b));

    assert(strcmp(str, "1001") == 0);
}
