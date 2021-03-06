#!/bin/bash
cd parserTypesCheck;
all=0;
ok=0;
for file in $(ls | grep ".in$" | cut -d '.' -f 1); do
	all=$[$all+1]
	/Users/user/Desktop/University/c-compiler/Build/Products/Debug/c-compiler /p ${file}.in ok;
	if diff ${file}.out ${file}.sol >/dev/null ; then
		ok=$[$ok+1]
  		echo ${file} OK > /dev/null;
	else
  		echo ${file} BAD
	fi
done
cd ..
echo '========== Parse Types tests completed' $ok 'of' $all '==========';
