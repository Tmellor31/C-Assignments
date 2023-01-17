#include "nuclei.h"
/*Finish main parsing functions
Add asserts for correct results - including where get position ++ change for get next char
which will eliminate spaces and new lines - \n and \r
#defines
then maybe another look at is at start.*/

// Need to check functions to see if I want to fail if the file ends during the function
// If reach endfile at any point apart from via Prog close bracket, exit the program
// For each line in the file going to read it into a string

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
void test(void);

int main(void)
{
    test();
}

int currentx_position(InputString *input_string)
{
    return input_string->x_position;
}

void Prog(InputString *input_string)
{
    if (input_string->array2d[0][0] != '(')
    {
        printf("Was expecting a '('\n");
        exit(EXIT_FAILURE);
    }
    input_string->x_position++;
    INSTRCTS(input_string);
}

void INSTRCTS(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] == ')')
    {
        input_string->x_position++;
    }
    else
    {
        INSTRCT(input_string);
        INSTRCTS(input_string);
    }
}

void INSTRCT(InputString *input_string)
{
    if (input_string->array2d[input_string->y_position][input_string->x_position] != '(')
    {
        printf("Expected an '(' at row %i col %i \n", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }

    bool haspassed = false;
    haspassed = LISTFUNC(input_string) || IOFUNC(input_string);

    if (!haspassed)
    {
        printf("Expected CAR,CDR,CONS,PRINT, or SET?\n");
        exit(EXIT_FAILURE);
    }
}

bool LISTFUNC(InputString *input_string)
{
    if (is_at_start(input_string->array2d[input_string->y_position], "CAR", input_string->x_position))
    {
        LIST(input_string);
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "CDR", input_string->x_position))
    {
        LIST(input_string);
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "CONS", input_string->x_position))
    {
        LIST(input_string);
        LIST(input_string);
    }
    else
    {
        // No CAR,CDR or CONS found
        return false;
    }
}

bool is_at_start(char *inputstring, char *keyword, int inputposition)
{
    for (int i = 0; i < strlen(keyword); i++)
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
    int y_length = strlen(input_string->array2d[input_string->y_position]);

    if (input_string->y_position == input_string->row_count-1 &&
        input_string->x_position == y_length-1)
    {
        // End of file reached
        return true;
    }

}

bool get_next_char(InputString *input_string)
{
    int y_length = strlen(input_string->array2d[input_string->y_position]);
    while (true)
    {
        if (!end_of_file_reached(input_string))
        {
            return false;
        }
        else if (input_string->array2d[input_string->y_position][input_string->x_position] == '\n')
        {
            input_string->y_position++;
        }
        else if (isspace(input_string->array2d[input_string->y_position][input_string->x_position]))
        {
            input_string->x_position++;
        }
        else
        {
            // Successfully moved to next char
            return true;
        }
    }
}

bool IOFUNC(InputString *input_string)
{
    if (is_at_start(input_string->array2d[input_string->y_position], "SET", input_string->x_position))
    {
        VAR(input_string);
        LIST(input_string);
    }

    else if (is_at_start(input_string->array2d[input_string->y_position], "PRINT", input_string->x_position))
    {
        VAR(input_string);
    }

    else
    {
        // NO PRINT OR SET found
        return false;
    }
}

void LIST(InputString *input_string)
{
    bool functionchecker = false;
    if (is_at_start(input_string->array2d[input_string->y_position], "'", input_string->x_position))
    {
        LITERAL(input_string);
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "NIL", input_string->x_position))
    {
        input_string->x_position += strlen("NIL");
    }
    else if (is_at_start(input_string->array2d[input_string->y_position], "(", input_string->x_position))
    {
        if (!LISTFUNC(input_string))
        {
            printf("Expected a CAR, CONS or CDR?\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            input_string->x_position++;
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
        input_string->x_position++;
    }
    else
    {
        printf("Expected a character from A-Z");
        exit(EXIT_FAILURE);
    }
}

void LITERAL(InputString *input_string)
{
    input_string->x_position++;
    if (input_string->array2d[input_string->y_position][input_string->x_position] == '\'' ||
        input_string->array2d[input_string->y_position][input_string->x_position] >= '0' &&
            input_string->array2d[input_string->y_position][input_string->x_position] <= '9')
    {
        input_string->x_position++;
    }

    else if (strlen(input_string->array2d[input_string->y_position][input_string->x_position]) == 0)
    {
        printf("Given string is empty?\n");
    }

    else
    {
        printf("Expected a number from 1-9 or a list of numbers :) \n");
        exit(EXIT_FAILURE);
    }
}

void test(void)
{
    char test_input_string[] = "Fred likes samsung tvs";
    char test_keyword[] = "Fred";
    char test_wrong_keyword[] = "John";
    char test_fail[] = "likes";
    int test_input_position = 0;

    assert(is_at_start(test_input_string, test_keyword, test_input_position) == true);
    assert(is_at_start(test_input_string, test_wrong_keyword, test_input_position) == false);
    assert(is_at_start(test_input_string, test_fail, test_input_position) == false);
    assert(is_at_start(test_input_string, test_fail, 5) == true);
    assert(is_at_start(test_input_string, "likes", 5) == true);

    /*TestInputString *test_input_struct; 
    test_input_struct->test_array2d= 
        {
            {'J', 'a', 'v', 'a', '\0'},
            {'P', 'y', 't', 'h', 'o', 'n', '\0'},
            {'C', '+', '+', '\0'},
            {'H', 'T', 'M', 'L', '\0'},
            {'S', 'Q', 'L', '\0'}};

    assert(end_of_file_reached(test_array2d) == false);*/
}
