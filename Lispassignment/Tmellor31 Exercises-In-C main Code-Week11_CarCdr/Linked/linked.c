#include "specific.h"

// things to think about - how to write lisptostr so it converts con1 (in the test) to (1 2).

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
    cons->car = NULL;
    cons->cdr = NULL;
    return cons;
}

// Returns a list containing the car as 'l1'
// and the cdr as 'l2'- data in 'l1' and 'l2' are reused,
// and not copied. Either 'l1' and/or 'l2' can be NULL
lisp *lisp_cons(const lisp *l1, const lisp *l2)
{
    lisp *cons = (lisp *)malloc(sizeof(lisp));
    if (cons == NULL)
    {
        exit(EXIT_FAILURE);
    }
    cons->car = (lisp *)l1;
    cons->cdr = (lisp *)l2;
    cons->atom = UNBOUND;
    return cons;
}

// Returns the car (1st) component of the list 'l'.
// Does not copy any data.
lisp *lisp_car(const lisp *l)
{
    return l->car;
}

// Returns the cdr (all but the 1st) component of the list 'l'.
// Does not copy any data.
lisp *lisp_cdr(const lisp *l)
{
    return l->cdr;
}

// Returns the data/value stored in the cons 'l'
atomtype lisp_getval(const lisp *l)
{

    return l->atom;
}

// Returns a deep copy of the list 'l'
// How deep?
lisp *lisp_copy(const lisp *l)
{
    if (l == NULL)
    {
        return NULL;
    }

    lisp *cons = lisp_atom(l->atom);
    cons->car = lisp_copy(l->car);
    cons->cdr = lisp_copy(l->cdr);
    return cons;
}

// Returns number of components in the list.
int lisp_length(const lisp *l)
{
    if (l == NULL)
    {
        return 0;
    }
    int length = 0;
    while (l->cdr != NULL)
    {
        length++;
        l = l->cdr;
    }
    return length;
}

// Returns a boolean depending up whether l points to an atom (not a list)
bool lisp_isatomic(const lisp *l)
{
    if (l == NULL)
    {
        return false;
    }
    if (l->car == NULL && l->cdr == NULL)
    {
        return true;
    }
    return false;
}

// Returns stringified version of list
void lisp_tostring(const lisp *l, char *str)
{
    if (l == NULL)
    {
        strcpy(str, "()");
        return;
    }
    (void)str;
}

// Clears up all space used
// Double pointer allows function to set '*l' to NULL on success
void lisp_free(lisp **l)
{
    (void)l;
}

void test(void)
{
    lisp *atom1 = lisp_atom(1);
    lisp *atom2 = lisp_atom(2);
    lisp *con2 = lisp_cons(atom2, NULL);
    lisp *con1 = lisp_cons(atom1, con2);

    assert(lisp_car(con1) == atom1);
    assert(lisp_car(con2) == atom2);
    assert(lisp_getval(con1) == UNBOUND);
    assert(lisp_getval(con2) == UNBOUND);
    assert(lisp_getval(atom1) == 1);
    assert(lisp_getval(atom2) == 2);

    assert(lisp_isatomic(atom1) == true);
    assert(lisp_isatomic(con1) == false);

    lisp *con1copy = lisp_copy(con1);
    assert(lisp_isatomic(con1copy) == false);
    assert(lisp_getval(con1copy) == UNBOUND);
    assert(lisp_car(con1copy) != atom1);
    assert(lisp_car(con1copy) != NULL);
    assert(lisp_isatomic(lisp_car(con1copy)) == true);
    assert(lisp_getval(lisp_car(con1copy)) == 1); // Is this a magic number?
}

/* ------------- Tougher Ones : Extensions ---------------*/

// Builds a new list based on the string 'str'
lisp *lisp_fromstring(const char *str)
{
    (void)str;
    return lisp_atom(3);
}

// Returns a new list from a set of existing lists.
// A variable number 'n' lists are used.
// Data in existing lists are reused, and not copied.
// You need to understand 'varargs' for this.
lisp *lisp_list(const int n, ...)
{
    (void)n;
    return lisp_atom(3);
}

// Allow a user defined function 'func' to be applied to
// each component of the list 'l'.
// The user-defined 'func' is passed a pointer to a cons,
// and will maintain an accumulator of the result.
void lisp_reduce(void (*func)(lisp *l, atomtype *n), lisp *l, atomtype *acc)
{
    func(NULL, NULL);
    (void)l;
    (void)acc;
}