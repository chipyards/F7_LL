/**
  */

#include "stm32f7xx.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_gpio.h"
#include "math.h"
#include <stdio.h>
#include "options.h"
#include "s_gpio.h"
#include "uarts.h"

// declaration
void SystemCoreClockUpdate(void);
void SystemClock_Config_PLL( int HSE_flag, int Mdiv, int Nmul );
static void CPU_CACHE_Enable(void);

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
extern "C" void SysTick_Handler()
{
++ticks;
switch	( ticks % 100 )
	{
	// LED blink
	case 0  : profile_D13( 1 ); break;
	case 5  : profile_D13( 0 ); break;
	/* 1s pulse for clock check *
	case 10 : profile_D8_lo(); break;
	case 20 : profile_D8_hi(); break;
	//*/
	}
}

void HSE_start(void)
{
LL_RCC_HSE_DisableBypass();
LL_RCC_HSE_Enable();
while	(!LL_RCC_HSE_IsReady())
	{}
LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
}

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
CPU_CACHE_Enable();

SystemCoreClockUpdate();	// 16 MHz HSI 1% precision

GPIO_config();
GPIO_config_LCD_BL();	// eteindre le backlight qui est on au reset (pullup)
#ifdef PROFILER_PI2
GPIO_config_profiler_PI1_PI2();
#endif

#ifdef OPT_HSE
HSE_start();			// 25 MHz Xtal precision
SystemCoreClockUpdate();
#ifdef OPT_PLL
SystemClock_Config_PLL( 1, LL_RCC_PLLM_DIV_25, FRE_PLL * 2 );
SystemCoreClockUpdate();
#endif
#else				// 16 MHz internal osc.
#ifdef OPT_PLL
SystemClock_Config_PLL( 0, LL_RCC_PLLM_DIV_16, FRE_PLL * 2 );
SystemCoreClockUpdate();
#endif
#endif

// config systick @ 100Hz
Systick_init( SystemCoreClock / 100 );
#ifdef USE_UART1
CDC_init();
CDC_print("Hello World %u Hz\n", lookatme );
#endif

/* Infinite loop */
while	(1)
	{
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
			case 'a' : {	float F; F = float_test_b( 200.0f, 10000.0f );
					CDC_print("F = %d\n", (int)F );	} break;
			case 'B' : CDC_print("baud div = %u\n", (unsigned int)USART1->BRR ); break;
			default  : CDC_print("cmd '%c' %u\n", c, SystemCoreClock );
			}
		}
	#endif
	if	( IS_BLUE_PRESSED() )	// ANTI BRICK GREEN CPU
		profile_D13( 1 );
	else	{
		#ifdef GREEN_CPU
		profile_D8_tog();
		/* Clear SLEEPDEEP bit of Cortex System Control Register */
  		CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
		/* Ensure that all instructions done before entering SLEEP mode */
		__DSB();
		__ISB();
		// Wait For Interrupt
		__WFI();
		// profile_D8_lo();
		#endif
		}
	}
}

// N.B. SYSCLK c'est l'horloge dont sont derivees celles des bus AHB, APB1, APB2
// HCLK c'est l'horloge du bus AHB
/**
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI ou HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSI or HSE Frequency(Hz)       = selon Mdiv (cf system_stm32f7xx.c)
  *            PLL_M                          = Mdiv, LL_RCC_PLLM_DIV_16 ou LL_RCC_PLLM_DIV_25
  *            PLL_N                          = Nmul, 400 ou 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9 (pas utilise, pour 48MHz domain))
  *            PLL_R                          = 7 (pas utilise, pour I2S)
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6 ou 7 selon Nmul
  */
void SystemClock_Config_PLL( int HSE_flag, int Mdiv, int Nmul )
{
  /* Set FLASH latency = 6 de 180 a 210 MHz, 7 au dela )*/
  LL_FLASH_SetLatency( ( Nmul > 420 )?(LL_FLASH_LATENCY_7):(LL_FLASH_LATENCY_7) );

  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Over Drive for the voltage regulator
   * on se demande si cela active le scaling 1
   * Autre hypothese :
   *	These bits (VOS[1:0] can be modified only when the PLL is OFF.
   *	The new value programmed is active only when the PLL is ON.
   *	When the PLL is OFF, the voltage scale 3 is automatically selected.
   * La valeur par defaut c'est scale 1, donc il suffirait de demarrer la PLL
   * pour passer en 1.
   * scaling 2 est requis pour HCLK > 144 MHz
   * scaling 1 est requis pour HCLK > 168 MHz,
   * over drive pour HCLK > 180 MHz
   */
  LL_PWR_EnableOverDriveMode();
  while	(LL_PWR_IsActiveFlag_OD() != 1) { };

  /* Activation OverDrive Switching */
  LL_PWR_EnableOverDriveSwitching();
  while	(LL_PWR_IsActiveFlag_ODSW() != 1)  { };

  /* Main PLL configuration and activation */
  // LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_16, 432, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_ConfigDomain_SYS( ( (HSE_flag)?(LL_RCC_PLLSOURCE_HSE):(LL_RCC_PLLSOURCE_HSI) ),
  				Mdiv, Nmul, LL_RCC_PLLP_DIV_2 );
  LL_RCC_PLL_Enable();
  while	(LL_RCC_PLL_IsReady() != 1) { };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while	(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) { };

  /* Set APB1 & APB2 prescaler */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  // SystemCoreClock = 216000000;
}


/**
  * @brief  CPU L1-Cache enable.
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();
  /* Enable D-Cache */
  SCB_EnableDCache();
}

