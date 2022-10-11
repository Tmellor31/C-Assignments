#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 

double microwave(double ctime);

int main(void)
{
 //assert(microwave(60) == 6); 
 double result;
 double ctime; 
 scanf("%lf", &ctime); 
 result = microwave(ctime); 
 printf("%lf button press ", result); 
}

double microwave(double ctime)
{
 double buttons[3] = {0.10,1,10}; 
 int buttonpresses = 0;
 
for(;ctime > 0;)
 { 
 
   if(ctime >= 10) 
   {
     ctime -= buttons[2];
     buttonpresses++;
    //  printf("This is first button %i\n", buttonpresses);
    //  printf("%lf", ctime); 
   }
   if(ctime >= 1 && ctime < 10) 
   {
     ctime -= buttons[1];
     buttonpresses++;  
    //  printf("This is second button %i\n", buttonpresses);
    //  printf("%lf", ctime); 
   }
   if(ctime < 1 && ctime > 0) 
   {
     ctime -= buttons[0];
     buttonpresses++;  
    //  printf("This is third button %i\n", buttonpresses);
    //  printf("%lf", ctime); 
   }
 }
return buttonpresses; 
} 
