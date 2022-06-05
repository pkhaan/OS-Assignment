//#######################################################
//#######################################################
//#######################################################
//#######################################################
//--------------Petros Chanas[3170173]-------------------
//--------------Alex Dounakis[3170044]-------------------
//--------------EL.Michailidis[3170110]------------------
//#######################################################
//#######################################################
//#######################################################
//#######################################################
//#######################################################
//#######################################################

/*
In order to compile the program there are 2 alternatives

Either run gcc -o theater -pthread filename.c filename.h 
and then run ./theater [number_of_clients] [seed]

Or edit the test-res.sh file with the values of your choosing
and then just execute it from the bash like this ./test-res.sh

*/



//Necessary Libraries
#include "p3170173-p3170044-p3170110-res.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>




//In here we initialize all the necessary mutexes required for the parameters of the system

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

//Conditional Mutexes that check the availability of cashiers or telephones
pthread_cond_t availablePhones = PTHREAD_COND_INITIALIZER;
pthread_cond_t availableCashiers = PTHREAD_COND_INITIALIZER;

//GLOBAL VARIABLES
int cnt_cardFail; 
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

//Functions are declared here as global because the are written after the main finction of the program 
void successReservation(int id, int * zone, int seats_per_zone,int randomSeats, double cost);
void * run(void * client_id);
int reserveSeats(int id,int * zone,int  _totalSeats,int _randomSeats);
void emptyReservation(int id, int * zone, int seats);

int main(int argc, char *argv[]){

//In here we check if the program takes the necessary parameters
	if (argc != 3) {

		printf("ERROR: Program must take 2 arguments\n\n\n");
		return(0);
	}

	int CLIENT_N = atoi(argv[1]);
	//You cannot give ac negative amount of clients
	if (CLIENT_N <= 0) {
		printf("Try again, client number must be positive\n\n\n");
		return(0);
	}

	//Directly from the header
	telephones = NO_TEL;
	cashiers = NO_CASH;
	//These variables are used for evaluation in the final output of the program
	cnt_cardFail = 0;
	required_seats = 0;
	cnt_transaction = 0;
	waitingTime = 0;
	serviceTime = 0;

	seed = atoi(argv[2]);

	int no_seats = NO_SEATS_PER_ROW;
	int no_rows_A = NO_ZONE_ALPHA;
	int no_rows_B = NO_ZONE_BETA;
	
	printf("\n############################## \nWelcome to theater management system \n################\nYoy have chosen %d clients and seed %d \n \n \n", CLIENT_N, seed);
	
	//This is the plan of seats for the first zone
	int num_of_seats_A = no_seats * no_rows_A;
	seatsA = malloc(sizeof(int)*num_of_seats_A);
	for(int i=0;i<num_of_seats_A;++i)
	{
		seatsA[i] = -99;
	}

	//This is the plan of seats for the second zone
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
    //Initialize threads
	pthread_t threads[CLIENT_N];
	for(int i=0; i<CLIENT_N; ++i)
	{
		pthread_create(&threads[i], NULL, &run, &clients[i]);
	}


	//Holding Process: Until the end of the clients been given we say to the thread that in needs to wait	
	for(int i=0; i<CLIENT_N; ++i)
	{
		pthread_join(threads[i], NULL);
	}


	
	printf("\n\nTHIS IS ZONE A \n");
	for (int i = 0; i < no_seats*no_rows_A; i++) 
	{	
		if(seatsA[i] != -99){
			printf("seat %d client %d \n \n", i, seatsA[i]);
		}
		else{
			printf("seat %d nobody \n", i);	
		}
		
	}
	printf("\n\nTHIS IS ZONE B \n");
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
printf("\n \n average waiting time is: %f \n \n", waitingTime / CLIENT_N);
printf("\n \n total servicing time is: %f \n \n", serviceTime / CLIENT_N);
printf("\n \n percentage of successful payments is: %f \n \n", cnt_transaction*1.0f / CLIENT_N);
printf("\n \n percentage of card failures is: %f \n \n", cnt_cardFail*1.0f / CLIENT_N);
printf("\n \n percentage of clients that found no seats is: %f \n \n", required_seats*1.0f / CLIENT_N);

	//Free allocated memory 
	free(seatsA);
	free(seatsB);

	return 0;

}


//The function that emulates the call 
void * run(void * client_id){

	int rc;

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



	rc = pthread_mutex_lock(&mutex_telephones);

	 if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }



	while(telephones <=0){
		rc = pthread_cond_wait(&availablePhones ,&mutex_telephones);
		if(rc != 0){
        printf("Mutex: Error in condition with code %d\n", rc);
        pthread_exit(&rc);
    }
	}
	telephones--;

	clock_gettime(CLOCK_REALTIME, &client_service);

	rc = pthread_mutex_unlock(&mutex_telephones);
	if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }

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
	
	rc = pthread_mutex_lock(&zone);
	if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
	int value = reserveSeats(id, zoneArr, totalSeats, randomSeats);
	rc = pthread_mutex_unlock(&zone);
	if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }


	rc = pthread_mutex_lock(&mutex_telephones);
	if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
	++telephones;
	rc = pthread_cond_signal(&availablePhones);
	if(rc != 0){
        printf("Mutex: Error in condition with code %d\n", rc);
        pthread_exit(&rc);
    }
	rc = pthread_mutex_unlock(&mutex_telephones);
	if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }
	
	clock_gettime(CLOCK_REALTIME, &tel_end);
	clock_gettime(CLOCK_REALTIME, &client_end); 
	clock_gettime(CLOCK_REALTIME, &cashier);

	// did not find seats
	if(value != 200){ //OPCODE SUCCESS
		rc = pthread_mutex_lock(&mutex_complete_reservations);
		if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
		required_seats++;
		rc = pthread_mutex_unlock(&mutex_complete_reservations);
		if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }
		
		printf("NO SEATS FOUND...\n");
	}else{// found seats
		successReservation(id , zoneArr , totalSeats , randomSeats , cost);
		// waiting for cashier
		rc = pthread_mutex_lock(&mutex_cashiers);
		if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
		while(cashiers <= 0)
		{
			rc = pthread_cond_wait(&availableCashiers ,&mutex_cashiers); 
			if(rc != 0){
        printf("Mutex: Error in condition with code %d\n", rc);
        pthread_exit(&rc);
    }
	    	}
		--cashiers;
		rc = pthread_mutex_unlock(&mutex_cashiers);
		if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }

		// cashier found
		clock_gettime(CLOCK_REALTIME, &cashier);

		bool toDoTransaction = rand_r(&seed) % 100 / 100.0f <= P_CARD_SUCCESS; 
		
		if(toDoTransaction){
			rc = pthread_mutex_lock(&mutex_complete_transactions);
			if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    	}
			cnt_transaction++;
			rc = pthread_mutex_unlock(&mutex_complete_transactions);
				if(rc != 0){
			printf("Mutex: Error in unlocking with code %d\n", rc);
			pthread_exit(&rc);
		}
			// Update deposits
			rc = pthread_mutex_lock(&mutex_account_balance);
				if(rc != 0){
			printf("Mutex: Error in locking with code %d\n", rc);
			pthread_exit(&rc);
		}
			deposit += randomSeats * cost;
			rc = pthread_mutex_unlock(&mutex_account_balance);
				if(rc != 0){
			printf("Mutex: Error in unlocking with code %d\n", rc);
			pthread_exit(&rc);
		}

			printf("Successful Reservation \n the id of the client is:  %d \n and the reserved : ", id);
			for (int i = 0; i < totalSeats; i++){

				if (zoneArr[i] == id) printf("%d seat, ", i);
			}
			printf("\nAmount that is due to payment in euros is %.2f \n\n", randomSeats * cost);
			
		}else{
			rc = pthread_mutex_lock(&mutex_cardFail);
			if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
			cnt_cardFail++;
			rc = pthread_mutex_unlock(&mutex_cardFail);
				if(rc != 0){
			printf("Mutex: Error in unlocking with code %d\n", rc);
			pthread_exit(&rc);
		}
			// release the seats that have been up until now reserved
			emptyReservation(id, zoneArr, totalSeats);
			printf("\nCARD NOT VALID.... \n");
		}
		rc = pthread_mutex_lock(&mutex_cashiers);
			if(rc != 0){
			printf("Mutex: Error in locking with code %d\n", rc);
			pthread_exit(&rc);
		}
		++cashiers;
		rc = pthread_cond_signal(&availableCashiers);
				if(rc != 0){
        printf("Mutex: Error in condition with code %d\n", rc);
        pthread_exit(&rc);
    }
		rc = pthread_mutex_unlock(&mutex_cashiers);
		if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }
		
	}
	
	clock_gettime(CLOCK_REALTIME, &client_end);

	rc = pthread_mutex_lock(&mutex_terminal);
	if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
	rc = pthread_mutex_unlock(&mutex_terminal);
	if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }
		
	rc = pthread_mutex_lock(&mutex_total_time_on_hold);
	if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
	waitingTime += (client_service.tv_sec - client_begin.tv_sec) + (cashier.tv_sec - tel_end.tv_sec);
	rc = pthread_mutex_unlock(&mutex_total_time_on_hold);
	if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }

	rc = pthread_mutex_lock(&mutex_total_service_time);
	if(rc != 0){
        printf("Mutex: Error in locking with code %d\n", rc);
        pthread_exit(&rc);
    }
	serviceTime += (client_end.tv_sec - client_service.tv_sec) + (client_end.tv_sec - cashier.tv_sec);
	rc = pthread_mutex_unlock(&mutex_total_service_time);
	if(rc != 0){
        printf("Mutex: Error in unlocking with code %d\n", rc);
        pthread_exit(&rc);
    }


	pthread_exit(NULL);

}




//This is the function that parses the theater and finds any available seats in continuous order
//and in the reserved seat it replaces the -99 empty signal with the id of the client
int reserveSeats(int id, int * zone, int  _totalSeats, int _randomSeats){

	bool flag = false;
	int position_of_pointer;
	int rows = _totalSeats / 10;
       if(_randomSeats != 0){
	for(int rowIndex = 0; rowIndex < rows; rowIndex++){
		int sequential = 0;

		for(int columnIndex=0; columnIndex < 10; columnIndex++){
			if(zone[rowIndex*10 + columnIndex] == -99){
				++sequential;
			}else{
				sequential = 0;
			}
			if(sequential == _randomSeats){
				position_of_pointer = columnIndex;
				flag = true;
			}
		}
		if(flag){
			for(int j  = position_of_pointer; j > position_of_pointer - _randomSeats;j--){
				zone[rowIndex*10 + j] = id;
			}
			return 200;//success opcode
		}
	}
}
	return 404;//failure opcode
}

void successReservation(int id, int * zone, int seats_per_zone, int randomSeats, double cost){


	for (int i = 0; i < seats_per_zone; i++)
	{
		if (zone[i] == id){
		 printf("%d seat, ", i);
		}
	}
	printf("\n\nTotal Amount for credit is  %.4f € \n\n", randomSeats*cost);	

}
//Empty the reserved seat
void emptyReservation(int id, int * zone, int seats){
	for(int i=0;i<seats;i++)
	{
		if(zone[i] == id) {
			zone[i] = -99;
			}  
	}
}
