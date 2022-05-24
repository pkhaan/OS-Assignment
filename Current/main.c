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
//int Num;
//seats per every zone
int * seatsA;
int * seatsB;
//Variables of Usage from the system

long int tv_sec;
long int tv_usec;

//Mutex Initialization
/*
 * Telephones
 * Seats
 * Account Balance
 *Console Output->Lock(Screen)
 * Services
 */


#define seats_completed 300; //const int seats_found_success = 200;
#define done 0; //const int flag_ok = 0;
#define availability 0;
#define client -2;
#define full -3;


/*const int flag_not_available_seats = -1;

const int flag_customer_did_not_pay = -2;

const int flag_sold_out = -3;

const int seats_found_success = 200;

const int flag_ok = 0;*/


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
double avgWaitTime = 0; //Average Client Waiting Time
double avgServeTime = 0; //Average Client Servicing Time
int totalClientWaiting;
int availableSeats;
int exchangeCnt; //transaction ID in increasing order
//int seats[NO_SEATS_PER_ROW][NO_ZONE_ALPHA + NO_ZONE_BETA];


int full_theater; //statistics
int full_seats;
int cnt_transaction;

/*
int no_sold_out_case; //statistics
int no_adequate_seats_case;
int no_succesfull_trans;
*/



int reserveSeats(int client_id, int sum_of_seats, int * zone, int cost, int client_seats);
void errorHandler(int answer);
void full_theater_func();
void availabilityError();
 void transactionError()

int main(int argc, char *argv[]){

		int rc;

    int CLIENT_N = atoi(argv[1]);
    id = (int*)malloc(CLIENT_N * sizeof(int));
    
	int client_id[CLIENT_N];
	pthread_t threads[CLIENT_N];
	for(int i = 0; i < CLIENT_N; i++)

	{
		client_id[i] = i;
		threads[i] = i;
	}


    seed = (unsigned int) atoi(argv[2]);


int number_of_seats_A = NO_SEATS_PER_ROW * NO_ZONE_ALPHA;
int number_of_seats_B = NO_SEATS_PER_ROW * NO_ZONE_BETA;

seats_A = (int*)malloc(sizeof(int)*number_of_seats_A);
for(i = 0; i < NO_SEATS_PER_ROW * NO_ZONE_ALPHA ; i++){
	seatsA[i] = -1;
}

seats_B = (int*)malloc(sizeof(int)*number_of_seats_B);
for(j = 0; j < NO_SEATS_PER_ROW * NO_ZONE_BETA; j++){
	seatsB[j] = -1;
}

/*Create threads*/

	pthread_t threads[CLIENT_N];
	for(int i=0; i < CLIENT_N; i++)
	{
		pthread_create(&threads[i], NULL, &run, &client_id[i]);
	}


	for(int i = 1; i < CLIENT_N; i++)
	{
		pthread_join(threads[i], NULL);
	}
//This finishes the procedure of the program

	


//release allocated memory -> MALLOC USAGE
free(seatsA);
free(seatsB);



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


	pthread_mutex_t mutex_per_zone;//init mutex 


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


//Cashier from now on.
		clock_gettime(CLOCK_REALTIME, &cashier_start);  //track starting time for cashier
		//This bool checks if the payment was completed 
		bool done_payment = rand_r(&seed) % 100 / 100.0f <= P_CARD_SUCCESS;  //Start paying

		if(!done_payment)
		{//Print user didn't pay
			flag = flag_customer_did_not_pay ;
			//release seats
			response_code = pthread_mutex_lock(&mutex_per_zone);
			errorHandler(answer);
	  		//releaseBookedSeats(zone, total_seats_in_zone,  cid); //Release booked seats
			answer = pthread_mutex_unlock(&mutex_per_zone);
			errorHandler(answer);
		}
        
		else{

			answer = pthread_mutex_lock(&mutex_success);
			errorHandler(answer);
			cnt_transaction++;
			answer = pthread_mutex_unlock(&mutex_success);
			errorHandler(answer);
			
			//todo update transactions
			//Update balance
			answer = pthread_mutex_lock(&account_balance);
			errorHandler(answer);
			deposit += randomSeats * cost; //User pay
			answer = pthread_mutex_unlock(&account_balance);
			errorHandler(answer);
		}

		answer = pthread_mutex_lock(&mutex_cashiers);
		errorHandler(answer);
	    cashiers_available++;
	    pthread_cond_signal(&availableCondCashiers);
	   	answer = pthread_mutex_unlock(&mutex_cashiers);
	    errorHandler(answer);
	    clock_gettime(CLOCK_REALTIME, &threadEnd); //Track finish time




//Print transaction info
	answer = pthread_mutex_lock(&terminalMutex);
	errorHandler(answer);
	if(done == full_theater)
	{       
		full_theater_func();
	}
	else if(done == cnt_transaction)
	{
		 transactionError();
	}
	else if(done == full_seats)
	{

		availabilityError();	
	}
	else
	{
	     completion_imminent(id, zone, zoneSelect, total_seats_in_zone, cost_zone_per_seat); //TODO we need to pass the zone as well,
	}
	answer = pthread_mutex_unlock(&terminalMutex);
	errorHandler(answer);

	//update total_waiting_time
	answer = pthread_mutex_lock(&time_on_hold);
	errorHandler(answer);
	avgWaitTime += (customer_serviced.tv_sec - customer_start.tv_sec) + (customer_cashier_start.tv_sec - customer_telephone_finish.tv_sec) ;
	answer = pthread_mutex_unlock(&time_on_hold);
	errorHandler(answer)

	//update total_service_time
	answer = pthread_mutex_lock(&total_time);
	errorHandler(answer);
	avgServeTime += (customer_finish.tv_sec - customer_serviced.tv_sec) + (customer_finish.tv_sec - customer_cashier_start.tv_sec);
	answer = pthread_mutex_unlock(&total_time);
	errorHandler(answer);

	pthread_exit(NULL);





}



int reserveSeats(int client_id, int sum_of_seats, int * zone, int client_seats){

		bool flag_end_search;
		bool flag_for_switching_row;
		bool maxed_out;


		//int map = sum_of_seats / NO_SEATS_PER_ROW;
		int position_of_pointer = 0;
		int seats_stored;
		int returnPosition;//flag



		for (int i = 0; i < sum_of_seats / NO_SEATS_PER_ROW; i++){








		}






	
}

//release all the seats booked from customer with id: cid 
void releaseReservedSeats(int client_id, int sum_of_seats, int * zone)
{
	for(int i=0;i < sum_of_seats; i++)
	{
		if(zone[i] == client_id)  {
			zone[i] = -1;//empty the seats
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

void completion_imminent(int client_id, const int* map, int sum_of_seats, int seats_per_zone, double cost)
{
	printf("[Success]: \n seats reserved %d \n for client: ", client_id);
	for (int i = 0; i < zone_total_seats; i++)
	{
		if (map[i] == client_id){
			printf("%d seat, ", i);
			}
		 
	}
	printf("\nPayment Amount: ", sum_of_seats * cost);

}






void full_theater_func()
{
	printf("Theater is full, no availability\n");
}

void availabilityError()
{
    printf("the seats are all reserved");
}

 void transactionError(){
	 printf("payment failed due to credit card implications")
 }





