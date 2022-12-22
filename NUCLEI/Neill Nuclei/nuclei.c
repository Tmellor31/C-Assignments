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
        printf("Expected '(' at row %i col %i /n", input_string->y_position, input_string->x_position);
        exit(EXIT_FAILURE);
    }
}
