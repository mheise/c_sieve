#!/bin/sh
# A shell script to compile and run the build unit test(s).

cc -std=c99 -lm sieve_unit_test.c
./a.out
