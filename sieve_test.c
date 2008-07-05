#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "sieve.h"

void print_sieve(sieve_t s){
		for(size_t i=0; i<max+1; ++i){
				if(s[i])
						printf("%u\n", i);
		}
}

int main(int argc, char **argv){
		assert(2 == argc);

		size_t max = strtoul(argv[1], NULL, 10);
		sieve_t my_sieve = sieve(max);
		print_sieve(my_sieve);
		free(my_sieve);

		return 0;
}
