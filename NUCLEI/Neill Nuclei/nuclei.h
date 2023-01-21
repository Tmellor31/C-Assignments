#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "general.h"
#include "specific.h"
#define MAXLINENO 50
#define MAXLINEWIDTH 50
#define MAXVARIABLES 50
#define MAXLISTSIZE 50
#define SPACE ' '
#define NEWLINE '\n'
#define OPEN_BRACKET '('
#define CLOSE_BRACKET ')'
#define COMMENT '#'

typedef struct Var
{
    lisp *value; 
    char name;
} Var;

typedef struct InputString
{
    char array2d[MAXLINENO][MAXLINEWIDTH];
    int row;
    int col;
    int row_count;
    Var variables[MAXVARIABLES];
    int variable_count;
} InputString;
