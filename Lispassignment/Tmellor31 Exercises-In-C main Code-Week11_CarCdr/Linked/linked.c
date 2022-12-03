#include "specific.h"

// Returns element 'a' - this is not a list, and
// by itelf would be printed as e.g. "3", and not "(3)"
lisp *lisp_atom(const atomtype a)
{
    lisp *cons = malloc(sizeof(lisp));
    if (cons == NULL)
    {
        exit(EXIT_FAILURE);
    }
    cons->atom = a;
    return cons;
}

// Returns a list containing the car as 'l1'
// and the cdr as 'l2'- data in 'l1' and 'l2' are reused,
// and not copied. Either 'l1' and/or 'l2' can be NULL
lisp *lisp_cons(const lisp *l1, const lisp *l2)
{
    lisp *cons = malloc(sizeof(lisp));
    if (cons == NULL)
    {
        exit(EXIT_FAILURE);
    }
    cons->car = l1;
    cons->cdr = l2; 
    return cons;
}

// Returns number of components in the list.
int lisp_length(const lisp *l)
{
    int length;
    do
    {
        length++;

    } while (l != NULL);

    return length;
}
