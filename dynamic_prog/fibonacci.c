#include <stdio.h>


//using dynamic programming
void fibo(int n)
{
   int f1,f2,f3;
   printf("0, 1");
   
   f1 = 0;
   f2 = 1;

   n -= 2;
   while (n != 0) {
      f3 = f1 + f2;
      printf(", %d", f3);
      f1 = f2;
      f2 = f3;
      
      n--;
   }
   printf("\n");
}

//recursive soln using dynamic programming
void fibo_rec(int n, int f1, int f2)
{
   int f3;
   
   if (n == 0)
      return;   

      f3 = f1 + f2;
      printf(", %d", f3);
      fibo_rec(n-1, f2, f3); 
}


// time complexity : exponential
int fibo_expo(int n)
{
   if (n == 0)
      return 0;
   
   else if (n == 1)
      return 1;
   
   else {
      return fibo_expo(n-1) + fibo_expo(n-2);
   }
}


int main()
{
   int n = 5;
   int i;

//method 1
   fibo(n);

//method 2
   printf("0, 1");
   fibo_rec(n-2, 0, 1); 
   printf("\n");

//method 3
   printf("0");
   n--;
   for(i = 1; i <= n; i++)
      printf(", %d", fibo_expo(i));

   printf("\n");

   return 0;
}
