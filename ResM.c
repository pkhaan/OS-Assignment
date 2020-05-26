/* Operating Systems - 1st Assignment 
 * Jason Christofilakis - AM 3170182
 * Petros Chanas - AM 3170173
 * Damianos Iakovidis - AM 3170051
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "p3170182-p3170173-p3170051-res1.h"




/*
 * Ekswterikes metavlites pou vlepoun ola ta threads.
*/
int Ncust; //plithos pelatwn
int randSeed; //sporos dosmenos apo ton xristi
int deposit; //trapeziko upoloipo ths etaireias
int exchangeCnt; //aukswn arithmos sunallagis
int availableMachines; //plithos diathesimwn thlefwnitwn
double avgWaitTime; //mesos xronos anamonis pelatwn
double avgServeTime; //mesos xronos eksupiretisis pelatwn
int seatPlan[Nseat]; // to plano tou theatrou
int availableSeats; // plithos diathesimwn thesewn
int anamonhSum; // sunolikh anamonh gia touw pelates

/*
 * To mutex pou tha xrisimopoiithei gia tin prostasia tis koinis metavlitis
 * availableMachines.
*/
pthread_mutex_t availableMachinesMutex;
/*
 * To mutex pou tha xrisimopoiithei gia to kleidwma tou termatikou,
 * wste na eksasfalizetai oti mono ena nhma th fora grafei sthn othoni.
*/
pthread_mutex_t terminalMutex;
/*
 * To mutex pou tha xrisimopoiithei gia to kleidwma tou planou thesewn,
 * kata ti desmeusi kai apodesmeusi thesewn apo tous thelfwnites.
*/
pthread_mutex_t seatsMutex;
/*
 * Mutexes pou tha xrisimopoiithoun gia tin prostasia twn parapanw koinwn metavlitwn.
*/
pthread_mutex_t depositMutex, exchangeCntMutex, avgWaitTimeMutex, avgServeTimeMutex;
/*
 * Condition variable pou einai 'true' an uparxoun diathesimoi tilefwnites.
*/
pthread_cond_t availableCond;






/**
 * Sunartisi pou ektelei to thlefwnhma sto theatro gia kathe pelati
*/
void *makeCall(void *threadId) {
	
	// to id tou trexontos pelati
	int *id = (int *)threadId;


	int rc;
	
	
	printf("O pelatis %d kalei\n", *id);

	// ksekina na metras to xrono anamonis
	struct timespec anamonhStart, anamonhFinish; 
	clock_gettime(CLOCK_REALTIME, &anamonhStart);

	// mutex lock --- availableMachinesMutex --- enimerwsi diathesimwn tilefwnitwn
	rc = pthread_mutex_lock(&availableMachinesMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	

	while (availableMachines == 0) {
		printf("O pelatis %d den brike diathesimo tilefoniti. Perimenei...\n", *id);		
		rc = pthread_cond_wait(&availableCond, &availableMachinesMutex);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
			pthread_exit(&rc);
		}
	}

	
	printf("O pelatis %d eksipireteitai.\n", *id);


	// mutex lock --- avgWaitTimeMutex --- enimerwse to wait average
	rc = pthread_mutex_lock(&avgWaitTimeMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	clock_gettime(CLOCK_REALTIME, &anamonhFinish);
	avgWaitTime += (anamonhFinish.tv_sec - anamonhStart.tv_sec)/(float)Ncust;

	// mutex unlock --- avgWaitTimeMutex --- enimerwse to wait average
	rc = pthread_mutex_unlock(&avgWaitTimeMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}

	
	
	availableMachines--;	// meiwse tous diathesimous tilefwnites


	// mutex unlock --- availableMachinesMutex --- enimerwsi diathesimwn tilefwnitwn
	rc = pthread_mutex_unlock(&availableMachinesMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}


	//-------------kane kratisi me ton tilefoniti pou desmeuses-------------

	int seatsWanted = rand_r(&randSeed) % Nseathigh + Nseatlow;
	int secondsNeeded = rand_r(&randSeed) % tseathigh + tseatlow;

	sleep(secondsNeeded);

	
	

	
	if (availableSeats == 0) { // mataiwsi kratisis
		printf("Pelatis %d: H kratisi mataiwthike giati to theatro einai gemato.\n", *id);
	} else if (availableSeats < seatsWanted) { // mataiwsi kratisis
		printf("Pelatis %d: H kratisi mataiwthike giati den uparxoun arketes diathesimes theseis.\n", *id);
	} else { // sunexise me desmeusi thesewn

		// mutex lock --- seatsMutex --- demeusi thesewn
		rc = pthread_mutex_lock(&seatsMutex);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		

		// desmeuse theseis
		int seatCnt = 0;
		for (int i = 0; i<Nseat && seatCnt<seatsWanted; i++) {
			if (seatPlan[i]==-1) {
				seatPlan[i] = *id;
				seatCnt++;
			}		
		}

		// meiwse arithmo diathesimwn thesewn
		availableSeats-=seatsWanted;
		
		// mutex unlock --- seatsMutex --- demeusi thesewn
		rc = pthread_mutex_unlock(&seatsMutex);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		
		
		int cardSuccess;
		float randVal = (rand_r(&randSeed) % 100)/100.0;
		
		if  (randVal < Pcardsuccess) {
			cardSuccess = 1;
		}
		else {
			cardSuccess = 0;
		}
		

		if (cardSuccess) { // sunexise me xrewsh tou pelati

			// mutex lock --- depositMutex --- xrewsi logariasmou
			rc = pthread_mutex_lock(&depositMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(&rc);
			}

			
			// eisprakse
			deposit += seatsWanted*Cseat;
			
			
			// mutex unlock --- depositMutex --- xrewsi logariasmou
			rc = pthread_mutex_unlock(&depositMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(&rc);
			}


			

			// mutex lock --- exchangeCntMutex --- dwse aukswnta arithmo sunallagis
			rc = pthread_mutex_lock(&exchangeCntMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(&rc);
			}
			


			// dwse arithmo sunallagis
			exchangeCnt++;
			
				
			// mutex unlock --- exchangeCntMutex --- dwse aukswnta arithmo sunallagis
			rc = pthread_mutex_unlock(&exchangeCntMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(&rc);
			}


			// mhnuma epitixumenhs krathshs

			// mutex lock --- terminalMutex --- desmeuse to termatiko
			rc = pthread_mutex_lock(&terminalMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(&rc);
			}


			printf("Pelatis %d: H kratisi oloklirwthike epitixws. O arithmos sunallagis einai %d, oi theseis sas einai oi ", *id, exchangeCnt);
			for (int i = 0; i<Nseat; i++) {
				if (seatPlan[i]==*id) {
					printf("%d, ", i);
				}		
			}
			printf("kai to kostos tis sunallagis einai %d euro.\n", seatsWanted*Cseat);


			// mutex unlock --- terminalMutex --- desmeuse to termatiko
			rc = pthread_mutex_unlock(&terminalMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(&rc);
			}
				


		}
		else { // mataiwsi sunallagis
			printf("Pelatis %d: H kratisi mataiwthike giati h sunallagh me tin pistwtiki karta den egine apodekti.\n", *id);
			
			// apodesmeuse desmeumenes theseis
			
			// mutex lock --- seatsMutex --- apodemeusi thesewn
			rc = pthread_mutex_lock(&seatsMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
				pthread_exit(&rc);
			}


			// apodesmeuse desmeumenes theseis
			for (int i = 0; i<Nseat; i++) {
				if (seatPlan[i]==*id) {
					seatPlan[i] = -1;
				}		
			}

			// auksise arithmo diathesimwn thesewn
			availableSeats+=seatsWanted;
			
			

			// mutex unlock --- seatsMutex --- apodemeusi thesewn
			rc = pthread_mutex_unlock(&seatsMutex);
			if (rc != 0) {	
				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
				pthread_exit(&rc);
			}		
		}



	}
	


	//--------------telos douleias me ton tilefwniti--------------



	// mutex lock --- availableMachinesMutex --- enimerwsi diathesimwn tilefwnitwn
	rc = pthread_mutex_lock(&availableMachinesMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	

	printf("O pelatis %d eksipiretithike! \n", *id);


	// mutex lock --- avgServeTimeMutex --- enimerwse to wait average
	rc = pthread_mutex_lock(&avgServeTimeMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	clock_gettime(CLOCK_REALTIME, &anamonhFinish);
	avgServeTime += (anamonhFinish.tv_sec - anamonhStart.tv_sec)/(float)Ncust;

	// mutex unlock --- avgWaitTimeMutex --- enimerwse to wait average
	rc = pthread_mutex_unlock(&avgServeTimeMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}


	availableMachines++;
	rc = pthread_cond_broadcast(&availableCond);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_cond_broadcast() is %d\n", rc);
		pthread_exit(&rc);
	}

	// mutex unlock --- availableMachinesMutex --- enimerwsi diathesimwn tilefwnitwn
	rc = pthread_mutex_unlock(&availableMachinesMutex);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	


	
	pthread_exit(threadId);
}




/**
 * Sunartisi main
*/
int main(int argc, char *argv[]) {

	if (argc != 3) {
		printf("ERROR: the program should take two arguments, the number of customers and a seed for the random generator!\n");
		exit(-1);
	}

	Ncust = atoi(argv[1]);

	/**
	 * elegxoume oti to plithos twn customers pou edwse o xristis einai thetikos arithmos.
	*/
	if (Ncust < 0) {
		printf("ERROR: the number of customers should be a positive number. Current number given %d.\n", Ncust);
		exit(-1);
	}
	/**
	 * anathesi tou seed pou edwse o xristis sthn antistoixi metavliti.
	*/
	randSeed = atoi(argv[2]);


	printf("Main: We will create %d threads, one for each customer.\n", Ncust);
	

	int rc;
	pthread_t *threads = malloc(Ncust * sizeof(pthread_t));
	int *threadIds = malloc(Ncust * sizeof(int));
	// arxikopoiisi twn metavlitwn deposit, exchangeCount, availableMachines, avgWaitTime, avgServeTime
	deposit = 0;
	exchangeCnt = 0;
	availableMachines = Ntel;
	avgWaitTime = 0;
	avgServeTime = 0;
	availableSeats = Nseat;
	for (int i = 0; i<Nseat; i++) {
		seatPlan[i]=-1;
	}


	// arxikopoiisi twn mutex
	rc = pthread_mutex_init(&availableMachinesMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc = pthread_mutex_init(&terminalMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc = pthread_mutex_init(&seatsMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc = pthread_mutex_init(&depositMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc = pthread_mutex_init(&exchangeCntMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc = pthread_mutex_init(&avgWaitTimeMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	rc = pthread_mutex_init(&avgServeTimeMutex, NULL);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
		exit(-1);
	}
	// arxikopoiisi condition
	rc = pthread_cond_init(&availableCond, NULL);
	if (rc != 0) {
    		printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
       		exit(-1);
	}
	

	int i = 0;
	for (i = 0; i < Ncust; i++) {
		threadIds[i] = i + 1;
		printf("Main: creating thread %d\n", threadIds[i]);
		
		/*dimiourgia tou thread*/
    		rc = pthread_create(&threads[i], NULL, makeCall, &threadIds[i]);

		/*elegxos oti to thread dimiourgithike swsta.*/
    		if (rc != 0) {
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
	}
 
	void *status;
	for (i = 0; i < Ncust; i++) {
		rc = pthread_join(threads[i], &status);
		
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}

		printf("Main: Thread %d finished with status %d.\n", threadIds[i], *(int *)status);
	}



	// ektypwsi twn apotelesmatwn
  	printf ("Main(): Waited for %d threads to finish.\n", Ncust);
	printf("Teliko plano thesewn:\n");
	for (int i = 0; i<Nseat; i++) {
		if (seatPlan[i]!=-1) {
			printf("Thesi %d / Pelatis %d, ", i, seatPlan[i]);
		} else {
			printf("Thesi %d / Keni, ", i);
		}	
	}
	printf("\n");
	printf("Sunolika esoda = %d.\n", deposit);
	printf("Mesos xronos anamonis pelatwn = %f.\n", avgWaitTime);
	printf("Mesos xronos eksupiretisis pelatwn = %f.\n", avgServeTime);



	// "katastrofi" twn mutex
	rc = pthread_mutex_destroy(&availableMachinesMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	rc = pthread_mutex_destroy(&terminalMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	rc = pthread_mutex_destroy(&seatsMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	rc = pthread_mutex_destroy(&depositMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	rc = pthread_mutex_destroy(&exchangeCntMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	rc = pthread_mutex_destroy(&avgWaitTimeMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	rc = pthread_mutex_destroy(&avgServeTimeMutex);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}
	// "katastrofi" condition
	rc = pthread_cond_destroy(&availableCond);
	if (rc != 0) {
   		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
      		exit(-1);
	}


	// apeleutherwsi desmeumenis mnimis
	free(threads);
	free(threadIds);
	
	return 1;



}
