
#include "math.h"

volatile float float_test_b( float A, float M )
{
float B;
B = sqrtf( A );			// disaster with -Og, -Os : __ieee754_sqrtf
B *= M;
return B;
}
