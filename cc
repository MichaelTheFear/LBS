#!/bin/bash

SCRIPT_DIR=`pwd`
EXISTS=`ls $SCRIPT_DIR/$1`

if [ $EXISTS ]  
then
    gcc -o binaryC $SCRIPT_DIR/$1.c -lm
    valgrind -q --undef-value-errors=no --leak-check=full binaryC 
    rm -f binaryC
else
    echo "404 - File not found"
fi