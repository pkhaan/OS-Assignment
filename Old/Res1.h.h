#ifndef _MYEXT_H
#define _MYEXT_H

/* Constants declaration */

#define Nseat  250
#define Ntel 8
#define Nseatlow 1
#define Nseathigh 5
#define tseatlow 5
#define tseathigh 10
#define Pcardsuccess 0.9
#define Cseat 20


/**
 * Sunartisi pou ektelei to thlefwnhma sto theatro gia kathe pelati
*/
void *makeCall(void *threadId);

/**
 * Sunartisi main
*/
int main(int argc, char *argv[]);

#endif
