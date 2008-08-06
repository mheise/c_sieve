#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned char sievemember_t;
typedef sievemember_t*  sieve_t;

inline sievemember_t getbit(sieve_t s, uint64_t n);
inline void          setbit(sieve_t s, uint64_t n, sievemember_t c);

const sievemember_t PRIME      = 1;
const sievemember_t NOTPRIME   = 0;
const sievemember_t BYTE_PRIME =  UCHAR_MAX;

sieve_t
sieve(uint64_t max){
		/*	
		 *	Starting at 0 and ending at max+1(necessitating the '+1's) was done
		 *	to simplify other calculations (it lets the index correspond to the
		 *	number represented)
		 *	
		 *	Note that this function returns a structure allocated on the heap,
		 *	make sure to free it appropriately.
		 *
		 *	Note that access to this structure should be through
		 *	getbit()/setbit
		 */
		uint64_t smax = (uint64_t)ceil(sqrt((double)max));
		uint64_t amax = (max/8) + 1;//8 bits to a byte

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
		for(uint64_t i = 0; i < smax; ++i){
				if (NOTPRIME != getbit(numbers, i)){
						for(uint64_t j = i+i; j<max+1; j += i){
								setbit(numbers, j, NOTPRIME);
						}
				}
		}

		return numbers;
}

inline sievemember_t
getbit(sieve_t s, uint64_t n){
		/*	Returns the n'th bit of sieve s	*/
		return (s[n/8]) & (1 << (n%8));
}

inline void
setbit(sieve_t s, uint64_t n, sievemember_t c){
		/*	Sets the n'th bit of sieve s to value c	*/
		if (!c)
				s[n/8] &= ~(1 << (n%8));
		else
				s[n/8] |=  (1 << (n%8));
}
