#include "stm32f1xx_hal.h"
#include "../lib/LCD_MTC_S16204XRGHS/include/lcd_driver.h"

void SystemClock_Config(void);
void lcd_gpio_init(void);
void lcd_compatibility_test(void);

int main(void)
{
    HAL_Init();
    lcd_gpio_init();
    lcd_compatibility_test();

    while (1)
    {
        clear_display();
        set_ddram_address(0x00);
        write_string("Hello World!");
        HAL_Delay(2000);

        clear_display();
        set_ddram_address(0x00);
        write_string("LCD Driver Test");
        HAL_Delay(2000);

        clear_display();
        set_ddram_address(0x00);
        write_string("Line 1");
        set_ddram_address(0x40); // Second line
        write_string("Line 2");
        HAL_Delay(2000);
    }
}

void lcd_compatibility_test(void)
{
    HAL_Delay(50); // Power-on delay

    // Function set: 8-bit, 2-line, 5x8 font
    function_set(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
    HAL_Delay(5);

    // Display control: display on, cursor off, blink off
    set_display_control(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
    HAL_Delay(1);

    // Clear display
    clear_display();
    HAL_Delay(2);

    // Entry mode: increment, no shift
    set_entry_mode(GPIO_PIN_SET, GPIO_PIN_RESET);
    HAL_Delay(1);

    set_ddram_address(0x00);
    write_lcd_command(GPIO_PIN_SET, GPIO_PIN_RESET, 'H'); // Write 'H' to display
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void lcd_gpio_init(void)
{
    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure data pins (PA0-PA7) as outputs
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure control pins as outputs
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2; // Adjust based on your pin fix
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}