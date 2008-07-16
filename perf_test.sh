#!/bin/bash

gcc -std=c99 -Wall -lm -O2 -fgnu89-inline -s -march=native sieve_test.c
printf '\nTen million:'
time ./a.out 10000000 > /dev/null
printf '\nFifty million:'
time ./a.out 50000000 > /dev/null
printf '\nHundred million:'
time ./a.out 100000000 > /dev/null
