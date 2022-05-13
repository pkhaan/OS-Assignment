/**
	Dimiourgeiste ena programma to opoio tha ypologizei to ginomeno dyo tyxaiwn 		arithmwn (apo 1 ews 20) me xrisi enos thread. 
	Skopos tis ergasias einai na deite pws mporeite na perasete perissotera 	orismata se ena	thread. 
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

/**
 * H synartisi pou tha kaleite apo ta threads gia na ypologizei to ginomeno.
*/
void *multiplyNumbers(void *args) {
	//ylopoiisi tis ektypwsis pou theloume
}

/**
 * H main synartisi tha einai ypeuthini gia tin paragwgi twn dyo tyxaiwn arithmwn kai ti
 * dimiourgia tou thread pou tha kanei ton ypologismo.
*/
int main(int argc, char *argv[]) {
	
	//TODO Dimiourgia tyxaiwn arithmwn

	//TODO Ekkinisi tou thread

	//TODO Perimene mexri to thread na oloklirwsei tin ektelesi tou
	
	return 1;
}
