#include "nuclei.h"
// Need to add function to free things, tons of leaks rn

void PROG(InputString *input_string);
void INSTRCTS(InputString *input_string);
void INSTRCT(InputString *input_string);
bool LISTFUNC(lisp **new_variable, InputString *input_string);
lisp *CAR(InputString *input_string);
lisp *CDR(InputString *input_string);
lisp *CONS(InputString *input_string);
bool IOFUNC(InputString *input_string);
lisp *LIST(InputString *input_string);
char VAR(InputString *input_string);
lisp *LITERAL(InputString *input_string);
Var *literal_list(InputString *input_string);
Var *literal_digit(InputString *input_string);
void SET(InputString *input_string);
void add_variable(InputString *input_string, char letter, lisp *var);
void PRINT(InputString *input_string);
lisp *find_variable(InputString *input_string, char letter);
InputString *make_input_string(void);
bool is_at_start(char *inputstring, char *keyword, int inputposition);
char current_position(InputString *input_string);
bool end_of_file_reached(InputString *input_string);
bool is_char(char target);
bool get_next_char(InputString *input_string);
void move_next_char(InputString *input_string);
bool find_next_target(InputString *input_string, bool (*char_matches)(char target));

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

    while (fgets(primary_input_string->array2d[primary_input_string->row_count], MAXLINEWIDTH, fp) != NULL)
    {
        if (primary_input_string->array2d[primary_input_string->row_count][0] != COMMENT)
        {
            primary_input_string->row_count++;
        }
    }
    fclose(fp);
    PROG(primary_input_string);
    InputString *main_input_string = make_input_string();
    strcpy(main_input_string->array2d[main_input_string->row_count++], "(");
    strcpy(main_input_string->array2d[main_input_string->row_count++], " ( CAR '2' ) ");
    strcpy(main_input_string->array2d[main_input_string->row_count++], " (SET B '(1 2 3)' )");
    strcpy(main_input_string->array2d[main_input_string->row_count++], "");
    strcpy(main_input_string->array2d[main_input_string->row_count++], ")");
    PROG(main_input_string);

    free(primary_input_string);
    free(main_input_string);

    return 0;
}

InputString *make_input_string(void)
{
    InputString *input_string = ncalloc(1, sizeof(InputString));
    input_string->row_count = 0;
    input_string->variable_count = 0;
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
}

void INSTRCTS(InputString *input_string)
{
    if (current_position(input_string) == CLOSE_BRACKET)
    {
        get_next_char(input_string);
        return;
    }
    INSTRCT(input_string);
    INSTRCTS(input_string);
}

void INSTRCT(InputString *input_string)
{
    if (current_position(input_string) != OPEN_BRACKET)
    {
        printf("Expected an '(' at row %i col %i \n", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);

    FUNC(input_string);
    if (current_position(input_string) != CLOSE_BRACKET)
    {
        printf("Expected an ')' at row %i col %i. Received '%c' \n", input_string->row, input_string->col, current_position(input_string));
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
}

void FUNC(InputString *input_string)
{
    bool haspassed = RETFUNC(input_string) ||
                     IOFUNC(input_string) ||
                     IF(input_string) ||
                     LOOP(input_string);

    if (!haspassed)
    {
        printf("No RETFUNC, IOFUNC, IF or LOOP found within FUNC\n");
        exit(EXIT_FAILURE);
    }
}

bool RETFUNC(lisp *new_variable, InputString *input_string)
{
    bool haspassed = LISTFUNC(new_variable, input_string) ||
                     INTFUNC(input_string) ||
                     BOOLFUNC(input_string);

    return haspassed;
}

bool LISTFUNC(lisp **new_variable, InputString *input_string)
{
    if (is_at_start(input_string->array2d[input_string->row], "CAR", input_string->col))
    {
        *new_variable = CAR(input_string);
        return true;
    }
    else if (is_at_start(input_string->array2d[input_string->row], "CDR", input_string->col))
    {
        *new_variable = CDR(input_string);
        return true;
    }
    else if (is_at_start(input_string->array2d[input_string->row], "CONS", input_string->col))
    {
        *new_variable = CONS(input_string);
        return true;
    }
    else
    {
        // No CAR,CDR or CONS found
        *new_variable = NULL;
        return false;
    }
}
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

int INTFUNC(InputString *input_string)
{
    if (is_at_start(input_string->array2d[input_string->row], "PLUS", input_string->col))
    {
        LIST(input_string); 
        LIST(input_string); 
    }
}

bool BOOLFUNC(InputString *input_string)
{
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
    char letter = VAR(input_string);
    lisp *list = LIST(input_string);
    add_variable(input_string, letter, list);
}

void add_variable(InputString *input_string, char letter, lisp *var)
{
    input_string->variables[input_string->variable_count].value = var;
    input_string->variables[input_string->variable_count].name = letter;
    input_string->variable_count++;
}

void PRINT(InputString *input_string)
{
    input_string->col += strlen("PRINT");
    move_next_char(input_string);
    char letter = VAR(input_string);
    lisp *value = find_variable(input_string, letter);
    if (value == NULL)
    {
        printf("variable %c is undefined", letter);
        exit(EXIT_FAILURE);
    }
    char print_string[MAXLINEWIDTH] = "";
    lisp_tostring(value, print_string);
    puts(print_string);
}

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

char VAR(InputString *input_string)
{
    if (current_position(input_string) >= 'A' &&
        current_position(input_string) <= 'Z')
    {
        char letter = current_position(input_string);
        move_next_char(input_string);
        return letter;
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
    if (!current_position(input_string) == '(')
    {
        printf("Expected a '(' after IF\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    // BoolFunc here
    if (!current_position(input_string) == ')')
    {
        printf("Expected a ')' after BOOLFUNC\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    if (!current_position(input_string) == '(')
    {
        printf("Expected a '(' after BOOLFUNC BEFORE INSTRCTS\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    INSTRCTS(input_string);
    if (!current_position(input_string) == '(')
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
    if (!current_position(input_string) == '(')
    {
        printf("Expected a '(' after WHILE\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    // BOOLFUNC
    if (!current_position(input_string) == ')')
    {
        printf("Expected a ')' after BOOLFUNC\n");
        exit(EXIT_FAILURE);
    }
    if (!current_position(input_string) == '(')
    {
        printf("Expected a '(' after BOOLFUNC and before INSTRCTS\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    INSTRCTS(input_string);
}

lisp *LIST(InputString *input_string)
{
    if (current_position(input_string) == '\'')
    {
        return LITERAL(input_string);
    }
    else if (is_at_start(input_string->array2d[input_string->row], "NIL", input_string->col))
    {
        input_string->col += strlen("NIL");
        return NULL;
    }
    else if (current_position(input_string) == OPEN_BRACKET)
    {
        lisp *list_value = NULL;
        move_next_char(input_string);

        if (!LISTFUNC(&list_value, input_string))
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
        return list_value;
    }
    else
    {
        printf("Expected a LITERAL, 'NIL' or '(' at row %i, col %i", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }
    return 0;
}

char *STRING(InputString *input_string)
{
}

lisp *LITERAL(InputString *input_string)
{
    if (current_position(input_string) != '\'')
    {
        printf("Expected a single quote at the start of Literal\n");
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    char literalstring[MAXLINEWIDTH] = "";
    int counter = 0;
    do
    {
        literalstring[counter++] = current_position(input_string);
        input_string->col++;
    } while (!is_quote(current_position(input_string)));
    literalstring[counter] = NUM;
    lisp *literal_value = lisp_fromstring(literalstring);

    /*if (!get_next_quote(input_string))
    {
        printf("Unmatched quote at row %i col %i", input_string->row, input_string->col);
        exit(EXIT_FAILURE);
    }*/
    move_next_char(input_string);

    return literal_value;
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
}
