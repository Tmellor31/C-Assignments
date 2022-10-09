#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

void test(void);

bool isvowel(char s);
bool isnamevalid(char name[]);
int chartoint(char x);
bool isPrime(int num);

int main(void)
{
  test();
  char name[4] = {'a', 'a', 'a', '\0'};
  int order = 1;
  char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

  for (name[1] = letters[0]; name[1] <= 'z'; name[1]++) // Prints out all combinations of isnamevalid
  {
    if (isnamevalid(name))
    {
      printf("%i %s\n", order, name);
      order++;
    }
    for (name[0] = letters[0], name[2] = letters[0]; name[0] && name[2] <= 'z'; name[0]++ && name[2]++)
    {
      if (isnamevalid(name))
      {
         printf("%i %s\n", order, name);
         order++;
      }
    }
  }
}

bool isnamevalid(char name[]) //Runs name through many checks and returns either true or false
{
  if (strlen(name) != 3)
  {
    return false;
  }
  if (name[0] != name[2])
  {
    return false;
  }
  if (name[1] > 'z' || name[1] < 'a')
  {
    return false;
  }
  if (!((isvowel(name[0]) && isvowel(name[2]) && !isvowel(name[1])) || (!isvowel(name[0]) && !isvowel(name[2]) && isvowel(name[1]))))
  {
    // Checks if string is either c/v/c or v/c/v
    return false;
  }
  int position = 0;
  int sum = 0;

  for (position = 0; name[position] != '\0'; position++)
  {
    int several = chartoint(name[position]);
    sum += several;
  }
  if (!isPrime(sum))
  {
    return false;
  }
  return true;
}

bool isvowel(char s)
{
  char VOWELS[6] = {'a', 'e', 'i', 'o', 'u', '\0'};
  int position;
  for (position = 0; VOWELS[position] != '\0'; position++)
  {
    char vowel = VOWELS[position];
    if (s == vowel)
    {
      return true;
    }
  }
  return false;
}

int chartoint(char x)
{
  int y = x - 'a';
  y++;
  return y;
}

bool isPrime(int num)
{
  int count, x = 0;
  for (count = 1; count <= num; count++)
  {
    if (num % count == 0)
    {
      x++;
    }
  }
  if (x == 2)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void test(void)
{
  assert(isvowel('i') == true);
  assert(isvowel('x') == false);
  assert(isvowel('u') == true); 
  assert(isnamevalid("ixi") == false);
  assert(isnamevalid("xix") == false);
  assert(isnamevalid("iwi") == true);
  assert(isnamevalid("aba") == false);
  assert(isnamevalid("bob") == true);
  assert(isnamevalid("a{a") == false);
  assert(isPrime(17) == true); 
  assert(isPrime(18) == false); 
}
