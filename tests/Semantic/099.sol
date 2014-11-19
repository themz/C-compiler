+----Types----+
   int
   char
   float
   void
+----Functions----+
   int main(
   )
   {
   +----Types--+
      struct $point :
            +----Types--+
               struct $#unname#0 :
                     +----Variables----+
                         ro int
                         fi int

            +----Variables----+
                x int
                y int
                z struct $#unname#0

   +----Variables----+
       a struct $point

      {
         a.z
      }

   }
