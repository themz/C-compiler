+----Types----+
   int
   char
   float
   void
   struct $strct :
         +----Variables----+
             i int
             appv array [100] of pointer to pointer to int

+----Variables----+
    i int
    f float
    p pointer to int
    a array [42] of int
    s struct $strct
+----Functions----+
   int main()
   {

      {
         1*2
         3.140000/2.720000
         1.800000/2
         3/0.500000
         i*=2
         f/=3.140000
         i/=2.720000
         f/=42
         1%3
         i%=333
         3+2
         3.140000+5
         1+p
         p+1
         1+a
         a+1
         i+=2
         f+=5
         p+=3
         2.300000-5.600000
         2-3.300000
         p-1
         a-2
         p-p
         a-a
         a-p
         i-=3.140000
         f-=3.140000
         p-=42
         2<<3
         2>>3
         2&3
         2|3
         2^3
         i<<=1
         i>>=2
         i&=3
         i|=4
         i^=5
         1==3
         3!=f
         p<=p
         a>=a
         p<a
         3>a
         p<=2
         1||2
         2.300000||6.660000
         3.200000&&55
         a||p
         p&&3.140000
         i=2
         f=3.140000
         i=3.140000
         f=2
         i=p
         i=a
         p=3
         p=p
         p=a
         s=s
      }

   }
