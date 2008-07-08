#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

typedef unsigned char sievemember_t;
typedef sievemember_t*  sieve_t;

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
		 *	numbers internally as bits rather than bytes, it was intended that
		 *	the library would remain unchanged from a user/interface
		 *	perspective, and that "sieve_test.c" would remain unchanged between
		 *	the vanilla and bitwise versions.  However, due to the impossibility
		 *	of overloading operator[] in straight C, changed had to be made;
		 *	macros were defined to alleviate this situation somewhat.
		 */
		const sievemember_t PRIME    = 1;
		const sievemember_t NOTPRIME = 0;

		sieve_t numbers = malloc((max+1) * sizeof(sievemember_t));
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
