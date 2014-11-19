+----Types----+
   int
   char
   float
   void
   struct $point
   sp struct $point
   spp pointer to struct $point
+----Functions----+
   sp struct $point foo(
       #unname#0 sp struct $point
       #unname#1 sp struct $point
       #unname#2 spp pointer to struct $point
   )
   sp struct $point bar(
       #unname#3 struct $point
       #unname#4 pointer to struct $point
       #unname#5 sp struct $point
       #unname#6 spp pointer to struct $point
       a struct $point
       #unname#7 spp pointer to struct $point
       b pointer to struct $point
   )
