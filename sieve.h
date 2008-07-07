#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

typedef unsigned char* sieve_t;

sieve_t
sieve(size_t max){
		/*	
		 *	Sieve uses sentinel value of 0 for nonprimes
		 *	Starting at 0 and ending at max+1(necessitating the '+1's) was done
		 *	to simplify other calculations (it lets the index correspond to the
		 *	number represented)
		 *	
		 *	Note that this function returns a structure allocated on the heap,
		 *	make sure to free it appropriately.
		 *
		 *	BIT REPRESENTATION VERSION: As the structure is changed to represent
		 *	numbers internally as bits rather than bytes, it is intended that
		 *	the library remain unchanged from a user/interface perspective; as
		 *	such, "sieve_test.c" should remain unchanged between the vanilla and
		 *	bitwise versions.
		 */
		const unsigned char PRIME    = 1;
		const unsigned char NOTPRIME = 0;

		sieve_t numbers = malloc((max+1) * sizeof(*numbers));
		if(NULL == numbers){
				perror("Couldn't malloc enough for sieve");
				exit(EXIT_FAILURE);
		}

		//assume all numbers are prime before sieving
		memset(numbers, PRIME, max+1);

		numbers[0] = NOTPRIME;//remove 0, as it isn't prime
		numbers[1] = NOTPRIME;//ditto  1

		//sieve away
		for(size_t i = 0; i < (size_t)ceil(sqrt((double)max)); ++i){
				if (numbers[i] != NOTPRIME){
						for(size_t j = i+i; j<max+1; j += i){
								numbers[j] = NOTPRIME;
						}
				}
		}

		return numbers;
}
