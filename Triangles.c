#include <stdio.h> 
#include <assert.h> 

void test(void);
enum TriangleType {Equilateral, Scalene, Isosceles, NonPositive, Invalid};
typedef enum TriangleType TriangleType;
TriangleType typeoftriangle (int a, int b, int c);


int main(void)
{  
     test(); 

     int x = 0, y = 0, z = 0; 

     printf("Input the length of the three triangle sides");
     scanf("%i %i %i", &x, &y, &z);  
     int m = typeoftriangle(x,y,z); 
     printf("%i\n", m);
     return 0;
}


TriangleType typeoftriangle (int a, int b, int c) 
{ 
    if((a <= 0 ) || (b <= 0) || (c <= 0))
    {
      return NonPositive; 
    }
    else if ((c > a+b) || (b > a+b) || (a > c+b))
    { 
      return Invalid;
    }

    else if(a == b && b == c) //Check if equilateral
    { 
      return Equilateral;
    } 

    else if(a != b && b != c) //Check if scalene
    {
      return Scalene;
    }

    else //Must be isoceles
    {
     return Isosceles; 
    }
}

    

void test(void)
{
  assert(typeoftriangle(3,3,3) == Equilateral);  
  assert(typeoftriangle(6,6,6) == Equilateral); 
  assert(typeoftriangle(5,3,6) == Scalene); 
  assert(typeoftriangle(3,5,7) == Scalene);  
  assert(typeoftriangle(5,4,8) == Scalene); 
  assert(typeoftriangle(8,5,4) == Scalene); 
  assert(typeoftriangle(8,8,4) == Isosceles); 
  assert(typeoftriangle(4,4,8) == Isosceles);
  assert(typeoftriangle(-6,-6,-6) == NonPositive); 
  assert(typeoftriangle(2,4,500) == Invalid); 
  printf("Asserts found no errors \n"); 
}
  

 
