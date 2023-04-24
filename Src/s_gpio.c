#include "options.h"

#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_gpio.h"
#include "s_gpio.h"

#ifdef USE_UART1
void GPIO_config_uart1(void)
{
// port A
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
// PA9 alternate push pull, 2MHz		// UART 1 TX
LL_GPIO_SetPinMode(       GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
LL_GPIO_SetPinOutputType( GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL );
LL_GPIO_SetPinSpeed(      GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_MEDIUM);
LL_GPIO_SetPinPull(       GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_NO );
LL_GPIO_SetAFPin_0_7(     GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_7);
/*
LL_GPIO_InitTypeDef gpio_initstruct;
gpio_initstruct.Mode       = LL_GPIO_MODE_ALTERNATE;
gpio_initstruct.Speed      = LL_GPIO_SPEED_FREQ_MEDIUM;
gpio_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
gpio_initstruct.Pull       = LL_GPIO_PULL_NO;
gpio_initstruct.Alternate  = LL_GPIO_AF_7;
gpio_initstruct.Pin        = LL_GPIO_PIN_9;
LL_GPIO_Init(GPIOA, &gpio_initstruct);
*/
// port B
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
// RB7 alternate input				// UART 1 RX
LL_GPIO_SetPinMode(       GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
LL_GPIO_SetPinPull(       GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
LL_GPIO_SetPinSpeed(      GPIOB, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_MEDIUM);
LL_GPIO_SetAFPin_0_7(     GPIOB, LL_GPIO_PIN_7, LL_GPIO_AF_7);
/*
gpio_initstruct.Mode       = LL_GPIO_MODE_ALTERNATE;
gpio_initstruct.Speed      = LL_GPIO_SPEED_FREQ_MEDIUM;
gpio_initstruct.Pull       = LL_GPIO_PULL_NO;
gpio_initstruct.Alternate  = LL_GPIO_AF_7;
gpio_initstruct.Pin        = LL_GPIO_PIN_7;
LL_GPIO_Init(GPIOB, &gpio_initstruct);
*/
}
#endif
#ifdef USE_UART6
void GPIO_config_uart6(void)
{
// port C
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
// CN4.D1	PC6 = UART6 TX	AF8
// CN4.D0	PC7 = UART6 RX	AF8
LL_GPIO_InitTypeDef gpio_initstruct;
gpio_initstruct.Mode       = LL_GPIO_MODE_ALTERNATE;
gpio_initstruct.Speed      = LL_GPIO_SPEED_FREQ_MEDIUM;
gpio_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
gpio_initstruct.Pull       = LL_GPIO_PULL_NO;
gpio_initstruct.Alternate  = LL_GPIO_AF_8;
gpio_initstruct.Pin        = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
LL_GPIO_Init(GPIOC, &gpio_initstruct);
}
#endif

#ifdef PROFILER_PI2
// GPIO output pins PI1 (LED verte pres du bouton reset) et PI2 for profiling
void GPIO_config_profiler_PI1_PI2( void )
{
// port I (aussi utilise pour LED verte et bouton bleu)
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
// CN7.D13	PI1	(LED verte)
LL_GPIO_SetPinMode(       GPIOI, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT );
LL_GPIO_SetPinOutputType( GPIOI, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL );
LL_GPIO_SetPinSpeed(      GPIOI, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_MEDIUM);
LL_GPIO_SetPinPull(       GPIOI, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);
// CN7.D8	PI2
LL_GPIO_SetPinMode(       GPIOI, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT );
LL_GPIO_SetPinOutputType( GPIOI, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL );
LL_GPIO_SetPinSpeed(      GPIOI, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_VERY_HIGH);
LL_GPIO_SetPinPull(       GPIOI, LL_GPIO_PIN_2, LL_GPIO_PULL_NO);
/*
LL_GPIO_InitTypeDef gpio_initstruct;
gpio_initstruct.Mode       = LL_GPIO_MODE_OUTPUT;
gpio_initstruct.Speed      = LL_GPIO_SPEED_FREQ_VERY_HIGH;
gpio_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
gpio_initstruct.Pull       = LL_GPIO_PULL_NO;
gpio_initstruct.Pin        = LL_GPIO_PIN_1 | LL_GPIO_PIN_2;
LL_GPIO_Init(GPIOI, &gpio_initstruct);
*/
profile_D13(0);
profile_D8(0);
}

void profile_D13( int val )	// aka green LED
{
if	( val )
	LL_GPIO_SetOutputPin(   GPIOI, LL_GPIO_PIN_1 );
else	LL_GPIO_ResetOutputPin( GPIOI, LL_GPIO_PIN_1 );
}

void profile_D8( int val )
{
if	( val )
	LL_GPIO_SetOutputPin(   GPIOI, LL_GPIO_PIN_2 );
else	LL_GPIO_ResetOutputPin( GPIOI, LL_GPIO_PIN_2 );
}
#endif

// GPIO output pins for LCD LED backlight
void GPIO_config_LCD_BL( void )
{

// port K
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOK);

LL_GPIO_SetPinMode(       GPIOK, LCD_BL_GPIOK_PIN, LL_GPIO_MODE_OUTPUT );
LL_GPIO_SetPinOutputType( GPIOK, LCD_BL_GPIOK_PIN, LL_GPIO_OUTPUT_PUSHPULL );
LL_GPIO_SetPinSpeed(      GPIOK, LCD_BL_GPIOK_PIN, LL_GPIO_SPEED_FREQ_LOW );
LL_GPIO_SetPinPull(       GPIOK, LCD_BL_GPIOK_PIN, LL_GPIO_PULL_NO );
/*
LL_GPIO_InitTypeDef gpio_initstruct;
gpio_initstruct.Mode       = LL_GPIO_MODE_OUTPUT;
gpio_initstruct.Speed      = LL_GPIO_SPEED_FREQ_LOW;
gpio_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
gpio_initstruct.Pull       = LL_GPIO_PULL_NO;
gpio_initstruct.Pin        = LCD_BL_GPIOK_PIN;
LL_GPIO_Init(GPIOK, &gpio_initstruct);
*/
LCD_BL(0);
}

void LCD_BL( int val )
{
if	( val )
	LL_GPIO_SetOutputPin(   GPIOK, LCD_BL_GPIOK_PIN );
else	LL_GPIO_ResetOutputPin( GPIOK, LCD_BL_GPIOK_PIN );
}

