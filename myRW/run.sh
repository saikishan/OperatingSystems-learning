#!/bin/bash
gcc p-readers-writers.c -pthread -o output
echo "compiled"
./output
echo "executed"
rm output
echo "deleted the compiled file"
