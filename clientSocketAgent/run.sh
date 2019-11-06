#!/bin/bash
gcc socket_client.c -o output
echo "compiled"
./output
echo "executed"
rm output
echo "deleted the compiled file"
