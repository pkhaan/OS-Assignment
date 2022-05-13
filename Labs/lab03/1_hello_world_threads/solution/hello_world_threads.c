/**
	Skopos tis askisis einai i ylopoiisi enos programmatos to opoio tha 		dimiourgei pollapla threads ta opoia tha ektypwnoun sto termatiko ena mynima 		tis morfis Hello World from thread threadId, opou threadId tha einai i seira 		dimiourgias tou thread. Gia paradeigma to prwto thread tha prepei
	na ektypwsei to mhynyma: Thread 1: Hello World from thread 1.
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
	int *tid;
	tid = (int *)threadId;
	printf("Thread: Hello World from thread %d!\n", *tid);
	/*aparaitito gia na gnwrizei o pateras oti to thread termatise swsta,
	douleuei swsta kai an kanete return kapoia timi.*/
	pthread_exit(tid);
}

/**
 * H main synartisi tha pairnei san orisma to plithos twn threads pou prepei na 
 * dimiourgithoun. Sti synexeia tha ta dimiourgei kai tha kalei tin pthread_join(pthread_t thread, void **value_ptr)
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
	
	printf("Main: We will create %d for threads that will print hello world.\n", maxNumberOfThreads);

	pthread_t *threads;

	threads = malloc(maxNumberOfThreads * sizeof(pthread_t));
	if (threads == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}

	int rc;
   	int threadCount;
	int countArray[maxNumberOfThreads];
   	for(threadCount = 0; threadCount < maxNumberOfThreads; threadCount++) {
    		printf("Main: creating thread %d\n", threadCount);
		countArray[threadCount] = threadCount + 1;
		/*dimiourgia tou thread*/
    		rc = pthread_create(&threads[threadCount], NULL, PrintHello, &countArray[threadCount]);

		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
    	}

	void *status;
	/*Aparaitito gia na stamatisei to thread, an den to orisete yparxei
	periptwsi na teleiwsei o pateras prin ta threads kai ara na min exoume
	to epithymito apotelesma*/
	for (threadCount = 0; threadCount < maxNumberOfThreads; threadCount++) {
		rc = pthread_join(threads[threadCount], &status);
		
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}

		printf("Main: Thread %d returned %d as status code.\n", countArray[threadCount], (*(int *)status));
	}

	free(threads);
	return 1;
}
