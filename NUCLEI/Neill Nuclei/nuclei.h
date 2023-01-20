#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "general.h"
#define MAXXSIZE 50
#define MAXYSIZE 50
#define SPACE ' '
#define NEWLINE '\n'
#define OBRACKET '('
#define CBRACKET ')' 

typedef struct InputString
{
    char array2d[MAXXSIZE][MAXYSIZE];
    int y_position;
    int x_position;
    int row_count;
} InputString;
