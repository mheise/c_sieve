#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>

const unsigned char PRIME    = 1;
const unsigned char NOTPRIME = 0;

typedef unsigned char* sieve_t;

struct arg_set{
		size_t min;
		size_t max;
		size_t smax;
		sieve_t dataptr;
};

void *worker(void *args);
void divy_work(struct arg_set *args, size_t threadnum, size_t max);

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

		sieve_t numbers = malloc((max+1) * sizeof(unsigned char));
		if(NULL == numbers){
				perror("Couldn't malloc enough for sieve");
				exit(EXIT_FAILURE);
		}

		//assume all numbers are prime before sieving
		memset(numbers, PRIME, max+1);
		numbers[0] = NOTPRIME;//remove 0, as it isn't prime
		numbers[1] = NOTPRIME;//ditto  1

		//spawn our workers
		for(size_t i=0; i<NUM_THREADS; ++i){
				args.smax = (size_t)ceil(sqrt((double)max));
				args.dataptr = numbers;
				divy_work(&args, i, max);

				int ret = pthread_create(&threads[i], NULL, worker, (void*)&args);
				if(0 != ret){
						perror("Couldn't spawn thread!");
						exit(EXIT_FAILURE);
				}
		}

		//wait for our workers to finish
		for(size_t i=0; i<NUM_THREADS; ++i){
				pthread_join(threads[i], NULL);
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
		size_t smax = myargs->smax;
		sieve_t numbers = myargs->dataptr;

		for(size_t i = 0; i < smax; ++i){
				if (numbers[i] != NOTPRIME){
						for(size_t j = i+i; j<max; j += i){
								while (j<min) j += i; 
								numbers[j] = NOTPRIME;
						}
				}
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
		}else{
				fprintf(stderr, "Wrong threadnum passed somehow!\n");
				exit(EXIT_FAILURE);
		}
}
