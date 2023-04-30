// #define	USE_FULL_LL_DRIVER // c'est dans .cproject


#define GREEN_CPU	// sleep dans main loop
//#define OPT_HSE		// 25 MHz Xtal
//#define OPT_PLL
#define FRE_PLL		200


/** modules optionnels **/
#define USE_UART1		// CDC vers PC via ST-Link
// #define USE_UART6		// CN4.D1 = PC6 = UART6 TX, CN4.D0 = PC7 = UART6 RX

/** modules utilitaires **/
#define PROFILER_PI2	// pins PI1 aka D13, PI2 aka D8

