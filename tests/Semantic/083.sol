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
      struct $a :
            +----Variables----+
                a int

   +----Variables----+
       x pointer to array [15] of struct $a :
      +----Variables----+
          a int

       y pointer to function (
    #unname#0 struct $a
    #unname#1 struct $a
) returning struct $a :
      +----Variables----+
          a int

       z pointer to function (
    #unname#2 array [] of struct $a :
      +----Variables----+
          a int

    #unname#3 array [] of pointer to struct $a
) returning struct $a :
      +----Variables----+
          a int


   }
