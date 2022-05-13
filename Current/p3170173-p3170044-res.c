//
// Created by peter on 5/5/2022.
//

#include "p3170173-p3170044-res.h"



void assert_successful_mutex_action(int response_code);

//Mutexes
pthread_mutex_t mutex_no_available_phones= PTHREAD_MUTEX_INITIALIZER;



int no_available_phones;
int * seats;



unsigned int seed;






