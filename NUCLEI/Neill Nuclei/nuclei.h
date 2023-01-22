#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#ifdef INTERP
#  include "general.h"
#  include "specific.h"
#endif
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


void PROG(InputString *input_string);
void INSTRCTS(InputString *input_string);

#ifdef INTERP
void INSTRCT(lisp **lisp, InputString *input_string);
void FUNC(lisp **lisp, InputString *input_string);
bool RETFUNC(lisp **lisp, InputString *input_string);
bool LISTFUNC(lisp **new_variable, InputString *input_string);
lisp *CAR(InputString *input_string);
lisp *CDR(InputString *input_string);
lisp *CONS(InputString *input_string);
bool INTFUNC(lisp **atom, InputString *input_string);
bool BOOLFUNC(lisp **atom, InputString *input_string);
#else
void INSTRCT(InputString *input_string);
void FUNC(InputString *input_string);
bool RETFUNC(InputString *input_string);
bool LISTFUNC(InputString *input_string);
void CAR(InputString *input_string);
void CDR(InputString *input_string);
void CONS(InputString *input_string);
bool INTFUNC(InputString *input_string);
bool BOOLFUNC(InputString *input_string);
#endif

bool IOFUNC(InputString *input_string);
void SET(InputString *input_string);
void PRINT(InputString *input_string);
bool IF(InputString *input_string);
bool LOOP(InputString *input_string);

#ifdef INTERP
lisp *LIST(InputString *input_string);
#else
void LIST(InputString *input_string);
#endif

char VAR(InputString *input_string);
void STRING(char *string, InputString *input_string);

#ifdef INTERP
lisp *LITERAL(InputString *input_string);
#else
void LITERAL(InputString *input_string); 
#endif

Var *literal_list(InputString *input_string);
Var *literal_digit(InputString *input_string);

#ifdef INTERP
void add_variable(InputString *input_string, char letter, lisp *var);
lisp *find_variable(InputString *input_string, char letter);
#else
void add_variable(InputString *input_string, char letter);
void find_variable(InputString *input_string, char letter); 
#endif

InputString *make_input_string(void);
bool is_at_start(char *inputstring, char *keyword, int inputposition);
char current_position(InputString *input_string);
bool end_of_file_reached(InputString *input_string);
bool is_char(char target);
bool get_next_char(InputString *input_string);
void move_next_char(InputString *input_string);
bool find_next_target(InputString *input_string, bool (*char_matches)(char target));
