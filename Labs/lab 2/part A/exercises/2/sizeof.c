#include <stdio.h>

/**
	Na ftiaksete ena programma pou typwnei ta megethi twn basikwn typwn (p.x. int) se bits. 
	To programma exei kapoia lathi. 
	Tip: H sizeof epistrefei to plithos se bytes.
**/
int main() { //isodynamo tou "main(int argc, char** argv)" dioti den xrisimopoiei parametrous
    printf("#bits in a char = %u\n", sizeof(char)); 
    printf("#bits in a short = %u\n", sizeof(short));
    printf("#bits in a int = %u\n", sizeof(int));
    printf("#bits in a long = %u\n", sizeof(long));
    printf("#bits in a float = %u\n", sizeof(float));
    printf("#bits in a double = %u\n", sizeof(double));
    printf("#bits in a long double = %u\n", sizeof(long double));
    return 0;
}
