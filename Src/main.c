/**
  */

#include "stm32f7xx.h"

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

  /* Infinite loop */
  while (1)
  {
  lookatme ^= 1;
  }
}

