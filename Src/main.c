/**
  */

#include "stm32f7xx.h"
#include "options.h"
#include "s_gpio.h"

// declaration
void SystemCoreClockUpdate(void);

// variable for debug
volatile unsigned int lookatme;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  */
int main(void)
{
lookatme = SystemCoreClock;
SystemCoreClockUpdate();
lookatme = SystemCoreClock;
GPIO_config_LCD_BL();	// eteindre le backlight qui est on au reset (pullup)
GPIO_config_profiler_PI1_PI2();
  /* Infinite loop */
  while (1)
  {
  profile_D13( 1 );
  profile_D8( 0 );
  lookatme ^= 1;
  profile_D13( 0 );
  profile_D8( 1 );
  lookatme ^= 1;
  }
}

