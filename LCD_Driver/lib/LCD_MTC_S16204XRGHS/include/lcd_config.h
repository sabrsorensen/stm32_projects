#include "stm32f1xx_hal.h"

// LCD Data Bus Pin Definitions
#define LCD_D0_PORT GPIOA
#define LCD_D0_PIN GPIO_PIN_0
#define LCD_D1_PORT GPIOA
#define LCD_D1_PIN GPIO_PIN_1
#define LCD_D2_PORT GPIOA
#define LCD_D2_PIN GPIO_PIN_2
#define LCD_D3_PORT GPIOA
#define LCD_D3_PIN GPIO_PIN_3
#define LCD_D4_PORT GPIOA
#define LCD_D4_PIN GPIO_PIN_4
#define LCD_D5_PORT GPIOA
#define LCD_D5_PIN GPIO_PIN_5
#define LCD_D6_PORT GPIOA
#define LCD_D6_PIN GPIO_PIN_6
#define LCD_D7_PORT GPIOA
#define LCD_D7_PIN GPIO_PIN_7

// Control pins
//      DIRS - Data/Instruction Register Select
#define LCD_DIRS_PORT GPIOA
#define LCD_DIRS_PIN GPIO_PIN_8
//      RWS  - Read/Write Select
#define LCD_RWS_PORT GPIOA
#define LCD_RWS_PIN GPIO_PIN_9
//      EN   - Enable Pin
#define LCD_EN_PORT GPIOA
#define LCD_EN_PIN GPIO_PIN_10

// For use with bitmask method
#define LCD_DATA_PORT GPIOA
#define LCD_DATA_MASK 0x000000FF // Bits 0-7
#define LCD_DATA_SHIFT 0