#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "sieve.h"

int main(int argc, char **argv){
		assert(2 == argc);

		size_t max = strtoul(argv[1], NULL, 10);
		sieve_t my_sieve = sieve(max);
		
		for(size_t i=0; i<max+1; ++i){
				if(my_sieve[i])
						printf("%u\n", i);
		}

		free(my_sieve);

		return 0;
}

