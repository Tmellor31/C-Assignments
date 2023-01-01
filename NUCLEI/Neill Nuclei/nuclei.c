#include "nuclei.h"

void Prog(InputString *input_string);
void INSTRCTS(InputString *input_string);
void INSTRCT(InputString *input_string);
void LISTFUNC(InputString *input_string);
void IOFUNC(InputString *input_string);
void LIST(InputString *input_string);
void VAR(InputString *input_string);
void LITERAL(InputString *input_string);
int currentx_position(InputString *input_string);
bool is_at_start(char *inputstring, char *keyword, int inputposition);
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
        printf("Expected '(' at row %i col %i \n", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }
}

void LISTFUNC(InputString *input_string)
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
      printf("Expected a CAR,CDR or CONS?");
      exit(EXIT_FAILURE); 
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
}
