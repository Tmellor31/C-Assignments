#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define target 2.71828182845904523536


'a' - 'z'  

int targetfinder(double x);
int fraction;

int main(void)
{
  int num = 87;
  fraction = targetfinder(num);
  printf("The best approximation of %lf is %lf", target, fraction);
}

int targetfinder(int num)
{
  int den;
  float mindiff = target - 0.25;
  float maxdiff = target + 0.25;
  float close;

  for (den = 1; den < 100000; den++)
  {
    fraction = num / den if (fraction - target > maxdiff)
    {
      close = close;
    }
    elif (fraction - targer < mindiff)
    {
      close = close;
    }
    else
    {
      close = fraction;
    }
    return fraction;
  }