#include "bingrid.h"

#define BOARDSTR (MAX * MAX + 1)

void printboard(board *brd);
bool issolved(board *brd);
bool applyPairsToRow(board *brd, int row, int col);
bool applyPairsToCol(board *brd, int row, int col);
bool applyOXOToRow(board *brd, int row, int col);
bool applyOXOToCol(board *brd, int row, int col);
bool applyCountingToRow(board *brd, int row);
bool applyCountingToCol(board *brd, int col);

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
    if (strlen(str) % 2 != 0)
    {
        return false;
    }

    brd->sz = sqrt(strlen(str));
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
    int row, col;
    int k = 0;
    for (row = 0; row < brd->sz; row++)
    {
        for (col = 0; col < brd->sz; col++)
        {
            str[k] = brd->b2d[row][col];
            k++;
        }
    }
    str[k] = '\0';
}

bool issolved(board *brd)
{
    for (int row = 0; row < brd->sz; row++)
    {
        for (int col = 0; col < brd->sz; col++)
        {
            if (brd->b2d[row][col] == UNK)
            {
                return false;
            }
        }
    }
    return true;
}

bool solve_board(board *brd)
{
    bool haschanged = true;
    while (haschanged == true)
    {
        haschanged = false;
        for (int row = 0; row < brd->sz; row++)
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
    }
    return issolved(brd);
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
    if (brd->b2d[row][col] == brd->b2d[row][col + 1] && brd->b2d[row][col] != UNK)
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

bool applyPairsToCol(board *brd, int row, int col)
{
    bool haschanged = false;
    if (brd->b2d[row][col] == brd->b2d[row + 1][col] && brd->b2d[row][col] != UNK)
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
    if (brd->b2d[row][col] != brd->b2d[row][col + 2])
    {
        return false;
    }
    if (brd->b2d[row][col] == UNK)
    {
        return false;
    }
    if (col >= brd->sz - 1)
    {
        return false;
    }
    if (brd->b2d[row][col + 1] != UNK)
    {
        return false;
    }
    brd->b2d[row][col + 1] = oppositeNumber(brd->b2d[row][col + 2]);
    return true;
}

bool applyOXOToCol(board *brd, int row, int col)
{
    if ((brd->b2d[row][col] != brd->b2d[row + 2][col]))
    {
        return false;
    }
    if (brd->b2d[row][col] == UNK)
    {
        return false;
    }
    if (brd->b2d[row + 1][col] != UNK)
    {
        return false;
    }
    if (row >= brd->sz - 1)
    {
        return false;
    }
    brd->b2d[row + 1][col] = oppositeNumber(brd->b2d[row][col]);
    return true;
}

bool applyCountingToCol(board *brd, int col)
{
    int onecounter = 0;
    int zerocounter = 0;
    bool haschanged = false;

    for (int row = 0; row < brd->sz; row++)
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

    for (int row = 0; row < brd->sz; row++)
    {
        if (onecounter >= brd->sz / 2)
        {
            if (brd->b2d[row][col] == UNK)
            {
                brd->b2d[row][col] = ZERO;
                haschanged = true;
            }
        }
        if (zerocounter >= brd->sz / 2)
        {
            if (brd->b2d[row][col] == UNK)
            {
                brd->b2d[row][col] = ONE;
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

    for (int col = 0; col < brd->sz; col++)
    {
        if (onecounter >= brd->sz / 2)
        {
            if (brd->b2d[row][col] == UNK)
            {
                brd->b2d[row][col] = ZERO;
                haschanged = true;
            }
        }
        if (zerocounter >= brd->sz / 2)
        {
            if (brd->b2d[row][col] == UNK)
            {
                brd->b2d[row][col] = ONE;
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
    board b;
    char str[BOARDSTR];
    assert(str2board(&b, ".0.."));
    assert(b.sz == 2);
    assert(b.b2d[0][0] == UNK);
    assert(b.b2d[0][1] == ZERO);
    assert(b.b2d[1][0] == UNK);
    assert(b.b2d[1][1] == UNK);
    assert(applyCountingToRow(&b, 0) == true);

    assert(str2board(&b, "1001"));
    assert(b.b2d[0][0] == ONE);
    assert(b.b2d[0][1] == ZERO);
    assert(b.b2d[1][0] == ZERO);
    assert(b.b2d[1][1] == ONE);

    assert(oppositeNumber(ONE) == ZERO);
    assert(oppositeNumber(ZERO) == ONE);
    assert(oppositeNumber(UNK) == UNK);

    assert(str2board(&b, "....0.0....1..0."));
    assert(applyOXOToRow(&b, 1, 0) == true);
    assert(issolved(&b) == false);
    board2str(str, &b);
    assert(strcmp(str, "....010....1..0.") == 0);

    assert(str2board(&b, "1...1...0...0.01.1.1....0..0.1..1..."));
    assert(b.sz == 6);
    assert(solve_board(&b) == true);
    board2str(str, &b);
    assert(strcmp(str, "101010100101010101111010000011001100") == 0);
}
