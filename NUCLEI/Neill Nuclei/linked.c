#include "specific.h"

#define NIL NULL 
#define BIGSTR 50 

// Non-extension code works and has no mem leaks when I run it. I've attempted to write lisp_reduce, but can't test it aganist testlisp.c -
// - as that requires the from string function which I haven't done.

void cons_tostring(const lisp *l, char *str);
void list_tostring(const lisp *l, char *str);
void new_times(lisp* l, atomtype* n);
void new_atoms(lisp* l, atomtype* n);
lisp* lisp_fromstring(const char* str);
bool check_bracks(const char* str);
bool check_chars(const char* str);
lisp* from(int s, const info i[]); 
void get_str_info(const char* str, info i[]);
int endbrack(int s, const info i[]);


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

lisp *lisp_car(const lisp *l)
{
    return l->car;
}

lisp *lisp_cdr(const lisp *l)
{
    return l->cdr;
}

atomtype lisp_getval(const lisp *l)
{

    return l->atom;
}

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

int lisp_length(const lisp *l)
{
    if (l == NULL)
    {
        return 0;
    }
    int length = 0;
    while (l != NULL)
    {
        if (!lisp_isatomic(l))
        {
            length++;
        }
        l = l->cdr;
    }

    return length;
}

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

void cons_tostring(const lisp *l, char *str)
{
    if (l == NULL)
    {
        return;
    }
    if (lisp_isatomic(l))
    {
        char atomstr[ATOMSTRSIZE] = "";
        sprintf(atomstr, "%i", lisp_getval(l));
        strcat(str, atomstr);
    }
    else
    {
        list_tostring(l->car, str);
        char space[] = " ";
        if (l->cdr != NULL)
        {
            strcat(str, space); // adds a space between numbers
        }
        cons_tostring(l->cdr, str);
    }
}

void list_tostring(const lisp *l, char *str) // returns stringified version of list
{
    if (l == NULL)
    {
        strcpy(str, "()");
        return;
    }
    else if (lisp_isatomic(l))
    {
        cons_tostring(l, str);
    }
    else
    {
        strcat(str, "(");
        cons_tostring(l, str);
        strcat(str, ")");
    }
}

// Clears the current string before the new list is then stringified
void lisp_tostring(const lisp *l, char *str)
{
    strcpy(str, ""); // clear string
    list_tostring(l, str);
}

void lisp_free(lisp **l)
{
    if (l == NULL)
    {
        return;
    }
    if (*l == NULL)
    {
        return;
    }
    if ((*l)->car != NULL)
    {
        lisp_free(&(*l)->car);
    }
    if ((*l)->cdr != NULL)
    {
        lisp_free(&(*l)->cdr);
    }
    free(*l);
    *l = NULL;
}

void new_times(lisp* l, atomtype* accum)
{
   *accum = *accum * lisp_getval(l);
}

void new_atoms(lisp* l, atomtype* accum)
{
   *accum = *accum + lisp_isatomic(l);
   }

/*void test(void)
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
    assert(lisp_getval(lisp_car(con1copy)) == 1);

    char str[MAXLISTLENGTH] = "";
    lisp_tostring(con1copy, str);

    lisp *l1 = lisp_cons(lisp_atom(2), NULL);
    char teststring[MAXLISTLENGTH];
    assert(lisp_length(l1) == 1);
    lisp_tostring(l1, teststring);
    assert(strcmp(teststring, "(2)") == 0);

    atomtype accum = 1;
    (lisp_reduce(new_times,l1,&accum)); 
    assert(accum == 4); 
    accum = 0; 
    (lisp_reduce(new_atoms,l1,&accum)); 
    assert(accum == 2); 



    lisp_free(&con1);
    lisp_free(&con1copy);
    lisp_free(&l1); 
    assert(con1 == NULL);
    assert(con1copy == NULL);
    assert(l1 == NULL); 
}*/

/* ------------- Tougher Ones : Extensions ---------------*/

//TAKEN FROM JAKE AS PER KIRAS COMMENT ON MICROSOFT TEAMS SAYING WE CAN
//BORROW CODE FROM PREVIOUS EXTENSIONS
lisp* lisp_fromstring(const char* str){
    //these checks are not comprehensive, but help prevent seg faults
    if(!check_bracks(str) || !check_chars(str) || strcmp(str,"") == 0){
        fprintf(stderr, "Bad input string: '%s'\n", str);
        return NIL;
    }

    info i[BIGSTR];
    get_str_info(str, i);

    if(i[0].t == NUM){
        return atom(i[0].a);
    }

    return from(1,i);
}


bool check_bracks(const char* str){
    //+1 when we open a bracket, -1 when we subsequently close it
    int brackets = 0;
    for(int i=0; i< (int) strlen(str); i++){
        //brackets<0 implies erroneous close bracket, ie ( () )( ) would be invalid
        if(brackets < 0){
            return false;
        }
        if(str[i] == '('){
            brackets++;
        }
        if(str[i] == ')'){
            brackets--;
        }
    }
    if(brackets != 0){
        return false;
    }
    return true;
}


bool check_chars(const char* str){
    for(int i=0; i< (int) strlen(str); i++){
        //chars must be space, ( , ) , - , or digits
        if(str[i] != ')' && str[i] != '(' && str[i] != ' ' && str[i] != '-'){
            if(str[i] > '9' || str[i] < '0'){
                return false;
            }
        }
    }
    return true;
}


lisp* from(int s, const info i[]){

    if(i[s].t == '('){
        //everything within a bracketed expression becomes the car. everything after becomes the cdr.
        return cons( from(s+1, i) , from(endbrack(s,i), i) );
    }

    else if(i[s].t == NUM){
        return cons( atom(i[s].a) , from(s+1, i) );
    }

    else{
        return NIL;
    }
}


int endbrack(int s, const info i[]){
    int x=1;
    while(x != 0){
        s++;
        if(i[s].t == '('){
            x++;
        }
        if(i[s].t == ')'){
            x--;
        }
    }
    return s+1;
}


void get_str_info(const char* str, info i[]){
    int j=0;
    info addinfo = {'(',0};

    for(int k=0; k < (int) strlen(str); k++){
        if(str[k] == '('){
            addinfo.t = '(';
            i[j] = addinfo;
            j++;
        }
        else if(str[k] == ')'){
            addinfo.t = ')';
            i[j] = addinfo;
            j++;
        }
        else if(str[k] == ' '){
            //ignore spaces
        }
        else{
            sscanf(&str[k], "%d", &addinfo.a);
            addinfo.t = NUM;
            i[j] = addinfo;
            j++;
            //cycle to end of num (could be multiple digits)
            while((str[k] >= '0' && str[k] <= '9') || str[k] == '-'){
                k++;
            }
            k--;
        }
    }

    return;
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
void lisp_reduce(void (*func)(lisp *l, atomtype *n), lisp *l, atomtype *accum)
{
    if(lisp_isatomic(l)){
        func(l, accum); 
        return;
    }
    if(l == NULL){
        return;
    }
    lisp_reduce(func, l->car, accum);
    lisp_reduce(func,l->cdr, accum); 
    return; 
}

