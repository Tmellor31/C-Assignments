#include "nuclei.h"
// Need to add function to free things, tons of leaks rn

void test(void);

int main(int argc, char *argv[])
{
    test();
    InputString *primary_input_string = make_input_string();

    if (argc <= 1)
    {
        printf("Please enter a file\n");
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen(argv[argc - 1], "r");
    if (fp == NULL)
    {
        printf("Cannot open %s\n", argv[argc - 1]);
        exit(EXIT_FAILURE);
    }
    if (!fp)
    {
        fprintf(stderr, "Cannot read from %s\n", argv[argc - 1]);
        exit(EXIT_FAILURE);
    }

    while (fgets(primary_input_string->array2d[primary_input_string->row_count],
                 MAXLINEWIDTH, fp) != NULL)
    {
        if (primary_input_string->array2d[primary_input_string->row_count][0] != COMMENT)
        {
            primary_input_string->row_count++;
        }
    }
    fclose(fp);
    PROG(primary_input_string);
    free(primary_input_string);
    return 0;
}

InputString *make_input_string(void)
{
    InputString *input_string = ncalloc(1, sizeof(InputString));
    input_string->row_count = 0;
#ifdef INTERP
    input_string->variable_count = 0;
#endif
    return input_string;
}



char current_position(InputString *input_string)
{
    return input_string->array2d[input_string->row][input_string->col];
}

void PROG(InputString *input_string)
{
    if (current_position(input_string) != OPEN_BRACKET)
    {
        printf("Was expecting a '('\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    INSTRCTS(input_string);
    printf("Parsed OK\n");
}

void INSTRCTS(InputString *input_string)
{
    if (current_position(input_string) == CLOSE_BRACKET)
    {
        get_next_char(input_string);
        return;
    }
#ifdef INTERP
    lisp *lisp = NULL;
    INSTRCT(&lisp, input_string);
#else
    INSTRCT(input_string);
#endif
    INSTRCTS(input_string);
}

#ifdef INTERP
void INSTRCT(lisp **lisp, InputString *input_string)
#else
void INSTRCT(InputString *input_string)
#endif
{
    if (current_position(input_string) != OPEN_BRACKET)
    {
        printf("Expected an '(' at row %i col %i \n", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);

#ifdef INTERP
    FUNC(lisp, input_string);
#else
    FUNC(input_string);
#endif
    if (current_position(input_string) != CLOSE_BRACKET)
    {
        printf("Expected an ')' at row %i col %i. Received '%c' \n", input_string->row, input_string->col, current_position(input_string));
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
}

#ifdef INTERP
void FUNC(lisp **lisp, InputString *input_string)
#else
void FUNC(InputString *input_string)
#endif
{
#ifdef INTERP
    bool haspassed = RETFUNC(lisp, input_string) ||
                     IOFUNC(input_string) ||
                     IF(input_string) ||
                     LOOP(input_string);
#else
    bool haspassed = RETFUNC(input_string) ||
                     IOFUNC(input_string) ||
                     IF(input_string) ||
                     LOOP(input_string);
#endif

    if (!haspassed)
    {
        printf("No RETFUNC, IOFUNC, IF or LOOP found within FUNC row %i col %i \n", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
}

#ifdef INTERP
bool RETFUNC(lisp **lisp, InputString *input_string)
{
    bool haspassed = LISTFUNC(lisp, input_string) ||
                     INTFUNC(lisp, input_string) ||
                     BOOLFUNC(lisp, input_string);

    return haspassed;
}
#else
bool RETFUNC(InputString *input_string)
{
    bool haspassed = LISTFUNC(input_string) ||
                     INTFUNC(input_string) ||
                     BOOLFUNC(input_string);

    return haspassed;
}
#endif

#ifdef INTERP
bool LISTFUNC(lisp **new_variable, InputString *input_string)
#else
bool LISTFUNC(InputString *input_string)
#endif
{
    if (is_at_start(input_string->array2d[input_string->row], "CAR", input_string->col))
    {
#ifdef INTERP
        *new_variable = CAR(input_string);
#else
        CAR(input_string);
#endif
        return true;
    }
    else if (is_at_start(input_string->array2d[input_string->row], "CDR", input_string->col))
    {
#ifdef INTERP
        *new_variable = CDR(input_string);
#else
        CDR(input_string);
#endif
        return true;
    }
    else if (is_at_start(input_string->array2d[input_string->row], "CONS", input_string->col))
    {
#ifdef INTERP
        *new_variable = CONS(input_string);
#else
        CONS(input_string);
#endif
        return true;
    }
    else
    {
        // No CAR,CDR or CONS found
#ifdef INTERP
        *new_variable = NULL;
#endif
        return false;
    }
}

#ifdef INTERP
lisp *CAR(InputString *input_string)
{
    input_string->col += strlen("CAR");
    get_next_char(input_string);
    lisp *list_value = LIST(input_string);
    return lisp_car(list_value);
}

lisp *CDR(InputString *input_string)
{
    input_string->col += strlen("CDR");
    get_next_char(input_string);
    lisp *list_value = LIST(input_string);
    char teststr[50] = "";
    lisp_tostring(lisp_cdr(list_value), teststr);
    return lisp_cdr(list_value);
}

lisp *CONS(InputString *input_string)
{
    input_string->col += strlen("CONS");
    get_next_char(input_string);
    lisp *firstlist = LIST(input_string);
    lisp *secondlist = LIST(input_string);
    return lisp_cons(firstlist, secondlist);
}
#else
void CAR(InputString *input_string)
{
    input_string->col += strlen("CAR");
    get_next_char(input_string);
    LIST(input_string);
}

void CDR(InputString *input_string)
{
    input_string->col += strlen("CDR");
    get_next_char(input_string);
    LIST(input_string);
}

void CONS(InputString *input_string)
{
    input_string->col += strlen("CONS");
    get_next_char(input_string);
    LIST(input_string);
    LIST(input_string);
}
#endif

#ifdef INTERP
bool INTFUNC(lisp **atom, InputString *input_string)
#else
bool INTFUNC(InputString *input_string)
#endif
{
    if (is_at_start(input_string->array2d[input_string->row], "PLUS", input_string->col))
    {
        input_string->col += strlen("PLUS");
        move_next_char(input_string);
#ifdef INTERP
        lisp *first = LIST(input_string);
        lisp *second = LIST(input_string);
        lisp_isatomic(first);
        lisp_isatomic(second);
        // interpreter here
#else
        LIST(input_string);
        LIST(input_string);
#endif
        return true;
    }

    if (is_at_start(input_string->array2d[input_string->row], "LENGTH", input_string->col))
    {
        input_string->col += strlen("LENGTH");
        move_next_char(input_string);
#ifdef INTERP
        lisp *list = LIST(input_string);
        lisp_isatomic(list);
        // interpreter here
#else
        LIST(input_string);
#endif
        return true;
    }
#ifdef INTERP
    *atom = NULL;
#endif
    return false;
}

#ifdef INTERP
bool BOOLFUNC(lisp **atom, InputString *input_string)
#else
bool BOOLFUNC(InputString *input_string)
#endif
{
    if (is_at_start(input_string->array2d[input_string->row], "LESS", input_string->col))
    {
        input_string->col += strlen("LESS");
        move_next_char(input_string);
#ifdef INTERP
        lisp *first = LIST(input_string);
        lisp *second = LIST(input_string);
        lisp_isatomic(first);
        lisp_isatomic(second);
#else
        LIST(input_string);
        LIST(input_string);
#endif
        return true;
    }
    if (is_at_start(input_string->array2d[input_string->row], "GREATER", input_string->col))
    {
        input_string->col += strlen("GREATER");
        move_next_char(input_string);
#ifdef INTERP
        lisp *first = LIST(input_string);
        lisp *second = LIST(input_string);
        lisp_isatomic(first);
        lisp_isatomic(second);
#else
        LIST(input_string);
        LIST(input_string);
#endif
        return true;
    }
    if (is_at_start(input_string->array2d[input_string->row], "EQUAL", input_string->col))
    {
        input_string->col += strlen("EQUAL");
        move_next_char(input_string);
#ifdef INTERP
        lisp *first = LIST(input_string);
        lisp *second = LIST(input_string);
        lisp_isatomic(first);
        lisp_isatomic(second);
#else
        LIST(input_string);
        LIST(input_string);
#endif

        return true;
    }
#ifdef INTERP
    *atom = NULL;
#endif
    return false;
}

bool IOFUNC(InputString *input_string)
{
    if (is_at_start(input_string->array2d[input_string->row], "SET", input_string->col))
    {
        SET(input_string);
        return true;
    }

    else if (is_at_start(input_string->array2d[input_string->row], "PRINT", input_string->col))
    {
        PRINT(input_string);
        return true;
    }

    else
    {
        // NO PRINT OR SET found
        return false;
    }
}

void SET(InputString *input_string)
{
    input_string->col += strlen("SET");
    move_next_char(input_string);
#ifdef INTERP
    char letter = VAR(input_string);
    lisp *list = LIST(input_string);
    add_variable(input_string, letter, list);
#else
    VAR(input_string);
    LIST(input_string);
#endif
}

#ifdef INTERP
void add_variable(InputString *input_string, char letter, lisp *var)
{
    input_string->variables[input_string->variable_count].value = var;
    input_string->variables[input_string->variable_count].name = letter;
    input_string->variable_count++;
}
#endif

void PRINT(InputString *input_string)
{
    input_string->col += strlen("PRINT");
    move_next_char(input_string);
    if (current_position(input_string) == '"')
    {
#ifdef INTERP
        char *string = NULL;
        STRING(string, input_string);
        puts(string);
#else
        STRING(input_string);
#endif
    }
    else
    {
#ifdef INTERP
        lisp *list = LIST(input_string);
        char print_string[MAXLINEWIDTH] = "";
        lisp_tostring(list, print_string);
        puts(print_string);
#else
        LIST(input_string);
#endif
    }
}

#ifdef INTERP
lisp *find_variable(InputString *input_string, char letter)
{
    for (int i = 0; i < input_string->variable_count; i++)
    {
        if (letter == input_string->variables[i].name)
        {
            return input_string->variables[i].value;
        }
    }
    return NULL;
}
#endif

#ifdef INTERP
char VAR(InputString *input_string)
#else
void VAR(InputString *input_string)
#endif
{
    if (current_position(input_string) >= 'A' &&
        current_position(input_string) <= 'Z')
    {
#ifdef INTERP
        char letter = current_position(input_string);
#endif
        move_next_char(input_string);
#ifdef INTERP
        return letter;
#endif
    }
    else
    {
        printf("Expected a character from A-Z at row %i col %i \n", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
}

bool is_at_start(char *inputstring, char *keyword, int inputposition)
{
    for (unsigned int i = 0; i < strlen(keyword); i++)
    {
        if (inputstring[inputposition + i] != keyword[i])
        {
            return false;
        }
    }
    return true;
}

bool end_of_file_reached(InputString *input_string)
{
    int row_length = strlen(input_string->array2d[input_string->row]);

    if (input_string->row == input_string->row_count - 1 &&
        input_string->col == row_length - 1)
    {
        // End of file reached
        return true;
    }

    else
    {
        return false;
    }
}

bool is_char(char target)
{
    return !isspace(target);
}

bool get_next_char(InputString *input_string)
{
    return find_next_target(input_string, &is_char);
}

// Checks if you can move to next char - if at end of the string it throws an error
void move_next_char(InputString *input_string)
{
    if (!get_next_char(input_string))
    {
        printf("Next character not found after line %i col %i\n", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
}

bool is_quote(char target)
{
    return target == '\'';
}

bool get_next_quote(InputString *input_string)
{
    return find_next_target(input_string, &is_quote);
}

bool find_next_target(InputString *input_string, bool (*char_matches)(char target))
{
    int col = input_string->col + 1;
    for (int row = input_string->row; row < input_string->row_count; row++)
    {
        int row_length = strlen(input_string->array2d[row]);
        for (; col < row_length; col++)
        {

            if (char_matches(input_string->array2d[row][col]))
            {
                // Successfully moved to next char
                input_string->row = row;
                input_string->col = col;
                return true;
            }
        }
        col = 0;
    }
    return false;
}

bool IF(InputString *input_string)
{
    if (!is_at_start(input_string->array2d[input_string->row], "IF", input_string->col))
    {
        return false;
    }
    input_string->col += strlen("IF");
    move_next_char(input_string);
    if (current_position(input_string) != '(')
    {
        printf("Expected a '(' after IF at row %i col %i \n", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
#ifdef INTERP
    lisp **lisp = NULL;
    if (!BOOLFUNC(lisp, input_string))
#else
    if (!BOOLFUNC(input_string))
#endif
    {
        printf("BOOLFUNC failed to parse\n");
        exit(EXIT_FAILURE);
    }
    if (current_position(input_string) != ')')
    {
        printf("Expected a ')' after BOOLFUNC\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    if (current_position(input_string) != '(')
    {
        printf("Expected a '(' after BOOLFUNC BEFORE INSTRCTS\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    INSTRCTS(input_string);
    if (current_position(input_string) != '(')
    {
        printf("Expected a '(' after IF\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    INSTRCTS(input_string);
    return true;
}

bool LOOP(InputString *input_string)
{
    if (!is_at_start(input_string->array2d[input_string->row], "WHILE", input_string->col))
    {
        return false;
    }
    input_string->col += strlen("WHILE");
    move_next_char(input_string);
    if (current_position(input_string) != '(')
    {
        printf("Expected a '(' after WHILE\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
#ifdef INTERP
    lisp **lisp = NULL;
    if (!BOOLFUNC(lisp, input_string))
#else
    if (!BOOLFUNC(input_string))
#endif
    {
        printf("BOOLFUNC failed to parse\n");
        exit(EXIT_FAILURE);
    }
    if (current_position(input_string) != ')')
    {
        printf("Expected a ')' after BOOLFUNC\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    if (current_position(input_string) != '(')
    {
        printf("Expected a '(' after BOOLFUNC and before INSTRCTS\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    INSTRCTS(input_string);
    return true;
}

#ifdef INTERP
lisp *LIST(InputString *input_string)
#else
void LIST(InputString *input_string)
#endif
{
    if (is_at_start(input_string->array2d[input_string->row], "NIL", input_string->col))
    {
        input_string->col += strlen("NIL");
#ifdef INTERP
        return NULL;
#endif
    }

    else if (current_position(input_string) >= 'A' &&
             current_position(input_string) <= 'Z')
    {
#ifdef INTERP
        char letter = VAR(input_string);
        lisp *value = find_variable(input_string, letter);
        if (value == NULL)
        {
            printf("variable %c is undefined at row %i col %i", letter, input_string->row, input_string->col);
            exit(EXIT_FAILURE);
        }
        return value;
#else
        VAR(input_string);
#endif
    }
    else if (current_position(input_string) == '\'')
    {
#ifdef INTERP
        return LITERAL(input_string);
#else
        LITERAL(input_string);
#endif
    }
    else if (current_position(input_string) == OPEN_BRACKET)
    {
        move_next_char(input_string);

#ifdef INTERP
        lisp *list_value = NULL;
        if (!RETFUNC(&list_value, input_string))
#else
        if (!RETFUNC(input_string))
#endif
        {
            printf("Expected a CAR, CONS or CDR? at row %i col %i\n", input_string->row, input_string->col);
            exit(EXIT_FAILURE);
        }

        if (current_position(input_string) != CLOSE_BRACKET)
        {
            printf("Expected a ')' at row %i col %i \n", input_string->row, input_string->col);
            exit(EXIT_FAILURE);
        }
        move_next_char(input_string);
#ifdef INTERP
        return list_value;
#endif
    }
    else
    {
        printf("Expected a LITERAL, 'NIL' or '(' at row %i, col %i", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
#ifdef INTERP
    return NULL;
#endif
}

#ifdef INTERP
void STRING(char *string, InputString *input_string)
#else
void STRING(InputString *input_string)
#endif
{
    if (current_position(input_string) != '"')
    {
        printf("Expected \" at the start of STRING\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
#ifdef INTERP
    char datastring[MAXLINEWIDTH] = "";
#endif
    int counter = 0;
    do
    {
#ifdef INTERP
        datastring[counter++] = current_position(input_string);
#else
        counter++;
#endif
        input_string->col++;
    } while (current_position(input_string) != '"');
#ifdef INTERP
    datastring[counter] = NUM;
    strcpy(string, datastring);
#endif
    move_next_char(input_string);
}

#ifdef INTERP
lisp *LITERAL(InputString *input_string)
#else
void LITERAL(InputString *input_string)
#endif
{
    if (current_position(input_string) != '\'')
    {
        printf("Expected a single quote at the start of Literal\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
#ifdef INTERP
    char literalstring[MAXLINEWIDTH] = "";
    int counter = 0;
#endif
    do
    {
        if (input_string->col >= (int)strlen(input_string->array2d[input_string->row]))
        {
            printf("Reached end of line %i. Expected closing \'", input_string->row);
            exit(EXIT_FAILURE);
        }
#ifdef INTERP
        literalstring[counter++] = current_position(input_string);
#endif
        input_string->col++;
    } while (!is_quote(current_position(input_string)));
#ifdef INTERP
    literalstring[counter] = NUM;
    lisp *literal_value = lisp_fromstring(literalstring);
#endif
    move_next_char(input_string);

#ifdef INTERP
    return literal_value;
#endif
}

/*lisp *literal_list(InputString *input_string)
{
    lisp *list = ncalloc(1, sizeof(Var));
    char current_char;

    do
    {
        move_next_char(input_string);
        current_char = current_position(input_string);
        if (current_char != CLOSE_BRACKET)
        {
            lisp *atom = lisp_atom(current_char - '0');
            lisp_cons(atom, NULL);
            list->car;
        }
    } while (current_char != CLOSE_BRACKET);

    return list;
}*/

void test(void)
{
    char test_input_str[] = "Fred likes samsung tvs";
    char test_keyword[] = "Fred";
    char test_wrong_keyword[] = "John";
    char test_fail[] = "likes";
    int test_input_position = 0;

    assert(is_at_start(test_input_str, test_keyword, test_input_position) == true);
    assert(is_at_start(test_input_str, test_wrong_keyword, test_input_position) == false);
    assert(is_at_start(test_input_str, test_fail, test_input_position) == false);
    assert(is_at_start(test_input_str, test_fail, 5) == true);
    assert(is_at_start(test_input_str, "likes", 5) == true);

    InputString *test_input_string = ncalloc(1, sizeof(InputString));

    strcpy(test_input_string->array2d[0], "Java");
    strcpy(test_input_string->array2d[1], "Python");
    strcpy(test_input_string->array2d[2], "C++' ");
    strcpy(test_input_string->array2d[3], "Coffee");
    strcpy(test_input_string->array2d[4], "Silver");

    test_input_string->row_count = 5;
    assert(end_of_file_reached(test_input_string) == false);
    test_input_string->row = 4;
    test_input_string->col = strlen(test_input_string->array2d[4]) - 1;
    assert(end_of_file_reached(test_input_string) == true);
    test_input_string->row = 2;
    test_input_string->col = 3;

    assert(test_input_string->array2d[test_input_string->row][test_input_string->col] == '\'');

    assert(get_next_char(test_input_string));
    assert(test_input_string->row == 3);
    assert(test_input_string->col == 0);
    (move_next_char(test_input_string));
    assert(test_input_string->row == 3);
    assert(test_input_string->col == 1);
    test_input_string->row = 2;
    assert(current_position(test_input_string) == '+');
    assert(is_quote('\'') == true);
    assert(is_quote('X') == false);
    find_next_target(test_input_string, &is_quote);
    assert(current_position(test_input_string) == '\'');

    free (test_input_string); 

    /*InputString *main_input_string = make_input_string();
    strcpy(main_input_string->array2d[main_input_string->row_count++], "(");
    strcpy(main_input_string->array2d[main_input_string->row_count++], " ( CAR '2' ) ");
    strcpy(main_input_string->array2d[main_input_string->row_count++], " (SET B '(1 2 3)' )");
    strcpy(main_input_string->array2d[main_input_string->row_count++], "");
    strcpy(main_input_string->array2d[main_input_string->row_count++], ")");
    PROG(main_input_string);
    free(main_input_string);*/
}
