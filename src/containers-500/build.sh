#!/bin/bash
gcc -o contained contained.o -lseccomp -lcap
gcc -c -Wall -Werror contained.c
