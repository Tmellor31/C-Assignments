#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int randomm(int songlist[], int songmax);

int main(void)
{
  int songmax = 0;
  int songmix = 0;
  int songlist = 0;
  int count;
  scanf("%i", &songmax);
  int x = songmax;
  int choice = randomm;
  for (count = 0; count < songmax; count++)
  {
    printf("%i \n", randomm);
  }
  return 0;
}

// int randomlist(int songmax)
// {
//   int final = 0;
//   final = rand() % songmax;
//   printf("%i", final);
//   return final;
// }

int randomm(int songlist[], int songmax)
{ 
  int y = 0; 
  int result;
  y = rand() % songmax;
  result = songlist[y]; 
  return result; 
}
