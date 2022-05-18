//
// Created by peter on 5/5/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "main.h"
/*
 * External Variables that have access to each and every thread
*/
int *id;
unsigned int randSeed;
int Ncust;
//Varables of Usage from the system

void errorHandler(int answer);

//Mutex Initialization
/*
 * Telephones
 * Seats
 * Account Balance
 *Console Output->Lock(Screen)
 * Services
 */
pthread_mutex_t mutex_no_available_telephones = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cashiers = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seats_available = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_screen = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t account_balance = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_on_hold = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t total_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t availableMachinesMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t terminalMutex = PTHREAD_MUTEX_INITIALIZER;\
pthread_mutex_t mutex_zoneA= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_zoneB = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t availableCondTelephones = PTHREAD_COND_INITIALIZER;
pthread_cond_t availableCondCashiers = PTHREAD_COND_INITIALIZER;

int available_tel = TEL_N;
int cashiers_available = N_CASH;
int deposit;
double avgWaitTime = 0; //Averace Client Waiting Time
double avgServeTime = 0; //Averace Client Servicing Time
int totalClientWating;
int availableSeats;
int exchangeCnt; //transaction ID in increasing order


int reserveSeats(int client, int no_seats);

void* run (void* threadID);{

		    int id = *(int *)threadId; // int *id = (int *)threadId;

			int rc;
			int done = done_complete;

			struct timespec threadStart;//for client
			struct timespec service_time;
			struct timespec cashier_start;
			struct timespec telThreadStart;
			struct timespec telThreadEnd;


			struct timespec threadEnd;//for client

		 clock_gettime(CLOCK_REALTIME, &threadStart);
		
		answer = pthread_mutex_lock(&mutex_no_available_telephones);
		errorHandler(int answer);
		printf("The client's id is: %d \n",id);


//In here we will store the client that is immediately connected to the call
	while(available_tel <= 0)
		{
		pthread_cond_wait(& availableCondTelephones ,&mutex_no_available_telephones);
		}
	available_tel--;
	clock_gettime(CLOCK_REALTIME, &service_time); 


		 //Randomize the booking procedure and initiate the phone call
	int randomSeats= rand_r(&seed) % NO_SEAT_HIGH + NO_SEAT_LOW;
	int randomTime = rand_r(&seed) % TIME_SEAT_HIGH + TIME_SEAT_LOW;
	sleep(randomTime);


	answer = pthread_mutex_unlock(&mutex_no_available_telephones);
	errorHandler(answer);
	






}












int main(int argc, char *argv[]){
		

}

}




void errorHandler(int answer)
{
	if (answer != 0) 
    {	
		printf("error in handling the mutex with referred error code %d\n", answer);
		pthread_exit(&answer);
	}
}

