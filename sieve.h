#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>
#include <limits.h>
#include <stdio.h>

typedef unsigned char sievemember_t;
typedef sievemember_t*  sieve_t;


struct arg_set{
		size_t min;
		size_t max;
		size_t sieved_num;
		sieve_t dataptr;
};

/*	multithreading funcs	*/
void *worker(void *args);
void divy_work(struct arg_set *args, size_t threadnum, size_t max);
size_t correct_min(size_t sieved_num, size_t min);

/*	bit operation funcs	*/
inline sievemember_t getbit(sieve_t s, size_t n);
inline void          setbit(sieve_t s, size_t n, sievemember_t c);

const sievemember_t PRIME       = 1;
const sievemember_t NOTPRIME    = 0;
const sievemember_t BYTE_PRIME  =  UCHAR_MAX;
const char          NUM_THREADS = 2;

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

		pthread_t threads[NUM_THREADS];
		struct arg_set args;
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
		args.dataptr = numbers;

		//sieve away - note inner loop parallelized
		for(size_t i = 0; i < smax; ++i){
				if (NOTPRIME != getbit(numbers, i)){
						//spawn our workers
						args.sieved_num = i;
						for(size_t i=0; i<NUM_THREADS; ++i){
								divy_work(&args, i, max);
								int ret = pthread_create(&threads[i], NULL,
												worker, (void*)&args);
								if(0 != ret){
										perror("Couldn't spawn thread!");
										exit(EXIT_FAILURE);
								}
						}
						//wait for our workers to finish
						for(size_t i=0; i<NUM_THREADS; ++i){
								pthread_join(threads[i], NULL);
						}
				}
		}

		return numbers;
}

void*
worker(void *args){
		/*	Note that access in all worker threads to the main numbers vector
		 *	is asynchronous/mutex-free by design	*/

		struct arg_set *myargs = (struct arg_set*)args;
		size_t min  = myargs->min;
		size_t max  = myargs->max;
		size_t sieved_num = myargs->sieved_num;
		sieve_t numbers = myargs->dataptr;

		for(size_t j = correct_min(sieved_num, min); j<max; j += sieved_num){
				setbit(numbers, j, NOTPRIME);
		}

		pthread_exit(NULL);
}

void 
divy_work(struct arg_set *args, size_t threadnum, size_t max){
		args->min = (max/NUM_THREADS) * threadnum;
		args->max = (max/NUM_THREADS) * (threadnum + 1);

		if (NUM_THREADS == (threadnum + 1))
				++(args->max);
}

size_t
correct_min(size_t sieved_num, size_t min){
		if ( (sieved_num + sieved_num) >= min){
				return sieved_num + sieved_num;
		}else{
				return (min/sieved_num) * sieved_num;
		}
}

inline void
setbit(sieve_t s, size_t n, sievemember_t c){
		/*	Sets the n'th bit of sieve s to value c	*/
		if (!c)
				s[n/8] &= ~(1 << (n%8));
		else
				s[n/8] |= (1 << (n%8));
}

inline sievemember_t
getbit(sieve_t s, size_t n){
		/*	Returns the n'th bit of sieve s	*/
		return (s[n/8]) & (1 << (n%8));
}
