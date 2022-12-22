#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "general.h"
#define MAXXSIZE 50
#define MAXYSIZE 50

typedef struct InputString
{
    char array2d[MAXXSIZE][MAXYSIZE];
    int y_position;
    int x_position;
}InputString;
