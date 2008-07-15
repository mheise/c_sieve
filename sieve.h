#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

typedef unsigned char sievemember_t;
typedef sievemember_t*  sieve_t;

sievemember_t getbit(sieve_t s, size_t n);
void          setbit(sieve_t s, size_t n, sievemember_t c);

const sievemember_t PRIME      = 1;
const sievemember_t NOTPRIME   = 0;
const sievemember_t BYTE_PRIME =  UCHAR_MAX;

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
		 *	BIT REPRESENTATION VERSION: As the structure was changed to represent
		 *	numbers internally as bits rather than bytes, it was intended that
		 *	the library would remain unchanged from a user/interface
		 *	perspective, and that "sieve_test.c" would remain unchanged between
		 *	the vanilla and bitwise versions.  However, due to the impossibility
		 *	of overloading operator[] in straight C, changes had to be made;
		 *	unfortunately, access must now be through getbit()/setbit().
		 */
		size_t smax = (size_t)ceil(sqrt((double)max));
		size_t amax = (max/8) + 1;//8 bits to a byte

		sieve_t numbers = malloc((amax) * sizeof(sievemember_t));
		if(NULL == numbers){
				perror("Couldn't malloc enough for sieve");
				exit(EXIT_FAILURE);
		}

		//assume all numbers are prime before sieving
		memset(numbers, BYTE_PRIME, amax);
		setbit(numbers, 0, NOTPRIME);//remove 0, as it isn't prime
		setbit(numbers, 1, NOTPRIME);//ditto  1

		//sieve away
		for(size_t i = 0; i < smax; ++i){
				if (NOTPRIME != getbit(numbers, i)){
						for(size_t j = i+i; j<max+1; j += i){
								setbit(numbers, j, NOTPRIME);
						}
				}
		}

		return numbers;
}

sievemember_t
getbit(sieve_t s, size_t n){
		/*	Returns the n'th bit of sieve s	*/
		return (s[n/8]) & (1 << (n%8));
}

void
setbit(sieve_t s, size_t n, sievemember_t c){
		/*	Sets the n'th bit of sieve s to value c	*/
		if (!c)
				s[n/8] &= ~(1 << (n%8));
		else
				s[n/8] |= (1 << (n%8));
}
