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
//seats per every zone
int * seatsA;
int * seatsB;
//Varables of Usage from the system



//Mutex Initialization
/*
 * Telephones
 * Seats
 * Account Balance
 *Console Output->Lock(Screen)
 * Services
 */


#define seats_completed 300; //const int seats_found_success = 200;
#define done -1; //const int flag_ok = 0;
#define availability 0;
#define client -2;
#define full -3;









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

pthread_mutex_t mutex_full_theater = PTHREAD_MUTEX_INITIALIZER;//pthread_mutex_t mutex_transactions_no_sold_out_case= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_full_seats = PTHREAD_MUTEX_INITIALIZER;//pthread_mutex_t mutex_transactions_no_adequate_seats_case = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_success = PTHREAD_MUTEX_INITIALIZER;//pthread_mutex_t mutex_transactions_no_succesfull_trans = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t availableCondTelephones = PTHREAD_COND_INITIALIZER;
pthread_cond_t availableCondCashiers = PTHREAD_COND_INITIALIZER;

int available_tel = NO_TEL;
int cashiers_available = NO_CASH;
int deposit;
double avgWaitTime = 0; //Averace Client Waiting Time
double avgServeTime = 0; //Averace Client Servicing Time
int totalClientWaiting;
int availableSeats;
int exchangeCnt; //transaction ID in increasing order
int seats[NO_SEATS_PER_ROW][NO_ZONE_ALPHA + NO_ZONE_BETA];


int full_theater; //statistics
int full_seats;
int cnt_transaction;

/*
int no_sold_out_case; //statistics
int no_adequate_seats_case;
int no_succesfull_trans;
*/


void errorHandler(int answer);
int reserveSeats(int client_id, int sum_of_seats, int * zone, int cost, int client_seats);


int main(int argc, char *argv[]){

		int rc;

    int CLIENT_N = atoi(argv[1]);
    id = (int*)malloc(CLIENT_N * sizeof(int));
    
	int client_id[CLIENT_N];
	pthread_t threads[CLIENT_N];
	for(int i = 0; i < CLIENT_N; ++i)

	{
		client_id[i] = i;
		threads[i] = i;
	}




    seed = (unsigned int) atoi(argv[2]);

			
for(i = 0; i < 100; i++){
	seatsA[i] = -1;
}

for(j = 0; j < 200; j++){
	seatsB[j] = -1;
}









/*Create threads*/
	pthread_t threads[CLIENT_N];
	for(int i=0; i<CLIENT_N; ++i)
	{
		pthread_create(&threads[i], NULL, &run, &client_id[i]);
	}

/*Wait all threads to finish*/
	for(int i=0; i<CLIENT_N; ++i)
	{
		pthread_join(threads[i], NULL);
	}
//This finishes the procedure of the program









free(seats);//release allocated memory 
return 0;

}
























//In this function we make the call between client and system 
void* run (void* clientID){ //clientID binds with the threadID in main 

		    int id = *(int *)threadId; // int *id = (int *)threadId;

			int rc;
			int done = done_complete;

			struct timespec threadStart;//for client

			struct timespec service_time;//for client

			struct timespec cashier_start;//for cashier

			struct timespec telThreadStart;

			struct timespec telThreadEnd;//for client

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

	//It begins service time
	clock_gettime(CLOCK_REALTIME, &service_time); 
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

		 //Randomize the booking procedure and initiate the phone call
	int randomSeats = rand_r(&randSeed) % NO_SEAT_HIGH + NO_SEAT_LOW;
	sleep(rand_r(&randSeed) % TIME_SEAT_HIGH + TIME_SEAT_LOW);


	answer = pthread_mutex_unlock(&mutex_no_available_telephones);
	errorHandler(answer);
	clock_gettime(CLOCK_REALTIME, &waiting_time);


	double zoneSelect = (rand_r(&randSeed) % 100)/100.0f; //This is the input of the user in order to choose a random zone 
	int * zone;//preselected zone 
	double cost;
	int seats_per_zone;


	pthread_mutex_t mutex_zone;//init mutex 


	//In here we will have to check for the availability of the zones depending on the zones 
	//This part computes the probability of availability 
		

     if (zoneSelect <= P_ZONE_ALPHA){
		 zone = NO_ZONE_ALPHA; 
		 mutex_zone = mutex_zoneA;
		 seats_per_zone = NO_ZONE_ALPHA * NO_SEATS_PER_ROW;
		 cost = COST_PER_SEAT_ZONE_A;
	 }
	else {
	     zone = NO_ZONE_BETA;
		 mutex_zone = mutex_zoneB;
		 seats_per_zone = NO_ZONE_BETA * NO_SEATS_PER_ROW;
		 cost = COST_PER_SEAT_ZONE_B;	
	}

    answer = pthread_mutex_lock(&mutex_zone);
	errorHandler(answer);

	int value = reserveSeats(client_id, seats_per_zone, zone, randomSeats);

	answer = pthread_mutex_unlock(&mutex_zone);
	errorHandler(answer);

	pthread_mutex_lock(&mutex_no_available_telephones);
	available_tel++;
	pthread_cond_signal(&availableCondTelephones);
	pthread_mutex_unlock(mutex_no_available_telephones);

	clock_gettime(CLOCK_REALTIME, &telThreadEnd);//the phone call ends here
	clock_gettime(CLOCK_REALTIME, &threadEnd);//for client service
	clock_gettime(CLOCK_REALTIME, &cashier_start);//starts assigning cashier

   switch(answer){

	case full_seats_check :
		   //There are not seats
		flag = flag_not_available_seats;
		
		answer = pthread_mutex_lock(&mutex_full_seats);
		errorHandler(answer);
		full_seats++;
		answer = pthread_mutex_unlock(&mutex_full_seats);
		errorHandler(answer);

	case full_theater_check :

		flag = flag_sold_out; 
		answer = pthread_mutex_lock(&mutex_full_theater);
		errorHandler(answer);
		full_theater++;
		answer = pthread_mutex_unlock(&mutex_full_theater);
		errorHandler(answer);

	default:

		   answer = pthread_mutex_unlock(&mutex_cashiers);
	        errorHandler(answer);

			
        	while(cashiers_available <= 0)
		{
			pthread_cond_wait(&availableCondCashiers ,&mutex_cashiers); //Block until are available cashiers
	    	}
		    cashiers_available--;
	    	answer = pthread_mutex_unlock(&mutex_cashiers);
	    	errorHandler(answer);

   }



}



int reserveSeats(int client_id, int sum_of_seats, int * zone, int client_seats){
     
	int rows = 300 /  NO_SEATS_PER_ROW;

	 int totalSeats;



		for(int i = 0; i < rows*10; i++){
			

				if(zone[i] == -1 && (i % 10 == 1)){
				totalSeats++;	
			
				}else{totalSeats = 0;}

				if(totalSeats == 0 && i % 10 == 0){
					totalSeats++;
				}
	           


	        
			if(totalSeats == client_seats){
				for(j = totalSeats, j > 0, --j){
				zone[i - j] = client_id;
				}
				return 0;//true statement
			}

		
		}
		return -1;//false statement ## (error handling values)
}


void errorHandler(int answer)
{
	if (answer != 0) 
    {	
		printf("error in handling the mutex with referred error code %d\n", answer);
		pthread_exit(&answer);
	}
}

