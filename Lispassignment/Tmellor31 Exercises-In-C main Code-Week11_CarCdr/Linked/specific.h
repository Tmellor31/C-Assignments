#pragma once 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "../lisp.h" 
#define LISPIMPL "Linked" 
#define UNBOUND -1 

struct lisp
{
  struct lisp *car;
  struct lisp *cdr;
  atomtype atom;
};
