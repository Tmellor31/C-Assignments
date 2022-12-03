#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h> 
#include <string.h>  


typedef struct lisp {
  struct lisp *car;
  struct lisp *cdr;
  atomtype atom; 
} lisp; 

typedef atomtype atomtype; 