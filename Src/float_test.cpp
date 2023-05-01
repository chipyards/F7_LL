
// #include "math.h"
#include "float_test.h"
#include "lfsr.h"

float build_float( int sgn, int exp, unsigned int mantissa )
{
unsigned int resu = sgn & 1;
void * ptr;	// seems to avoid "dereferencing type-punned pointer will break strict-aliasing rules"
float fresu;
// 8-bit exponent
resu <<= 8;
resu |= ( exp + 127 ) & 0xFF;
// 23-bit mantissa
resu <<= 23;
resu |= ( mantissa & ( ( 1 << 23 ) - 1 ) );
ptr = (void *)&resu;
fresu = *((float *)ptr);
return fresu;
}

// float random numbers generator (malloc outside)
// returns the cycle length
unsigned int generate( float * fsrc, unsigned int cnt )
{
// 23-bit random numbers generator
lfsr randozer = lfsr( 2, 24 );
// printf("randozer: mask=%u, check cnt=%u\n", randozer.get_mask(), randozer.eval() );
// random int to float generator
int sgn, exp;
unsigned int mantissa;
for	( unsigned int i = 0; i < cnt; i++ )
	{
	exp = randozer.step();
	sgn = exp & 1;
	exp >>= 1;
	exp &= 15;
	exp -= 7;
	mantissa = randozer.step() & 0x7FFFFF;
	fsrc[i] = build_float( sgn, exp, mantissa );
	// printf("%.8f\n", fsrc[i] );
	}
return 0;	// randozer.eval();
}

// MAC benchmark, returns 5 floats in resu(]
// cette fonction effectue (cnt-4)*5 mult-accs, avec autant d'acces a un array massif
void macbench( unsigned int cnt, float * src, float * resu )
{
float s0 = 0.0, s1 = 0.0, s2 = 0.0, s3 = 0.0, s4 = 0.0;
for	( unsigned int i = 0; i < ( cnt - 4 ); i++ )
	{
	s0 += src[i] * src[i];
	s1 += src[i] * src[i+1];
	s2 += src[i] * src[i+2];
	s3 += src[i] * src[i+3];
	s4 += src[i] * src[i+4];
	}
resu[0] = s0;
resu[1] = s1;
resu[2] = s2;
resu[3] = s3;
resu[4] = s4;
}

