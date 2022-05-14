//
// Created by peter on 5/5/2022.
//

#include "p3170173-p3170044-res.h"



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



unsigned int seed;






