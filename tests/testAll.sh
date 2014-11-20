#!/bin/bash
clear;
echo ''
bash scannerTest.sh
echo ''
#bash parserExpTest.sh
#echo ''
bash parserSymTest.sh
echo ''
bash parserTypesCheck.sh
echo ''
bash generator.sh
echo ''
