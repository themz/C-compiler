#!/bin/bash
for i in *.c ; do
    mv -v $i ${i%.c}.in
done