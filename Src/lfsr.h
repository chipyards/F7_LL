/*
A 2-tap (simplest) lfsr, many-to-one style

taps are numbered from LSB of reg
i.e. tap 0 is LSB, which is the output of the first flip-fl0p
example : 
1 + v^3  + v^20 est un polynome primitif, il est suppose donner une sequence maximale et unique
ses taps sont ceux d'indice 2 et 19, la longueur de la sequence est 2^20 - 1
(une condition necessaire mais non sufisante est que le nombre de taps soit pair et qu'il n'existe pas
 de diviseur commun a tous les indices autre que 1)<== il y a doute!!!

Catalogue de polynomes primitifs
1 + v   + v2
1 + v   + v3
1 + v   + v4
1 + v2  + v5
1 + v   + v6
1 + v   + v7
1 + v   + v5   + v6  + v8
1 + v4  + v9
1 + v3  + v10
1 + v2  + v11
1 + v3  + v4   + v7  + v12
1 + v   + v3   + v4  + v13
1 + v   + v11  + v12 + v14
1 + v   + v15
1 + v2  + v3   + v5  + v16
1 + v3  + v17
1 + v7  + v18
1 + v   + v1   + v6  + v19
1 + v3  + v20
1 + v2  + v21
1 + v   + v22
1 + v5  + v23
1 + v   + v3   + v4  + v24
1 + v3  + v25
1 + v   + v7   + v8  + v26
1 + v   + v7   + v8  + v27
1 + v3  + v28
1 + v2  + v29
1 + v   + v15  + v16 + v30
1 + v3  + v31
1 + v   + v27  + v28 + v32
1 + v13 + v33
1 + v   + v14  + v15 + v34
1 + v2  + v35
1 + v11 + v36
1 + v2  + v10  + v12 + v37
1 + v   + v5   + v6  + v38
1 + v4  + v39
*/

class lfsr {
public:
unsigned int reg;
unsigned int tap1;
unsigned int tap2;
// constructor
lfsr( unsigned int t1 = 2, unsigned int t2 = 19 ) {
	tap1 = t1; tap2 = t2;
	reg = 1;
	};
// methods
void seed( unsigned int s ) {
	reg = s;
	};
unsigned int step() {
	unsigned int fb;			// feedback bit
	fb =  ( reg >> tap1 );	// 0nly LSB 0f fb is significant
	fb ^= ( reg >> tap2 );
	reg <<= 1;		// 0 is injected as LSB
	reg |= ( fb & 1 );	// fb is injected
	return reg;
	};
unsigned int get_mask() {
	return ( 1 << ( tap2 + 1 ) ) - 1;
	}
unsigned int eval() {
	unsigned int cnt = 0;
	unsigned int m = get_mask();
	reg = 1;
	do	{
		step(); cnt++;
		// printf("%08x\n", reg ); if ( cnt > 200 ) break;
		} while ( ( reg & m ) > 1 );
	return cnt;
	};
};
