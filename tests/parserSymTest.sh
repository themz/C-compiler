#!/bin/bash
cd parserSym;
all=0;
ok=0;
for file in $(ls | grep ".in$" | cut -d '.' -f 1); do
	all=$[$all+1]
	/Users/user/Desktop/University/c-compiler/Build/Products/Debug/c-compiler /p ${file}.in;
	if diff ${file}.out ${file}.sol >/dev/null ; then
		ok=$[$ok+1]
  		echo ${file} OK > /dev/null;
	else
  		echo ${file} BAD
	fi
done
cd ..
echo '========== Declaration and statement tests completed' $ok 'of' $all '==========';
