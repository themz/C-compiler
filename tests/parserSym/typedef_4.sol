+----Types----+
   int
   char
   float
   void
   r int
   qwe array [10] of array [20] of pointer to pointer to pointer to float
   struct $#unname#0 :
         +----Variables----+
             i int

   rr struct $#unname#0 :
      +----Variables----+
          i int

   struct $#unname#1 :
         +----Variables----+
             aa r int
             pp pointer to rr struct $#unname#0 :
      +----Variables----+
          i int


   tt struct $#unname#1 :
      +----Variables----+
          aa r int
          pp pointer to rr struct $#unname#0 :
      +----Variables----+
          i int


   tt2 array [10] of array [20] of pointer to struct $#unname#1
   struct $ts3 :
         +----Variables----+
             pts3 pointer to int
             b tt struct $#unname#1 :
      +----Variables----+
          aa r int
          pp pointer to rr struct $#unname#0 :
      +----Variables----+
          i int



   pts3 pointer to struct $ts3
   apts3 array [10] of pts3 pointer to struct $ts3
+----Variables----+
    a r int
    b qwe array [10] of array [20] of pointer to pointer to pointer to float
    c pts3 pointer to struct $ts3
