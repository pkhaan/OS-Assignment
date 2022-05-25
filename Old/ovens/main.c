#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Global variables
unsigned int currentSeed;
int cooksAvailable = Ncook;
int ovensAvailable = Noven;
int deliveryAvailable = Ndeliverer;
float timerSum = 0;
float timerMax = 0;
float timer2Sum = 0;
float timer2Max = 0;

//Mutexes
pthread_mutex_t cookLock;
pthread_mutex_t ovenLock;
pthread_mutex_t consoleLock;
pthread_mutex_t timerLock;
pthread_mutex_t timer2Lock;
pthread_mutex_t seedLock;
pthread_mutex_t deliveryLock;

//Mutex conditions
pthread_cond_t cooksLimitCondition;
pthread_cond_t ovensLimitCondition;
pthread_cond_t deliveryLimitCondition;

//functions
void *handleOrder(void *threadId);
unsigned int getRandomInt();
void reserveCook();
void freeCook();
void reserveOven();
void freeOven();
void lockConsole();
void freeConsole();
void lockTimer();
void freeTimer();
void lockTimer2();
void freeTimer2();
void reserveDelivery();
void freeDelivery();



//Main--------------------------------------------------------------------------------------------<
int main (int argc, char *argv[]) {
	
	//Get program arguments
	int rc1, rc2, rc3, rc4, rc5, rc6, rc7;
    if(argc != 3){printf("Error: Program takes 2 arguments"); return(0);}
    int Ncust = atoi(argv[1]);
    int seed = atoi(argv[2]); 
    if(Ncust <= 0 || seed <= 0) printf("Error: Arguments must be possitive numbers\n");
    currentSeed = seed;
    printf("Pizza Store Opened\n\n");
   
    //Initialize mutexes
    rc1 = pthread_mutex_init(&cookLock, NULL);
    rc2 = pthread_mutex_init(&ovenLock, NULL);
	rc3 = pthread_mutex_init(&consoleLock, NULL);
	rc4 = pthread_mutex_init(&timerLock, NULL);
	rc7 = pthread_mutex_init(&timer2Lock, NULL);
	rc5 = pthread_mutex_init(&seedLock, NULL);
	rc6 = pthread_mutex_init(&deliveryLock, NULL);
    if (rc1 != 0 || rc2 != 0 || rc3 != 0 || rc4 != 0 || rc5 != 0 || rc6 != 0 || rc7 != 0) {	
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc1);
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc2);
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc3);
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc4);
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc5);
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc6);
		printf("ERROR: return code from main.pthread_mutex_init() is %d\n", rc7);
		exit(-1);
    }
	
	//Initialize mutexe conditions
	rc1 = pthread_cond_init(&cooksLimitCondition, NULL);
	rc2 = pthread_cond_init(&ovensLimitCondition, NULL);
	rc3 = pthread_cond_init(&deliveryLimitCondition, NULL);
	if (rc1 != 0 || rc2 != 0 || rc3 != 0) {
    	printf("ERROR: return code from main.pthread_cond_init() is %d\n", rc1);
		printf("ERROR: return code from main.pthread_cond_init() is %d\n", rc2);
		printf("ERROR: return code from main.pthread_cond_init() is %d\n", rc3);
       	exit(-1);
	}
   
    //Initialize threads
    int i;
    pthread_t threads[Ncust];
    int threadIds[Ncust];
    for(i = 0; i < Ncust; i++) threadIds[i] = i + 1;
  
    //While there's a new order
    for(int i = 0; i < Ncust; i++){
		
		//Create a thread to handle the order
    	rc1 = pthread_create(&threads[i], NULL, handleOrder, &threadIds[i]);
		if (rc1 != 0) {
    		printf("ERROR: return code from pthread_create() is %d\n", rc1);
       		exit(-1);
       	}

		//Wait for new order
		sleep(getRandomInt(currentSeed, Torderlow, Torderhigh));
		currentSeed++;
    }
	
	//Join threads
	for (int i = 0; i < Ncust; i++) {
		rc1 = pthread_join(threads[i], NULL);
		
		if (rc1 != 0) {
			printf("ERROR: return code from main.pthread_join() is %d\n", rc1);
			exit(-1);		
		}
	}
	
	//Print avarage time of order delivery
	printf("\nAvarage delivery time: %.2f\n", timerSum / Ncust);
	
	//Print max time (time of the order that took longer to be delivered)
	printf("Maximun delivery time: %.2f\n", timerMax);
	
	//Print avarage time of order completion
	printf("\nAvarage time a ready order was cooling until pick up: %.2f\n", timer2Sum / Ncust);
	
	//Print max time (time of the order that took longer to complete)
	printf("Maximun time a ready order was cooling until pick up: %.2f\n", timer2Max);
	
    //Clean up
  	rc1 = pthread_mutex_destroy(&cookLock);
	rc2 = pthread_mutex_destroy(&ovenLock);
	rc3 = pthread_mutex_destroy(&consoleLock);
	rc4 = pthread_mutex_destroy(&timerLock);
	rc5 = pthread_mutex_destroy(&seedLock);
	rc6 = pthread_mutex_destroy(&deliveryLock);
	rc6 = pthread_mutex_destroy(&timer2Lock);
	if (rc1 != 0 || rc2 != 0 || rc3 != 0 || rc4 != 0 || rc5 != 0 || rc6 != 0 || rc7 != 0) {
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc1);
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc2);
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc3);
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc4);
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc5);
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc6);
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc7);
		exit(-1);		
	}
 	rc1 = pthread_cond_destroy(&cooksLimitCondition);
	rc2 = pthread_cond_destroy(&ovensLimitCondition);
	rc3 = pthread_cond_destroy(&deliveryLimitCondition);
	if (rc1 != 0 || rc2 != 0 || rc3 != 0) {
		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc1);
		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc2);
		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc3);
		exit(-1);		
	}
    printf("\nPizza Store Closed\n");
    pthread_exit(NULL);
  
}//End of main




//HandleOrder-------------------------------------------------------------------------------------<
void *handleOrder(void *threadId) {
	
    //Get thread arguments
    int *tid = (int *)threadId;
    unsigned int seed = currentSeed % *tid;
	
	//Start the timer 1
	struct timespec start, finish;
	clock_gettime(CLOCK_REALTIME, &start);
	
	//Get number of pizzas of the order
	int totalPizzas = getRandomInt(seed, Norderlow, Norderhigh);
	seed++;
    
	//Reserve an available cook
	reserveCook();
	
	//Prepare pizzas
	sleep(Tprep * totalPizzas);
	
	//Reserve an available oven
	reserveOven();
	
	//Free the cook
    freeCook();
	
	//Cook pizzas
	sleep(Tbake);
	
	//Start the timer 2
	struct timespec start2, finish2;
	clock_gettime(CLOCK_REALTIME, &start2);
	
	//Reserve an available delivery man
	reserveDelivery();
	
	//Free the oven
	freeOven();

	//Stop the timer 2
	clock_gettime(CLOCK_REALTIME, &finish2); 
	long seconds2 = finish2.tv_sec - start2.tv_sec; 
	if (start2.tv_nsec > finish2.tv_nsec) { //clock underflow 
		--seconds2; 
    } 
	
	//Wait for pizza to be delivered
	sleep(getRandomInt(seed, Tlow, Thigh));
	seed++;
	
	//Stop the timer 1
	clock_gettime(CLOCK_REALTIME, &finish); 
	long seconds = finish.tv_sec - start.tv_sec; 
	if (start.tv_nsec > finish.tv_nsec) { //clock underflow 
		--seconds; 
    } 
	
	//Wait for delivery man to return to the store
	sleep(getRandomInt(seed, Tlow, Thigh));
	seed++;
	
	//Free the delivery man
	freeDelivery();
	
	//Print order time in seconds
	lockConsole();
	printf("Order %d has been delivered in %ld minutes and was cooling for %ld\n", *tid, seconds, seconds2);
	freeConsole();
	
	//Add order delivery time to the sum
	lockTimer();
	timerSum += seconds;
	freeTimer();
	
	//Add order cooling time to the sum 2
	lockTimer2();
	timer2Sum += seconds2;
	freeTimer2();
	
	//Check if max delivery time
	if(seconds > timerMax){
		//Set the max time
		lockTimer();
		timerMax = seconds;
		freeTimer();
	}
	
	//Check if max waiting for pick up time
	if(seconds2 > timer2Max){
		//Set the max time
		lockTimer2();
		timer2Max = seconds2;
		freeTimer2();
	}
	
    //Terminate this thread
    pthread_exit(NULL);
}//End of order



//Other functions---------------------------------------------------------------------------------<

//Returns a random int (in the range of lower to upper bound) based on the current seed.
unsigned int getRandomInt(unsigned int seed, int lowerBound, int upperBound){

    //Lock seed mutex 
	int rc = pthread_mutex_lock(&seedLock);
	if (rc != 0) {	
		printf("ERROR: return code from getRandomInt.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}

	//Get random number
	int d = rand_r(&seed) % upperBound + lowerBound;
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&seedLock);
	if (rc != 0) {	
		printf("ERROR: return code from getRandomInt.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	

	//Bound number
	if(d < lowerBound) d = lowerBound;
	else if(d > upperBound) d = upperBound;

	return d;
}

/* Reserve an available cook
 * If there is no cook available, wait until one becomes available.
 */
void reserveCook(){
	
	//Lock cooks mutex 
	int rc = pthread_mutex_lock(&cookLock);
	if (rc != 0) {	
		printf("ERROR: return code from reserveCook.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	/* If no cook is available, wait until one becames available
	 * (We use while condition to avoid spurious wakeups) 
	 */
	while(cooksAvailable == 0){
		pthread_cond_wait(&cooksLimitCondition, &cookLock);
	}
	
	//reserve a cook
	--cooksAvailable;
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&cookLock);
	if (rc != 0) {	
		printf("ERROR: return code from reserveCook.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}

//Frees a cook previeusly reserved by reserveCook()
void freeCook(){
	
	//Lock cooks mutex 
	int rc = pthread_mutex_lock(&cookLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeCook.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	//free cook
	++cooksAvailable;
	
	//Notify other threads that this cook is now available
	pthread_cond_signal(&cooksLimitCondition);
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&cookLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeCook.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}

/* Reserve an available oven
 * If there is no oven available, wait until one becomes available.
 */
void reserveOven(){
	
	//Lock ovens mutex 
	int rc = pthread_mutex_lock(&ovenLock);
	if (rc != 0) {	
		printf("ERROR: return code from reserveOven.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	/* If no oven is available, wait until one becames available
	 * (We use while condition to avoid spurious wakeups) 
	 */
	while(ovensAvailable == 0){
		pthread_cond_wait(&ovensLimitCondition, &ovenLock);
	}
	
	//reserve an oven
	--ovensAvailable;
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&ovenLock);
	if (rc != 0) {	
		printf("ERROR: return code from reserveOven.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}

//Free an oven previusly reserved by reserveOven()
void freeOven(){
	
	//Lock ovens mutex 
	int rc = pthread_mutex_lock(&ovenLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeOven.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	//free oven
	++ovensAvailable;
	
	//Notify other threads that this oven is now available
	pthread_cond_signal(&ovensLimitCondition);
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&ovenLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeOven.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}

//Locks console for printing
void lockConsole(){
	
	//Lock console mutex 
	int rc = pthread_mutex_lock(&consoleLock);
	if (rc != 0) {	
		printf("ERROR: return code from lockConsole.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
}

//Unlocks console for printing
void freeConsole(){
	
	//Unlock console mutex
	int rc = pthread_mutex_unlock(&consoleLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeConsole.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}

//Locks timer for calculations
void lockTimer(){
	
	//Lock timer mutex 
	int rc = pthread_mutex_lock(&timerLock);
	if (rc != 0) {	
		printf("ERROR: return code from lockTimer.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
}

//Unlocks timer
void freeTimer(){
	
	//Unlock timer mutex
	int rc = pthread_mutex_unlock(&timerLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeTimer.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
}

//Locks timer 2 for calculations
void lockTimer2(){
	
	//Lock timer mutex 
	int rc = pthread_mutex_lock(&timer2Lock);
	if (rc != 0) {	
		printf("ERROR: return code from lockTimer2.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
}

//Unlocks timer 2
void freeTimer2(){
	
	//Unlock timer 2 mutex
	int rc = pthread_mutex_unlock(&timer2Lock);
	if (rc != 0) {	
		printf("ERROR: return code from freeTimer2.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
}

/* Reserve an available delivery man
 * If there is no delivery man available, wait until one becomes available.
 */
void reserveDelivery(){
	
	//Lock delivery mutex 
	int rc = pthread_mutex_lock(&deliveryLock);
	if (rc != 0) {	
		printf("ERROR: return code from reserveDelivery.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	/* If no delivery man is available, wait until one becames available
	 * (We use while condition to avoid spurious wakeups) 
	 */
	while(deliveryAvailable == 0){
		pthread_cond_wait(&deliveryLimitCondition, &deliveryLock);
	}
	
	//reserve a delivery man
	--deliveryAvailable;
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&deliveryLock);
	if (rc != 0) {	
		printf("ERROR: return code from reserveDelivery.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}

//Frees a delivery man previeusly reserved by reserveDelivery()
void freeDelivery(){
	
	//Lock delivery mutex 
	int rc = pthread_mutex_lock(&deliveryLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeDelivery.pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	//free delivery man
	++deliveryAvailable;
	
	//Notify other threads that this delivery man is now available
	pthread_cond_signal(&deliveryLimitCondition);
	
	//Unlock mutex
	rc = pthread_mutex_unlock(&deliveryLock);
	if (rc != 0) {	
		printf("ERROR: return code from freeDelivery.pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}	
}