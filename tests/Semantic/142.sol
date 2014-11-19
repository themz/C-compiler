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
                         a int

            +----Variables----+
                x int
                y int
                z struct $#unname#0

   +----Variables----+
       a struct $point

      {
         a.x=3
         a.y=4
         a.z.a=3
      }

   }
