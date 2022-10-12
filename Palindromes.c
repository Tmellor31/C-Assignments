#include <stdio.h> 
#include <ctype.h> 
#include <assert.h> 
#include <stdlib.h> 

#define MAXLENGTH 20

int lengthchecker(char phrase[]);

int main(void)
{
 assert(lengthchecker("Apple") == 5);
 assert(lengthchecker("Consequently") == 12);   
 assert(sanitiser("Gorilla") == "Gorilla"); 
}


//bool palincheck(char phrase[])
//{
 
//}

int lengthchecker(char phrase[])
{ 
 int x; 
 for(x = 0;x < MAXLENGTH; x++)
 { 
  if(phrase[x] == '\0')
  {
   return x; 
  } 
 }
return x;    
}

char sanitiser(char phrase[])
{
 int x; 
 char sanitisedphrase[20]; 
 for(x = 0;x < lengthchecker(phrase);)
 { 
  if(phrase[x] >= 'a' && phrase[x] <= 'z')
    {
     x++; 
    } 
  else 
    {
     sanitisedphrase[x] = phrase[x];
     x++;  
    } 
  }
return sanitisedphrase;  
}





