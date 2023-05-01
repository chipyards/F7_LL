/*
g++ -o demo demo.cpp ../Src/float_test.cpp -Wall -O0
*/
#include <stdio.h>
#include <stdlib.h>
#include "../Src/float_test.h"


int main( int argc, char ** argv )
{
unsigned cnt = 100;
if	( argc >= 2 )
	cnt = atoi(argv[1]);
// float buffer
float * A = (float *)malloc( cnt * sizeof(float) );
if	( A == NULL )
	exit(1);
// random numbers
unsigned int cycle = generate( A, cnt );
printf("%u : %d, %d, %d ... %d\n", cycle, int(A[1]*1000.0), int(A[2]*1000.0), int(A[3]*1000.0), int(A[cnt-1]*1000.0) );
float resu[5]; void * vresu; int * iresu;
// bench
macbench( cnt, A, resu );
// display
vresu = (void *)resu;
iresu = (int *)vresu;
printf("%08x, %08x, %08x, %08x, %08x, %d, %d, %d, %d, %d\n",
	iresu[0], iresu[1], iresu[2], iresu[3], iresu[4], int(resu[0]), int(resu[1]), int(resu[2]), int(resu[3]), int(resu[4]) );

return 0;
}
