#include <stdio.h> 
#include <assert.h> 

int percentfinder(int n); 

int main(void)
{
   printf("%c", 'a');
int n = 1000;  
int result = percentfinder(n);
printf("The percentage of primes ending in three between 1 and %i was %i\n", n, result); 
return 0; 
}

int percentfinder(int n){
    int i, num, count;
    int ldthree = 0;   
    int total = 0; 
    int percentthree = 0; 
     
      for(num = 1;num<=n;num++) 
   {
      count = 0;
      
      for(i=2;i<=num/2;i++) //Check if prime
      {
         if(num%i==0){
            count++;
            break;
           return 0; 
         }
      }
   if(count == 0 && num != 1) //print the prime
     { 
      //printf("%d ",num);
      total++; 
      if(num % 10 == 3) //keep track of how many end in 3
      {  
        ldthree++;
        //printf("%i\n", ldthree); 
      }
    }
   }
   return (ldthree * 100)/total; 
}



