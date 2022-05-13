/**
	Ylopoiiste ena progromma to opoio tha dimiourgei 3 nea threads. Ta dyo prwta
	tha ayksanoun mia metavliti count h opoia tha einai koini se ola. Otan i timi
	tou counter ftasei mia sygkekrimeni timi (12) tote to 3o thread tha prepei 		na enimerwnetai gia ayti tin allagi kai na diplasiazei tin metavliti count. 		To programma prepei na trexei mexris otou ta threads pou ayksanoun tin count 		kanoun 24 epanalipsis.
	Aparaitites synartiseis tis pthread.h vivliothikis:
	int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
		void *(*start_routine)(void*), void *arg);
	void pthread_exit(void *value_ptr);
	int pthread_join(pthread_t thread, void **value_ptr);
	int pthread_mutex_init(pthread_mutex_t *mutex, 
    	const pthread_mutexattr_t *attr);
	int pthread_mutex_destroy(pthread_mutex_t *mutex);
	int pthread_mutex_lock(pthread_mutex_t *mutex);
	int pthread_mutex_unlock(pthread_mutex_t *mutex);
	int pthread_cond_init(pthread_cond_t *cond,
    	const pthread_condattr_t *attr);
	int pthread_cond_destroy(pthread_cond_t *cond);
	int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
	int pthread_cond_signal(pthread_cond_t *cond);
	int pthread_cond_broadcast(pthread_cond_t *cond);
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS  3
#define TCOUNT 24
#define COUNT_LIMIT 12

int count = 0;

void *increaseCount(void *t) {
	int i;
	long *threadId = (long *)t;

	for (i=0; i < TCOUNT; i++) {
		//TODO Ti symbainei otan ftasoume tin timi COUNT_LIMIT??
		count++;
	
		printf("increaseCount(): thread %ld, count = %d, unlocking mutex\n", 
	   		*threadId, count);
    	}
  	
	pthread_exit(NULL);
}

void *doubleCountVariable(void *t) {
	long *threadId = (long *) threadId;
	printf("Starting doubleCountVariable: thread %ld\n", *threadId);
	
	if (count < COUNT_LIMIT) {
		//TODO Kapws prepei na perimenei mexri na erthei i seira tou
		//gia ektelesi

    		count *= 2;
    	}
  	
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int i, rc; 
	int t1 = 1, t2 = 2, t3 = 3;
	pthread_t threads[3];

  	pthread_create(&threads[0], NULL, doubleCountVariable, &t1);
	sleep(1);
  	pthread_create(&threads[1], NULL, increaseCount, &t2);
  	pthread_create(&threads[2], NULL, increaseCount, &t3);

	/* Wait for all threads to complete */
  	for (i = 0; i < NUM_THREADS; i++) {
    	pthread_join(threads[i], NULL);
  	}
  	
	printf ("Main(): Waited on %d threads. Final value of count = %d. Done.\n", 
          NUM_THREADS, count);

  	/* Clean up and exit */
  	return 1;
}
