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
int Num;
//Varables of Usage from the system



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

void errorHandler(int answer);
int reserveSeats(int client_id, int sum_of_seats, int * zone, int cost, int client_seats);


//In this function we make the call between client and system 
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
		errorHandler(int answer);//function the checks locking status of mutex
		//printf("The client's id is: %d \n",id);


//In here we will store the client that is immediately connected to the call
	while(available_tel <= 0)
		{
		pthread_cond_wait(&availableCondTelephones ,&mutex_no_available_telephones);
		}
	available_tel--;
	clock_gettime(CLOCK_REALTIME, &service_time); 
//---------------------------------------------------------------------------------------

		 //Randomize the booking procedure and initiate the phone call
	int randomSeats = rand_r(&randSeed) % NO_SEAT_HIGH + NO_SEAT_LOW;
	sleep(rand_r(&randSeed) % TIME_SEAT_HIGH + TIME_SEAT_LOW);


	answer = pthread_mutex_unlock(&mutex_no_available_telephones);
	errorHandler(answer);
	clock_gettime(CLOCK_REALTIME, &waiting_time);


	double zoneSelect = (rand_r(&randSeed) % 100)/100.0f; //This is the input of the user in order to choose a random zone 
	double * zone;//preselected zone 
	double cost;
	int sum_of_seats;

	pthread_mutex_t mutex_zone;//init mutex 


	//In here we will have to check for the availability of the zones depending on the zones 
	//This part computes the probability of availability 
		

     if (zoneSelect <= P_ZONE_ALPHA){
		 zone = NO_ZONE_ALPHA;
		 mutex_zone = mutex_zoneA;
		 cost = COST_PER_SEAT_ZONE_A;
	 }
	else {
	     zone = NO_ZONE_BETA;
		 mutex_zone = mutex_zoneB;
		 cost = COST_PER_SEAT_ZONE_B;	
	}

	answer = pthread_mutex_lock(&mutex_zone);
	






}








int main(int argc, char *argv[]){
		

}



int reserveSeats(int client_id, int sum_of_seats, int * zone, int cost, int client_seats){
     
	int position;
	int sum_of_seats;
	bool done; //validity check of whether we have seats available
	rows = sum_of_seats / client_seats;

	for(int i = 0; i < rows; i++){
		temp_sum = 0;
		for(int position = 0; position < client_seats; position++){

		}
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

