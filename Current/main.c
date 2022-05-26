
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "main.h"

pthread_mutex_t mutex_telephones= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cashiers = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_account_balance = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_terminal = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_total_time_on_hold = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_total_service_time = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_zoneA= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_zoneB = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutex_cardFail= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_complete_reservations = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_complete_transactions = PTHREAD_MUTEX_INITIALIZER;



//Condition
pthread_cond_t availablePhones = PTHREAD_COND_INITIALIZER;
pthread_cond_t availableCashiers = PTHREAD_COND_INITIALIZER;


int cnt_cardFail; //statistics
int required_seats;
int cnt_transaction;


int telephones;
int cashiers;


int * seatsA;
int * seatsB;

double waitingTime;
double serviceTime;

double deposit;


unsigned int seed;


void successReservation(int id, int * zone, int seats_per_zone,int randomSeats, double cost);
void * run(void * client_id);
int reserveSeats(int id,int * zone,int  _totalSeats,int _randomSeats);
void emptyReservation(int id, int * zone, int seats);

int main(int argc, char *argv[]){


	if (argc != 3) {

		printf("ERROR: Program must take 2 arguments\n\n\n");
		return(0);
	}

	int CLIENT_N = atoi(argv[1]);
	if (CLIENT_N <= 0) {
		printf("Try again, client number must be positive\n\n\n");
		return(0);
	}

	telephones = NO_TEL;
	cashiers = NO_CASH;

	cnt_cardFail = 0;
	required_seats = 0;
	cnt_transaction = 0;

	waitingTime = 0;
	serviceTime = 0;

	seed = atoi(argv[2]);

	int no_seats = NO_SEATS_PER_ROW;
	int no_rows_A = NO_ZONE_ALPHA;
	int no_rows_B = NO_ZONE_BETA;

	printf("\n \nWelcome to theater management system \nYoy have chosen %d clients and seed %d \n \n \n", CLIENT_N, seed);

	int num_of_seats_A = no_seats * no_rows_A;
	seatsA = malloc(sizeof(int)*num_of_seats_A);
	for(int i=0;i<num_of_seats_A;++i)
	{
		seatsA[i] = -99;
	}


	int num_of_seats_B = no_seats * no_rows_B;
	seatsB = malloc(sizeof(int)*num_of_seats_B);
	for(int i=0;i<num_of_seats_B;++i)
	{
		seatsB[i] = -99;
	}


	int clients[CLIENT_N];
	for(int i=0;i<CLIENT_N;++i)
	{
		clients[i] = i;
	}

	pthread_t threads[CLIENT_N];
	for(int i=0; i<CLIENT_N; ++i)
	{
		pthread_create(&threads[i], NULL, &run, &clients[i]);
	}

	for(int i=0; i<CLIENT_N; ++i)
	{
		pthread_join(threads[i], NULL);
	}


	
	printf("THIS IS ZONE A \n");
	for (int i = 0; i < no_seats*no_rows_A; i++) 
	{	
		if(seatsA[i] != -99){
			printf("seat %d client %d \n \n", i, seatsA[i]);
		}
		else{
			printf("seat %d nobody \n", i);	
		}
		
	}
	printf("THIS IS ZONE B \n");
	for (int i = 0; i < no_seats*no_rows_B; i++) 
	{	
		if(seatsB[i] != -99){
			printf("seat %d client %d \n", i, seatsB[i]);
		}
		else{
			printf("seat %d nobody \n", i);	
		}
		
	}


printf("\n \nTotal Balance is :%f \n \n", deposit);
printf("\n \n average wating time is: %f \n \n", waitingTime / CLIENT_N);
printf("\n \n total servicing time is: %f \n \n", serviceTime / CLIENT_N);
printf("\n \n percentage of successful payments is: %f \n \n", cnt_transaction*1.0f / CLIENT_N);
printf("\n \n percentage of card failures is: %f \n \n", cnt_cardFail*1.0f / CLIENT_N);
printf("\n \n percentage of clients that found no seats is: %f \n \n", required_seats*1.0f / CLIENT_N);

	free(seatsA);
	free(seatsB);

	return 0;

}



void * run(void * client_id){

	

	//int answer;
	int done;
	int id = *(int*) client_id;


	struct timespec client_begin;
	struct timespec	client_service;
	struct timespec tel_end;
	struct timespec cashier;
	struct timespec client_end;

	int randomSeats = rand_r(&seed) % NO_SEAT_HIGH + NO_SEAT_LOW;
	int randomTime = rand_r(&seed) % TIME_SEAT_HIGH + TIME_SEAT_LOW;
	float randomZone = (rand_r(&seed) % 100)/100.0f;


	clock_gettime(CLOCK_REALTIME, &client_begin); 



	pthread_mutex_lock(&mutex_telephones);


	while(telephones <=0){
		pthread_cond_wait(&availablePhones ,&mutex_telephones);
	}
	telephones--;

	clock_gettime(CLOCK_REALTIME, &client_service);

	pthread_mutex_unlock(&mutex_telephones);

	sleep(randomTime);

	pthread_mutex_t zone;
	int *zoneArr;
	double cost;
	int totalSeats;
	int no_seats_A = NO_ZONE_ALPHA;
	int no_seats_B = NO_ZONE_BETA;
	int columns = NO_SEATS_PER_ROW;

	bool check = randomZone <= P_ZONE_ALPHA;
	
	if (check){
		zoneArr = seatsA;
		zone = mutex_zoneA;
		cost = COST_PER_SEAT_ZONE_A;
		totalSeats = no_seats_A * columns;
		printf("A \n");
	}else{
		zoneArr = seatsB;
		zone = mutex_zoneB;
		cost = COST_PER_SEAT_ZONE_B;
		totalSeats = no_seats_B * columns;
		printf("B \n");
	}
	printf("id %d \n zone %d \n cost %f \n total Seats %d\n random seats  %d \n time %d \n", id,* zoneArr, cost, totalSeats, randomSeats, randomTime);
	pthread_mutex_lock(&zone);
	int value = reserveSeats(id, zoneArr, totalSeats, randomSeats);
	pthread_mutex_unlock(&zone);


	pthread_mutex_lock(&mutex_telephones);
	++telephones;
	pthread_cond_signal(&availablePhones);
	pthread_mutex_unlock(&mutex_telephones);
	
	clock_gettime(CLOCK_REALTIME, &tel_end);
	clock_gettime(CLOCK_REALTIME, &client_end); 
	clock_gettime(CLOCK_REALTIME, &cashier);

	// did not find seats
	if(value != 200){
		pthread_mutex_lock(&mutex_complete_reservations);
		required_seats++;
		pthread_mutex_unlock(&mutex_complete_reservations);
		
		printf("NO SEATS FOUND...\n");
	}else{// found seats
		successReservation(id , zoneArr , totalSeats , randomSeats , cost);
		// waiting for cashier
		pthread_mutex_lock(&mutex_cashiers);
		while(cashiers <= 0)
		{
			pthread_cond_wait(&availableCashiers ,&mutex_cashiers); //Block until are available cashiers
	    	}
		--cashiers;
		pthread_mutex_unlock(&mutex_cashiers);

		// got cashier
		clock_gettime(CLOCK_REALTIME, &cashier);

		bool toDoTransaction = rand_r(&seed) % 100 / 100.0f <= P_CARD_SUCCESS; 
		
		if(toDoTransaction){
			pthread_mutex_lock(&mutex_complete_transactions);
			cnt_transaction++;
			pthread_mutex_unlock(&mutex_complete_transactions);
			
			// Update deposits
			pthread_mutex_lock(&mutex_account_balance);
			deposit += randomSeats * cost;
			pthread_mutex_unlock(&mutex_account_balance);

			printf("Successful Reservation \n the id of the client is:  %d \n and the reserved : ", id);
			for (int i = 0; i < totalSeats; i++){

				if (zoneArr[i] == id) printf("%d seat, ", i);
			}
			printf("\nAmount that is due to payment in euros is %.2f \n\n", randomSeats * cost);
			
		}else{
			pthread_mutex_lock(&mutex_cardFail);
			cnt_cardFail++;
			pthread_mutex_unlock(&mutex_cardFail);
			// release seats 
			emptyReservation(id, zoneArr, totalSeats);
			printf("\nCARD NOT VALID.... \n");
		}
		// release cashier 
		pthread_mutex_lock(&mutex_cashiers);
		++cashiers;
		pthread_cond_signal(&availableCashiers);
		pthread_mutex_unlock(&mutex_cashiers);
		
	}
	
	clock_gettime(CLOCK_REALTIME, &client_end);

	pthread_mutex_lock(&mutex_terminal);
	pthread_mutex_unlock(&mutex_terminal);
		
	pthread_mutex_lock(&mutex_total_time_on_hold);
	waitingTime += (client_service.tv_sec - client_begin.tv_sec) + (cashier.tv_sec - tel_end.tv_sec);
	pthread_mutex_unlock(&mutex_total_time_on_hold);

	pthread_mutex_lock(&mutex_total_service_time);
	serviceTime += (client_end.tv_sec - client_service.tv_sec) + (client_end.tv_sec - cashier.tv_sec);
	pthread_mutex_unlock(&mutex_total_service_time);


	pthread_exit(NULL);

}


int reserveSeats(int id,int * zone,int  _totalSeats,int _randomSeats){

	bool flag = false;
	int position_of_pointer;
	int rows = _totalSeats / 10;

	for(int rowIndex = 0; rowIndex < rows; rowIndex++){
		int sequential = 0;

		for(int columnIndex=0; columnIndex < 10;columnIndex++){
			if(zone[rowIndex*10 + columnIndex] == -99){
				++sequential;
				// if(_randomSeats == sequential){
				// 	temp = rowIndex;

				// }
			}else{
				sequential = 0;
			}
			if(sequential == _randomSeats){
				position_of_pointer = columnIndex;
				flag = true;
				//break;
			}
		}
		if(flag){
			for(int j  = position_of_pointer;j > position_of_pointer-_randomSeats;j--){
				zone[rowIndex*10 + j] = id;
			}
			return 200;//correct opcode
		}
	}

	return 404;//error opcode
}

void successReservation(int id, int * zone, int seats_per_zone, int randomSeats, double cost){


	for (int i = 0; i < seats_per_zone; i++)
	{
		if (zone[i] == id){
		 printf("%d seat, ", i);
		}
	}
	printf("\nTotal Amount for credit is  %.4f € \n\n", randomSeats*cost);	

}

void emptyReservation(int id, int * zone, int seats){
	for(int i=0;i<seats;i++)
	{
		if(zone[i] == id) {
			zone[i] = -99;
			}  
	}
}