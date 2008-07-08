#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>

typedef unsigned char sievemember_t;
typedef sievemember_t*  sieve_t;

const sievemember_t PRIME    = 1;
const sievemember_t NOTPRIME = 0;

struct arg_set{
		size_t min;
		size_t max;
		size_t sieved_num;
		sieve_t dataptr;
};

void *worker(void *args);
void divy_work(struct arg_set *args, size_t threadnum, size_t max);
size_t correct_min(size_t sieved_num, size_t min);

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
		 *	MULTITHREAD VERSION: As SMP capability is added, my intention is for
		 *	the vanilla and pthread versions of this library be identical from a
		 *	user/interface perspective; as such, "sieve_test.c" should be
		 *	identical in all branches.
		 */
		const unsigned char NUM_THREADS = 2;

		pthread_t threads[NUM_THREADS];
		struct arg_set args;
		size_t smax = (size_t)ceil(sqrt((double)max));

		sieve_t numbers = malloc((max+1) * sizeof(sievemember_t));
		if(NULL == numbers){
				perror("Couldn't malloc enough for sieve");
				exit(EXIT_FAILURE);
		}

		//assume all numbers are prime before sieving
		memset(numbers, PRIME, max+1);
		numbers[0] = NOTPRIME;//remove 0, as it isn't prime
		numbers[1] = NOTPRIME;//ditto  1
		args.dataptr = numbers;

		//sieve away - note inner loop parallelized
		for(size_t i = 0; i < smax; ++i){
				if (numbers[i] != NOTPRIME){
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
				numbers[j] = NOTPRIME;
		}

		pthread_exit(NULL);
}

void 
divy_work(struct arg_set *args, size_t threadnum, size_t max){
		if (0 == threadnum){
				args->min = 0;
				args->max = max/2+1;
		}else if (1 == threadnum){
				args->min = max/2;
				args->max = max+1;
		}
}

size_t
		correct_min(size_t sieved_num, size_t min){
				if ( (sieved_num + sieved_num) >= min)
						return sieved_num + sieved_num;
				else
						return (min/sieved_num) * sieved_num;
		}
