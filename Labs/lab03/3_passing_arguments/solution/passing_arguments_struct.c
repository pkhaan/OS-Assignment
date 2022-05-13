/**
	Dimiourgeiste ena programma to opoio tha ypologizei to ginomeno dyo tyxaiwn
	arithmwn (apo 1 ews 20) me xrisi enos thread. 
	Skopos tis ergasias einai na deite pws mporeite na perasete perissotera 	orismata se ena thread. 
	Aparaitites synartiseis tis pthread.h vivliothikis:
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void *(*start_routine)(void*), void *arg);
	void pthread_exit(void *value_ptr);
	int pthread_join(pthread_t thread, void **value_ptr);
	Alles aparaitites synartiseis:
	void srand(unsigned int seed);
    	int rand(void);
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct product_arguments {
	int firstArgument;
	int secondArgument;
} PRODUCT_ARGUMENTS;

/**
 * H synartisi pou tha kaleite apo ta threads gia na ypologizei to ginomeno.
*/
void *multiplyNumbers(void *args) {

	PRODUCT_ARGUMENTS *productArgs;
	productArgs = (PRODUCT_ARGUMENTS *) args;

	//ypologismos tou ginomenou.
	int product = productArgs->firstArgument * productArgs->secondArgument;
	printf("Thread: The product of %d and %d is %d.\n", productArgs->firstArgument, productArgs->secondArgument, product);
	pthread_exit(NULL);
}

/**
 * H main synartisi tha einai ypeuthini gia tin paragwgi twn dyo tyxaiwn arithmwn 
 * kai ti dimiourgia tou thread pou tha kanei ton ypologismo.
*/
int main(int argc, char *argv[]) {
	PRODUCT_ARGUMENTS productArgs;
	pthread_t threadId;
	int rc;
	
	/*dimiourgia tyxaiwn arithmwn*/
	srand(time(NULL));
    	productArgs.firstArgument = rand() % 20 + 1;
	productArgs.secondArgument = rand() % 20 + 1;

	printf("Thread: Random numbers to multiply %d, %d.\n", 	productArgs.firstArgument, productArgs.secondArgument);

	rc = pthread_create(&threadId, NULL, multiplyNumbers, &productArgs);
	
	/*elegxos oti to thread dimiourgithike swsta.*/
    	if (rc != 0) {
		printf("ERROR: return code from pthread_create() is %d\n", rc);
       		exit(-1);
    	}
	
	rc = pthread_join(threadId, NULL);
	
	if (rc != 0) {
		printf("ERROR: return code from pthread_join() is %d\n", rc);
		exit(-1);		
	}

	printf("Main: Thread finished.\n");
	return 1;
}
