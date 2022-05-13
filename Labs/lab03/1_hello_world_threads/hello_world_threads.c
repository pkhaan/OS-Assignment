/**
	Skopos tis askisis einai i ylopoiisi enos programmatos to opoio tha 
	dimiourgei pollapla
	threads ta opoia tha ektypwnoun sto termatiko ena mynima tis morfis Hello 		World from thread threadId,
	opou threadId tha einai i seira dimiourgias tou thread. Gia paradeigma to 		prwto thread tha prepei na ektypwsei to mhynyma: 
	Thread 1: Hello World from thread 1.
    	To plithos twn threads tha to diavazete
    	apo ti grammi entolwn. 
	Aparaitites synartiseis tis pthread.h vivliothikis:
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void *(*start_routine)(void*), void *arg);
	void pthread_exit(void *value_ptr);
	int pthread_join(pthread_t thread, void **value_ptr);
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * H synartisi pou tha kaleite apo ta threads gia na typwnei to mynima 
 * Thread: Hello World from thread threadId
*/
void *PrintHello(void *threadId) {
	//ylopoiisi tis ektypwsis pou theloume
}

/**
 * H main synartisi tha pairnei san orisma to plithos twn threads pou prepei na 
 * dimiourgithoun.
*/
int main(int argc, char *argv[]) {
	
	//TODO Elegxos oti o xristis edwse to katallilo plithos orismatwn.
	int maxNumberOfThreads;

	//TODO Anathesi tou plithous pou edwse o xristis sti metavliti
	//maxNumberOfThreads kai elegxos oti einai thetikos arithmos

	
	printf("Main: We will create %d for threads that will print hello world.\n", maxNumberOfThreads);
   	
	//TODO Dimiourgia twn threads

	return 1;
}
