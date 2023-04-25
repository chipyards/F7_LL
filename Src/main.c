/**
  */

#include "stm32f7xx.h"
// #include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_pwr.h"
#include "math.h"
#include "options.h"
#include "s_gpio.h"
#include "uarts.h"

// declaration
void SystemCoreClockUpdate(void);

// variables
volatile unsigned int ticks = 0;
volatile unsigned int lookatme;		// pour debug

/* Application functions ---------------------------------------------------------*/
#include "float_test.h"

void FPU_status()
{
CDC_print("SCB->CPACR = %08X\n",  SCB->CPACR );
}

/* System functions ---------------------------------------------------------*/

// systick interrupt handler
void SysTick_Handler()
{
++ticks;
#ifdef PROFILER_PI2
profile_D8( ticks & 1 );
#endif
}

void HSE_start(void)
{
LL_RCC_HSE_DisableBypass();
LL_RCC_HSE_Enable();
while	(!LL_RCC_HSE_IsReady())
	{}
// N.B. SYSCLK c'est l'horloge dont sont derivees celles des bus AHB, APB1, APB2
// HCLK c'est l'horloge du bus AHB
LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
}

//LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
//LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
//LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);


// config systick avec interrupt
void Systick_init( unsigned int periode_en_ticks )
{
// periode
SysTick->LOAD  = periode_en_ticks - 1;
// priorite
NVIC_SetPriority( SysTick_IRQn, 7 );
// init counter
SysTick->VAL = 0;
// prescale (0 ===> %8)
SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
// enable timer, enable interrupt
SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief  Main program
  */
int main(void)
{
lookatme = SystemCoreClock;
SystemCoreClockUpdate();	// 16 MHz HSI 1% precision
lookatme = SystemCoreClock;
GPIO_config_LCD_BL();	// eteindre le backlight qui est on au reset (pullup)
#ifdef PROFILER_PI2
GPIO_config_profiler_PI1_PI2();
#endif
HSE_start();			// 25 MHz Xtal precision
SystemCoreClockUpdate();
lookatme = SystemCoreClock;

// config systick @ 100Hz
Systick_init( SystemCoreClock / 100 );
#ifdef USE_UART1
CDC_init();
CDC_print("Hello World %u Hz\n", lookatme );
#endif

/* Infinite loop */
while	(1)
	{
	lookatme += 1;
	#ifdef USE_UART1
	int c;
	if	( ( c = CDC_getcmd() ) > 0 )
		{
		switch	(c)
			{
			case '0' : profile_D13( 0 ); break;
			case '1' : profile_D13( 1 ); break;
			case 'L' : LCD_BL( 1 ); break;
			case 'D' : LCD_BL( 0 ); break;
			case 'f' : FPU_status(); break;
//			case 'a' : {	float F; F = float_test_a( 200.0f, 10000.0f );
//					CDC_print("F = %d\n", (int)F );	} break;
			case 'b' : {	float F; F = float_test_b( 200.0f, 10000.0f );
					CDC_print("F = %d\n", (int)F );	} break;
//			case 'c' : {	float F; F = float_test_c( 200.0f, 10000.0f );
//					CDC_print("F = %d\n", (int)F );	} break;
			default  : CDC_print("cmd '%c' %u\n", c, lookatme );
			}
		}
	#endif
	#ifdef GREEN_CPU
	/* Clear SLEEPDEEP bit of Cortex System Control Register */
  	CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
	/* Ensure that all instructions done before entering SLEEP mode */
	__DSB();
	__ISB();
	// Wait For Interrupt
	__WFI();
	#endif
	}
}

