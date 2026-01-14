#include "stm32f1xx_hal.h"

#define LED_PIN GPIO_PIN_13
#define LED_GPIO_PORT GPIOC
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()

#define RGB_COLOR_LED_PIN_RED GPIO_PIN_5
#define RGB_COLOR_LED_PIN_GREEN GPIO_PIN_6
#define RGB_COLOR_LED_PIN_BLUE GPIO_PIN_7
#define RGB_COLOR_LED_GPIO_PORT GPIOB
#define RGB_COLOR_LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define BUTTON_PIN GPIO_PIN_0
#define BUTTON_GPIO_PORT GPIOB
#define BUTTON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

void LED_Init();
void RGB_Color_LED_Init();
void Button_Init();

int main(void)
{
    HAL_Init();
    LED_Init();
    RGB_Color_LED_Init();
    Button_Init();

    while (1)
    {
        // Read button state (LOW = pressed, HIGH = not pressed with pull-up)
        GPIO_PinState button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_PIN);
        int delay = (button_state == GPIO_PIN_RESET) ? 200 : 1000; // 200ms if pressed, else 1000ms

        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_RED, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_GREEN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_BLUE, GPIO_PIN_RESET);
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(delay);

        button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_PIN);
        delay = (button_state == GPIO_PIN_RESET) ? 200 : 1000; // 200ms if pressed, else 1000ms

        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_RED, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_GREEN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_BLUE, GPIO_PIN_RESET);
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(delay);

        button_state = HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_PIN);
        delay = (button_state == GPIO_PIN_RESET) ? 200 : 1000; // 200ms if pressed, else 1000ms

        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_RED, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_GREEN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RGB_COLOR_LED_GPIO_PORT, RGB_COLOR_LED_PIN_BLUE, GPIO_PIN_SET);
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(delay);
    }
}

void LED_Init()
{
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void RGB_Color_LED_Init()
{
    RGB_COLOR_LED_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = RGB_COLOR_LED_PIN_RED | RGB_COLOR_LED_PIN_GREEN | RGB_COLOR_LED_PIN_BLUE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RGB_COLOR_LED_GPIO_PORT, &GPIO_InitStruct);
}

void Button_Init()
{
    BUTTON_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // Enable pull-up for button
    HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
