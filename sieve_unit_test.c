#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "sieve.h"

int main(void){

		const size_t MAX_NUM = 2000000;
		const unsigned long long CORRECT_SUM = 142913828922LLU;

		long long unsigned int sum = 0;
		sieve_t my_sieve = NULL;

		//memory allocated correctly	
		my_sieve = sieve(MAX_NUM);
		assert(NULL != my_sieve);

		//sieve produces correct results
		for(int i=0; i<MAX_NUM; ++i){
				if(my_sieve[i])
						sum += i;
		}
		assert(CORRECT_SUM == sum);

		puts("All tests passed!\n");
		return EXIT_SUCCESS;
}

