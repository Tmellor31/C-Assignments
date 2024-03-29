#pragma once 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "lisp.h" 
#define LISPIMPL "Linked" 
#define UNBOUND -1 
#define ATOMSTRSIZE 50
#define MAXLISTLENGTH 100
#define NUM '\0'

#define atom(X)       lisp_atom(X)
#define cons(X, Y)    lisp_cons(X, Y)
#define car(L)        lisp_car(L)
#define cdr(L)        lisp_cdr(L)
#define copy(L)       lisp_copy(L)
#define fromstring(L) lisp_fromstring(L)

struct lisp
{
  struct lisp *car;
  struct lisp *cdr;
  atomtype atom;
};


//JAKE ROBERTS CODE - AS PER COMMENT FROM KIRA THAT WE CAN USE OLD ASSESSMENT CODE FROM OTHERS
//HIS EMAIL ci19032@bristol.ac.uk
//use info when we want a variable that can be a char OR an int
//when we want info to be an int, t = NUM
//otherwise t is whatever character we want saved
struct info{
    //type
    char t;
    //atom value (only relevant if t=NUM)
    atomtype a;
};

typedef struct info info;

