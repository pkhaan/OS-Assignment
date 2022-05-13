/**
	Skopos tis askisis einai na anixneseute ta lathi pou yparxoun sto programma 		kai na ta diorthwsete. To programma sti swsti tou morfi tha prepei na exei 		ta idia apotelesmata me tis askisis 1.
	Aparaitites synartiseis tis pthread.h vivliothikis:
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void *(*start_routine)(void*), void *arg);
	void pthread_exit(void *value_ptr);
	int pthread_join(pthread_t thread, void **value_ptr);
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * H synartisi pou tha kaleite apo ta threads gia na typwnei to mynima 
 * Thread: Hello World from thread threadId
*/
void *PrintHello(void *threadId) {
	int *tid;
	tid = (int *)threadId;
	printf("Thread: Hello World from thread %d!\n", *tid);
	sleep(3);
	return NULL;
}

/**
 * H main synartisi tha pairnei san orisma to plithos twn threads pou prepei na 
 * dimiourgithoun. Sti synexeia tha ta dimiourgei kai tha kalei tin pthread_exit(NULL)
 * prokeimenou ola ta threads na exoun ektypwsei to katallilo mhnyma protou o pateras
 * termatisei ti leitourgia tou.
*/
int main(int argc, char *argv[]) {
	
	if (argc != 2) {
		printf("ERROR: the program should take one argument, the number of threads to create!\n");
		exit(-1);
	}

	int maxNumberOfThreads = atoi(argv[1]);

	/*elegxoume oti to plithos twn threads pou edwse o xristis einai thetiko
	arithmos.*/
	if (maxNumberOfThreads < 0) {
		printf("ERROR: the number of threads to run should be a positive number. Current number given %d.\n", maxNumberOfThreads);
		exit(-1);
	}
	
	printf("We will create %d for threads that will print hello world.\n", maxNumberOfThreads);

	pthread_t *threads;

	threads = malloc(maxNumberOfThreads * sizeof(pthread_t));
	if (threads == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}

	int rc;
   	int i, threadCount;
   	for(i = 0; i < maxNumberOfThreads; i++) {
		threadCount = i + 1;
    		printf("Main: creating thread %d\n", threadCount);
		
		/*dimiourgia tou thread*/
    		rc = pthread_create(&threads[i], NULL, PrintHello, &threadCount);

		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR; return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
    	}

	return 1;
}
