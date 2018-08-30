#!/bin/bash
set -ex
gcc -c -Wall -Werror contained.c
gcc -o contained contained.o -lseccomp -lcap
rm contained.o
