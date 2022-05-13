#include <stdio.h>

/**
	Programma pou typwnei ta megethi twn basikwn typwn (p.x. int) se bits. To programma exei kapoia lathi. 
	Tip: H sizeof epistrefei to plithos se bytes.
**/
int main(void) { //isodynamo tou "main(int argc, char** argv)" an den xreiazomaste parametrous
    printf("#bits in a char = %u \n", sizeof(char) * 8); // x 8 gia na einai to apotelesma se bits
    printf("#bits in a short = %u \n", sizeof(short) * 8);
    printf("#bits in a int = %u \n", sizeof(int) * 8);
    printf("#bits in a long = %u \n", sizeof(long) * 8);
    printf("#bits in a float = %u \n", sizeof(float) * 8);
    printf("#bits in a double = %u \n", sizeof(double) * 8);
    printf("#bits in a long double = %u \n", sizeof(long double) * 8);
    return 0;
}
