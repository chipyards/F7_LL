// carte disco F7 LCD

// bouton bleu
#define IS_BLUE_PRESSED()	LL_GPIO_IsInputPinSet( GPIOI, LL_GPIO_PIN_11 )


/* Display enable pin, act high, R74 pulldown */
#define LCD_DISP_GPIOI_PIN	LL_GPIO_PIN_12
/* Backlight control pin, act high, R85 pullup */
#define LCD_BL_GPIOK_PIN	LL_GPIO_PIN_3


#ifdef __cplusplus
extern "C" {
#endif

// configuration des GPIO
void GPIO_config(void);
void GPIO_config_uart1(void);
void GPIO_config_uart6(void);
void GPIO_config_profiler_PI1_PI2( void );
void GPIO_config_LCD_BL(void);
void LCD_BL( int val );
// action bits profiler
void profile_D13( int val );
void profile_D8( int val );
#define profile_D8_hi()  LL_GPIO_SetOutputPin( GPIOI, LL_GPIO_PIN_2 )
#define profile_D8_lo()  LL_GPIO_ResetOutputPin( GPIOI, LL_GPIO_PIN_2 )
#define profile_D8_tog() LL_GPIO_TogglePin( GPIOI, LL_GPIO_PIN_2 )

#ifdef __cplusplus
}
#endif
