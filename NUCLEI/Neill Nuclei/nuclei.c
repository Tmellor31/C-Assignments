#include "nuclei.h"
/*Put current position everywhere relevant, file reading, #define CAR CDR ALL KEYWORDS*/

void Prog(InputString *input_string);
void INSTRCTS(InputString *input_string);
void INSTRCT(InputString *input_string);
bool LISTFUNC(InputString *input_string);
bool IOFUNC(InputString *input_string);
void LIST(InputString *input_string);
void VAR(InputString *input_string);
void LITERAL(InputString *input_string);
int currentx_position(InputString *input_string);
bool is_at_start(char *inputstring, char *keyword, int inputposition);
bool end_of_file_reached(InputString *input_string);
bool is_char(char target);
bool get_next_char(InputString *input_string);
void move_next_char(InputString *input_string);
bool find_next_target(InputString *input_string, bool (*char_matches)(char target));

void test(void);

int main(void)
{
    test();
    InputString *main_input_string = ncalloc(1, sizeof(InputString));
    strcpy(main_input_string->array2d[0], "(");
    strcpy(main_input_string->array2d[1], " ( CAR '2' ) ");
    strcpy(main_input_string->array2d[2], " (SET B '(1 2 3)' )");
    strcpy(main_input_string->array2d[3], "");
    strcpy(main_input_string->array2d[4], ")");
    main_input_string->x_position = 0;
    main_input_string->y_position = 0;
    main_input_string->row_count = 5;
    Prog(main_input_string);
    free(main_input_string);
}

char current_position(InputString *input_string)
{
    return input_string->array2d[input_string->y_position][input_string->x_position];
}

void Prog(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] != '(')
    {
        printf("Was expecting a '('\n");
        exit(EXIT_FAILURE);
    }
    INSTRCTS(input_string);
}

void INSTRCTS(InputString *input_string)
{
    move_next_char(input_string);
    if (input_string->array2d[input_string->y_position][input_string->x_position] == ')')
    {
        return;
    }
    INSTRCT(input_string);
    INSTRCTS(input_string);
}

void INSTRCT(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] != '(')
    {
        printf("Expected an '(' at row %i col %i \n", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }
    move_next_char(input_string);
    bool haspassed = false;
    haspassed = LISTFUNC(input_string) || IOFUNC(input_string);

    if (!haspassed)
    {
        printf("Expected CAR,CDR,CONS,PRINT, or SET?\n");
        exit(EXIT_FAILURE);
    }

    get_next_char(input_string);
    if (input_string->array2d[input_string->y_position][input_string->x_position] != ')')
    {
        printf("Expected an ')' at row %i col %i \n", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }
}

bool LISTFUNC(InputString *input_string)
{
    if (is_at_start(input_string->array2d[input_string->y_position], "CAR", input_string->x_position))
    {
        input_string->x_position += strlen("CAR");
        get_next_char(input_string);
        LIST(input_string);
        return true;
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "CDR", input_string->x_position))
    {
        input_string->x_position += strlen("CDR");
        get_next_char(input_string);
        LIST(input_string);
        return true;
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "CONS", input_string->x_position))
    {
        input_string->x_position += strlen("CONS");
        get_next_char(input_string);
        LIST(input_string);
        input_string->x_position += strlen("CONS");
        get_next_char(input_string);
        LIST(input_string);
        return true;
    }
    else
    {
        // No CAR,CDR or CONS found
        return false;
    }
}

bool IOFUNC(InputString *input_string)
{
    printf("HEY\n");
    if (is_at_start(input_string->array2d[input_string->y_position], "SET", input_string->x_position))
    {
        input_string->x_position += strlen("SET");
        printf("row %i col %i\n", input_string->y_position, input_string->x_position);
        move_next_char(input_string);
        VAR(input_string);
        LIST(input_string);
        return true;
    }

    else if (is_at_start(input_string->array2d[input_string->y_position], "PRINT", input_string->x_position))
    {
        move_next_char(input_string);
        VAR(input_string);
        return true;
    }

    else
    {
        // NO PRINT OR SET found
        return false;
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
    int row_length = strlen(input_string->array2d[input_string->y_position]);

    if (input_string->y_position == input_string->row_count - 1 &&
        input_string->x_position == row_length - 1)
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
        printf("Next character not found after row %i col %i\n", input_string->x_position, input_string->y_position);
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
    int col = input_string->x_position + 1;
    for (int row = input_string->y_position; row < input_string->row_count; row++)
    {
        int row_length = strlen(input_string->array2d[row]);
        for (; col < row_length; col++)
        {

            if (char_matches(input_string->array2d[row][col]))
            {
                // Successfully moved to next char
                input_string->y_position = row;
                input_string->x_position = col;
                return true;
            }
        }
        col = 0;
    }
    return false;
}

void LIST(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] == '\'')
    {
        LITERAL(input_string);
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "NIL", input_string->x_position))
    {
        input_string->x_position += strlen("NIL");
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "(", input_string->x_position))
    {
        if (LISTFUNC(input_string))
        {
            move_next_char(input_string);
        }
        else
        {
            printf("Expected a CAR, CONS or CDR? at row %i col %i\n", input_string->y_position, input_string->x_position);
            exit(EXIT_FAILURE);
        }

        if (input_string->x_position != ')')
        {
            printf("Expected a ')' at row %i col %i \n", input_string->y_position, input_string->x_position);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Expected a LITERAL, 'NIL' or '(' at row %i, col %i", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }
}

void VAR(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] >= 'A' &&
        input_string->array2d[input_string->y_position][input_string->x_position] <= 'Z')
    {
        move_next_char(input_string);
    }
    else
    {
        printf("Expected a character from A-Z");
        exit(EXIT_FAILURE);
    }
}

void LITERAL(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] != '\'')

    {
        printf("Expected a single quote at the start of Literal\n");
        exit(EXIT_FAILURE);
    }
    if (!get_next_quote(input_string))
    {
        printf("Unmatched quote at row %i col %i", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }
    printf("literalrow %i literalcol %i \n ", input_string->y_position, input_string->x_position);
}

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
    strcpy(test_input_string->array2d[2], "C++  ");
    strcpy(test_input_string->array2d[3], "Coffee");
    strcpy(test_input_string->array2d[4], "Silver");

    test_input_string->row_count = 5;
    assert(end_of_file_reached(test_input_string) == false);
    test_input_string->y_position = 4;
    test_input_string->x_position = strlen(test_input_string->array2d[4]) - 1;
    assert(end_of_file_reached(test_input_string) == true);
    test_input_string->y_position = 2;
    test_input_string->x_position = 2;

    assert(test_input_string->array2d[test_input_string->y_position][test_input_string->x_position] == '+');

    assert(get_next_char(test_input_string));
    assert(test_input_string->y_position == 3);
    assert(test_input_string->x_position == 0);
    (move_next_char(test_input_string));
}
