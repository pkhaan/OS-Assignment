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
#define done_complete 0; //const int flag_ok = 0;
#define availability -1;
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


void* run (void* clientID);
int reserveSeats(int client_id, int sum_of_seats, int * zone, int cost, int client_seats);
void errorHandler(int answer);
void full_theater_func();
void availabilityError();
void transactionError();
void completion_imminent(int client_id, const int* map, int sum_of_seats, int seats_per_zone, double cost);

int main(int argc, char *argv[]) {

	int rc;
    if(argc != 3){printf("Error: Program takes 2 arguments"); return(0);}
	
    int CLIENT_N = atoi(argv[1]);
		if (CLIENT_N <= 0) {
		printf("TRY AGAIN: Client number must be positive\n");
		exit(-1);
	}

    id = (int*)malloc(CLIENT_N * sizeof(int));
    
	int client_id[CLIENT_N];
	pthread_t threads[CLIENT_N];
	for(int i = 0; i < CLIENT_N; i++)

	{
		client_id[i] = i;
		threads[i] = i;
	}


    randSeed = (unsigned int) atoi(argv[2]);


int number_of_seats_A = 10 * 10;
int number_of_seats_B = 10 * 20;

seatsA = (int*)malloc(sizeof(int)*number_of_seats_A);
for(int i = 0; i < number_of_seats_A ; i++){
	seatsA[i] = -1;
}

seatsB = (int*)malloc(sizeof(int)*number_of_seats_B);
for(int j = 0; j < number_of_seats_B; j++){
	seatsB[j] = -1;
}

/*Create threads*/

	for(int i=0; i < CLIENT_N; i++)
	{
		pthread_create(&threads[i], NULL, &run, &client_id[i]);
	}


	for(int i = 1; i < CLIENT_N; i++)
	{
		pthread_join(threads[i], NULL);
	}
//This finishes the procedure of the program
printf("##########################################################################\n");
printf("##########################################################################\n");
printf("#################################THEATER##################################\n");
printf("In here we map the theater per zone and we assign the seat to each CLIENT\n");

	for (int i = 0; i < number_of_seats_A; i++) 
	{	
		if(seatsA[i] != -1){
			printf("The seat %d is assigned to the client %d in zone A\n", i, seatsA[i]);
		}
		else{
			printf("RESERVED %d \n", i);	
		}
		
	}
	for (int i = 0; i < number_of_seats_B; i++) 
	{	
		if(seatsB[i] != -1){
			printf("The seat %d is assigned to the client %d in zone B\n", i, seatsB[i]);
		}
		else{
			printf("RESERVED %d \n", i);	
		}
		
	}
//int available_tel = NO_TEL;
//int cashiers_available = NO_CASH;
//int deposit;
//double avgWaitTime = 0; //Average Client Waiting Time
//double avgServeTime = 0; //Average Client Servicing Time
//int totalClientWaiting;
//int serviceTime
//int availableSeats;
//int exchangeCnt;

printf("Total Balance %d", deposit);
printf("%d", totalClientWaiting / CLIENT_N);
//printf("%d", (serviceTime / CLIENT_N));


//release allocated memory -> MALLOC USAGE
free(seatsA);
free(seatsB);



return 0;


}
	
//In this function we make the call between client and system 
void* run (void* clientID){ //clientID binds with the threadID in main 

		    int client_id = *(int *)clientID; // int *id = (int *)threadId;

			int rc;

			int answer;

			int done = done_complete;

			struct timespec threadStart;//for client

			struct timespec service_time;//for client

			struct timespec waiting_time;

			struct timespec cashier_start;//for cashier

			struct timespec telThreadStart;

			struct timespec telThreadEnd;//for client

			struct timespec threadEnd;//for client

		 clock_gettime(CLOCK_REALTIME, &threadStart);
		
		answer = pthread_mutex_lock(&mutex_no_available_telephones);
		errorHandler(answer);//function the checks locking status of mutex
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
	sleep(randomSeats);


	answer = pthread_mutex_unlock(&mutex_no_available_telephones);
	errorHandler(answer);
	clock_gettime(CLOCK_REALTIME, &waiting_time);


	float zoneSelect = (rand_r(&randSeed) % 100)/100.0f; //This is the input of the user in order to choose a random zone 
	int * zone;//preselected zone 
	pthread_mutex_t mutex_zone;
	double cost;
	int seats_per_zone;


	pthread_mutex_t mutex_per_zone;//init mutex 


	//In here we will have to check for the availability of the zones depending on the zones 
	//This part computes the probability of availability 
		
		bool check = zoneSelect <= P_ZONE_ALPHA;
		int noA = NO_ZONE_ALPHA;
		int noB = NO_ZONE_BETA;
		int seats_row = NO_SEATS_PER_ROW;
     if (check){
		 zone = seatsA; 
		 mutex_zone = mutex_zoneA;
		 seats_per_zone = noA * seats_row;
		 cost = COST_PER_SEAT_ZONE_A;
	 }
	else {
	     zone = seatsB;
		 mutex_zone = mutex_zoneB;
		 seats_per_zone = noB * seats_row;
		 cost = COST_PER_SEAT_ZONE_B;	
	}

    answer = pthread_mutex_lock(&mutex_zone);
	errorHandler(answer);

	int value = reserveSeats(client_id, seats_per_zone, zone,cost, randomSeats);

	answer = pthread_mutex_unlock(&mutex_zone);
	errorHandler(answer);

	pthread_mutex_lock(&mutex_no_available_telephones);
	available_tel++;
	pthread_cond_signal(&availableCondTelephones);
	pthread_mutex_unlock(&mutex_no_available_telephones);

	clock_gettime(CLOCK_REALTIME, &telThreadEnd);//the phone call ends here
	clock_gettime(CLOCK_REALTIME, &threadEnd);//for client service
	clock_gettime(CLOCK_REALTIME, &cashier_start);//starts assigning cashier

   if(value == full_seats){
		   //There are not seats
		done = full_seats;
		
		answer = pthread_mutex_lock(&mutex_full_seats);
		errorHandler(answer);
		full_seats++;
		answer = pthread_mutex_unlock(&mutex_full_seats);
		errorHandler(answer);

   }else if(value == full_theater){

		done = full_theater; 
		answer = pthread_mutex_lock(&mutex_full_theater);
		errorHandler(answer);
		full_theater++;
		answer = pthread_mutex_unlock(&mutex_full_theater);
		errorHandler(answer);

	}else
	{
		   answer = pthread_mutex_unlock(&mutex_cashiers);
	        errorHandler(answer);

			
        	while(cashiers_available <= 0)
		{
			pthread_cond_wait(&availableCondCashiers ,&mutex_cashiers); //Block until are available cashiers
	    	}
		    cashiers_available--;
	    	answer = pthread_mutex_unlock(&mutex_cashiers);
	    	errorHandler(answer);

		//Cashier from now on.
		clock_gettime(CLOCK_REALTIME, &cashier_start);  //track starting time for cashier
		//This bool checks if the payment was completed 
		bool done_payment = rand_r(&randSeed) % 100 / 100.0f <= P_CARD_SUCCESS;  //Start paying

		if(!done_payment)
		{//Print user didn't pay
			done = client;
			//release seats
			answer = pthread_mutex_lock(&mutex_per_zone);
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
	}



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
	     completion_imminent(client_id, zone, zoneSelect, seats_per_zone, cost); //TODO we need to pass the zone as well,
	}
	answer = pthread_mutex_unlock(&terminalMutex);
	errorHandler(answer);

	//update total_waiting_time
	answer = pthread_mutex_lock(&time_on_hold);
	errorHandler(answer);
	avgWaitTime += (service_time.tv_sec - threadStart.tv_sec) + (cashier_start.tv_sec - telThreadEnd.tv_sec) ;
	answer = pthread_mutex_unlock(&time_on_hold);
	errorHandler(answer);

	//update total_service_time
	answer = pthread_mutex_lock(&total_time);
	errorHandler(answer);
	avgServeTime += (threadEnd.tv_sec - service_time.tv_sec) + (threadEnd.tv_sec - cashier_start.tv_sec);
	answer = pthread_mutex_unlock(&total_time);
	errorHandler(answer);

	pthread_exit(NULL);





}



int reserveSeats(int client_id, int sum_of_seats, int * zone, int cost, int client_seats){

		
		bool flag_for_switching_row = false;
		bool maxed_out;


		//int map = sum_of_seats / NO_SEATS_PER_ROW;
		int seats_stored;
		bool flag_end_search = false;
		int position_of_pointer = 0;
		int returnPosition;//flag


		int sum_seats;
		bool max_so_far = false;
		int flag;
		int index;
		int seats_comp = seats_completed;
		int seats_in_row = NO_SEATS_PER_ROW;
		int rows = sum_of_seats / seats_in_row;

		for (int i = 0; i < rows; i++){
				maxed_out = false;
				seats_stored = 0;
				for(int j = 0; j < seats_in_row; ++j){
					if(zone[(i)*seats_in_row + j] == -1){
						flag_end_search == true;
						++seats_stored;
						if(client_seats == seats_stored){
							position_of_pointer = j + i * NO_SEATS_PER_ROW;
							returnPosition = seats_completed;
							flag_for_switching_row = true;
							maxed_out = true;
							break;
						}
					}else{seats_stored = 0; maxed_out = false;}

				if(seats_stored == client_seats) break;
		}
		if(flag_end_search && returnPosition != seats_comp) return availability;
		if(returnPosition != seats_comp && flag_end_search == true) return full;

}//Change zone!
		for(int temp = position_of_pointer; temp > index - client_seats; --temp){
			zone[temp] = client_id;
			flag_for_switching_row = true;
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
	for (int i = 0; i < seats_per_zone; i++)
	{
		if (map[i] == client_id){
			printf("%d seat, ", i);
			}
		 
	}
	printf("\nPayment Amount: %f", sum_of_seats * cost);

}





void full_theater_func()
{
	printf("Theater is full, no availability\n");
}

void availabilityError()
{
    printf("the seats are all reserved\n");
}

 void transactionError(){
	 printf("payment failed due to credit card implications\n");
 }





