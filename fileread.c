#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int main(int argc, char *argv[])
{
    char string[200]; 
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
    
    fgets(string,200,fp);
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '\n')
        {
            putchar('N'); 
        }
        else 
        {
            putchar(string[i]); 
        }
    } 
    fclose(fp); 
}