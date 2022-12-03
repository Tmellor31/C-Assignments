#pragma once 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "../lisp.h" 
#define LISPIMPL "Linked" 

struct lisp
{
  const struct lisp *car;
  const struct lisp *cdr;
  atomtype atom;
};
