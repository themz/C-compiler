#!/bin/bash
cd parserSym;
for file in $(ls | grep ".in$" | cut -d '.' -f 1); do
	/Users/user/Desktop/University/c-compiler/Build/Products/Debug/c-compiler /p ${file}.in;
	if diff ${file}.out ${file}.sol >/dev/null ; then
  		echo ${file} OK
	else
  		echo ${file} BAD
	fi
done
cd ..
