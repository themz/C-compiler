#!/bin/bash
cd scanner;
for file in $(ls | grep ".in$" | cut -d '.' -f 1); do
	/Users/user/Desktop/University/c-compiler/Build/Products/Debug/c-compiler /s ${file}.in;
	if diff ${file}.out ${file}.sol >/dev/null ; then
  		echo ${file}
	else
  		echo ${file} BAD
	fi
done
cd ..