//
// Created by peter on 5/5/2022.
//

#include "p3170173-p3170044-res.h"


/*
 * External Variables that have access to each and every thread
*/
int nCust; //This counts how many customers we have
int randSeed; //seed of the generator
int deposit; //this stores the account balance
int exchangeID; //this stores the number of the transaction
int availableTelephones; //this stores the number of the available telephones
double avgWaitTime; //Average Client Waiting Time
double avgServeTime; //Average Client Servicing Time
int seatPlan[Nseat]; //Theater Map
int availableSeats; //This stores the availability of the seats
int waitTotal; // total client waiting time


void assert_successful_mutex_action(int response_code);


















//Mutex Initialization
/*
 * Telephones
 * Seats
 * Account Balance
 *Console Output->Lock(Screen)
 * Services
 */
pthread_mutex_t mutex_no_available_telephones= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seats_available = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_screen = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t account_balance = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_on_hold = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t total_time = PTHREAD_MUTEX_INITIALIZER;




int no_available_phones;
int * seats;
int * customerID;




unsigned int seed;  //seed of the random generatort 




void * run(void * customer_id){
    
clock_gettime(CLOCK_REALTIME, &beginClock); //Here the timer is set

    struct timespec beginClock;
	struct timespec	inRow;
	struct timespec endClock;

























}






