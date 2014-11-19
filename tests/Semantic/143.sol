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
                         ro float
                         fi float

            +----Variables----+
                x int
                y int
                z struct $#unname#0

   +----Variables----+
       a struct $point

      {
         &a
         &a.x
         &a.y
         &a.z
         &a.z.ro
         &a.z.fi
      }

   }
