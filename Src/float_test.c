
#include "math.h"

/* float -> double -> double -> float : personnene veut cela *
volatile float float_test_a( float A, float M )
{
float B;
B = sqrt( A );			// big disaster : conversion en double puis __ieee754_sqrt
B *= M;
return B;
}
//*/

volatile float float_test_b( float A, float M )
{
float B;
B = sqrtf( A );			// disaster with -Og, -Os : __ieee754_sqrtf
B *= M;
return B;
}

/* experience superflue : __builtin_sqrtf() est alias de sqrtf() *
volatile float float_test_c( float A, float M )
{
float B;
B = __builtin_sqrtf( A );	// disaster with -Og, -Os : __ieee754_sqrtf
B *= M;
return B;
}
//*/
