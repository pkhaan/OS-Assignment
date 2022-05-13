/**
	Dimiourgeiste ena programma to opoio tha ypologizei to ginomeno dyo tyxaiwn 		arithmwn (apo 1 ews 20) me xrisi enos thread. 
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

int firstNumber;
int secondNumber;

/**
 * H synartisi pou tha kaleite apo ta threads gia na ypologizei to ginomeno.
*/
void *multiplyNumbers(void *args) {
	//ypologismos tou ginomenou.
	int product = firstNumber * secondNumber;
	printf("Thread: The product of %d and %d is %d.\n", firstNumber, secondNumber, product);
	pthread_exit(NULL);
}

/**
 * H main synartisi tha einai ypeuthini gia tin paragwgi twn dyo tyxaiwn arithmwn
 * kai ti dimiourgia tou thread pou tha kanei ton ypologismo.
*/
int main(int argc, char *argv[]) {
	pthread_t threadId;
	int rc;

	/*dimiourgia tyxaiwn arithmwn*/
	srand(time(NULL));
    	firstNumber = rand() % 20 + 1;
	secondNumber = rand() % 20 + 1;

	printf("Main: Random numbers to multiply %d, %d.\n", firstNumber, secondNumber);

	rc = pthread_create(&threadId, NULL, multiplyNumbers, NULL);
	
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
