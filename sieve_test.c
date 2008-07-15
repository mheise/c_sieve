#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "sieve.h"

void print_sieve(sieve_t s, uint64_t max){
		for(uint64_t i=0; i<max+1; ++i){
				if (getbit(s, i))
						printf("%llu\n", i);
		}
}

int main(int argc, char **argv){
		assert(2 == argc);

		uint64_t max = strtoull(argv[1], NULL, 0);
		sieve_t my_sieve = sieve(max);
		print_sieve(my_sieve, max);
		free(my_sieve);

		return 0;
}
