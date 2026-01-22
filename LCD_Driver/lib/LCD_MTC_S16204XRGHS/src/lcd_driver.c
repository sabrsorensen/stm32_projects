#include "../include/lcd_driver.h"
#include "stm32f1xx_hal.h"

void write_lcd_data_bitbang(uint8_t data)
{
    // Use port and pin definitions to individually set each data pin
    // ANDs the data with the respective bitmask for the relevant pin to clear other bits and sets the pin high for true and low for false
    HAL_GPIO_WritePin(LCD_D0_PORT, LCD_D0_PIN, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D1_PORT, LCD_D1_PIN, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D2_PORT, LCD_D2_PIN, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D3_PORT, LCD_D3_PIN, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D4_PORT, LCD_D4_PIN, (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_PORT, LCD_D5_PIN, (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_PORT, LCD_D6_PIN, (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_PORT, LCD_D7_PIN, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void write_lcd_data_bitmask(uint8_t data)
{
    uint32_t port_value = LCD_DATA_PORT->ODR;         // Read current state of output data register
    port_value &= ~(LCD_DATA_MASK << LCD_DATA_SHIFT); // Use the mask and data shift to clear bits
    port_value |= ((uint32_t)data << LCD_DATA_SHIFT); // Use the shift to set new data bits
    LCD_DATA_PORT->ODR = port_value;                  // Write updated state back to output data register
}

void write_lcd_data(uint8_t data)
{
    if (LCD_D0_PORT == LCD_DATA_PORT &&
        LCD_D1_PORT == LCD_DATA_PORT &&
        LCD_D2_PORT == LCD_DATA_PORT &&
        LCD_D3_PORT == LCD_DATA_PORT &&
        LCD_D4_PORT == LCD_DATA_PORT &&
        LCD_D5_PORT == LCD_DATA_PORT &&
        LCD_D6_PORT == LCD_DATA_PORT &&
        LCD_D7_PORT == LCD_DATA_PORT)
    {
        // All data pins are using the same port, can use bitmask method
        write_lcd_data_bitmask(data);
    }
    else
    {
        // Data pins are using different ports, must use bitbang method
        write_lcd_data_bitbang(data);
    }
}

void write_lcd_command(GPIO_PinState reg_select, GPIO_PinState read_write, uint8_t command)
{
    HAL_GPIO_WritePin(LCD_DIRS_PORT, LCD_DIRS_PIN, reg_select); // Set Data/Instruction Register Select
    HAL_GPIO_WritePin(LCD_RWS_PORT, LCD_RWS_PIN, read_write);   // Set Read/Write Select

    write_lcd_data(command); // Write the command to the data bus

    // Pulse the Enable pin to latch the command
    HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_SET);
    HAL_Delay(1); // tW in timing chart, short delay to ensure correct enable pulse width. Datasheet measures delays in nanoseconds, so 1 ms is more than sufficient.
    HAL_GPIO_WritePin(LCD_EN_PORT, LCD_EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(1); // tH, tH1, tH2, or tDH in timing chart, short delay to ensure RegSelect, R/W, and Data pins are held long enough to be latched.
    // The above delay is also sufficient to cover tC, the minimum Enable cycle time before another command can be sent.
}

void clear_display()
{
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x01;                    // Clear Display command
    write_lcd_command(reg_select, read_write, command);
    HAL_Delay(1); // Clear Display command requires a longer delay to complete, write_lcd_command already includes a 1 ms delay, so an additional delay is added here.
}

void return_home()
{
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x02;                    // Return Home command
    // bit 0 is don't care
    write_lcd_command(reg_select, read_write, command);
    HAL_Delay(1); // Return Home command requires a longer delay to complete, write_lcd_command already includes a 1 ms delay, so an additional delay is added here.
}

void set_entry_mode(GPIO_PinState increment, GPIO_PinState shift)
{
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x04;                    // Base Entry Mode Set command
    if (increment == GPIO_PIN_SET)
    {
        command |= 0x02; // Set I/D bit for increment
    }
    if (shift == GPIO_PIN_SET)
    {
        command |= 0x01; // Set S bit for display shift
    }
    write_lcd_command(reg_select, read_write, command);
}

void set_display_control(GPIO_PinState display_on, GPIO_PinState cursor_on, GPIO_PinState blink_on)
{
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x08;                    // Display Control command
    if (display_on == GPIO_PIN_SET)
    {
        command |= 0x04; // Set D bit for display on
    }
    if (cursor_on == GPIO_PIN_SET)
    {
        command |= 0x02; // Set C bit for cursor on
    }
    if (blink_on == GPIO_PIN_SET)
    {
        command |= 0x01; // Set B bit for blink on
    }
    write_lcd_command(reg_select, read_write, command);
}

void cursor_or_display_shift(GPIO_PinState shift_or_cursor, GPIO_PinState right_or_left)
{
    // shift_or_cursor: GPIO_PIN_SET for display shift, GPIO_PIN_RESET for cursor move
    // right_or_left: GPIO_PIN_SET for right, GPIO_PIN_RESET for left
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x10;                    // Cursor or Display Shift command
    if (shift_or_cursor == GPIO_PIN_SET)
    {
        command |= 0x08; // Set S/C bit for increment
    }
    if (right_or_left == GPIO_PIN_SET)
    {
        command |= 0x04; // Set R/L bit for display shift
    }
    // bits 0 and 1 are don't care
    write_lcd_command(reg_select, read_write, command);
}

void function_set(GPIO_PinState data_length, GPIO_PinState num_lines, GPIO_PinState font)
{
    // data_length: GPIO_PIN_SET for 8-bit, GPIO_PIN_RESET for 4-bit
    // num_lines: GPIO_PIN_SET for 2 lines, GPIO_PIN_RESET for 1 line
    // font: GPIO_PIN_SET for 5x11 dots, GPIO_PIN_RESET for 5x8 dots
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x20;                    // Function Set command
    if (data_length == GPIO_PIN_SET)
    {
        command |= 0x10; // Set DL bit for 8-bit data length
    }
    if (num_lines == GPIO_PIN_SET)
    {
        command |= 0x08; // Set N bit for 2 lines
    }
    if (font == GPIO_PIN_SET)
    {
        command |= 0x04; // Set F bit for 5x11 dots font
    }
    write_lcd_command(reg_select, read_write, command);
}

void set_cgram_address(uint8_t address)
{
    // Address is 6 bits (0-63)
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x40 | (address & 0x3F); // Set CGRAM Address command with address
    write_lcd_command(reg_select, read_write, command);
}

void set_ddram_address(uint8_t address)
{
    // In 1-line display mode, address can be 0x00-0x4F
    // In 2-line display mode, 1st line is 0x00-0x27, 2nd line is 0x40-0x67
    GPIO_PinState reg_select = GPIO_PIN_RESET; // Instruction mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    uint8_t command = 0x80 | (address & 0x7F); // Set DDRAM Address command with address
    write_lcd_command(reg_select, read_write, command);
}

void set_cursor_position(uint8_t line, uint8_t column, uint8_t num_lines)
{
    // In 1-line display mode, address can be 0x00-0x4F
    // In 2-line display mode, 1st line is 0x00-0x27, 2nd line is 0x40-0x67
    uint8_t address;

    if (line == 0)
    {
        address = column;
        if (num_lines > 1 && address > 0x27)
        {
            address = 0x27; // 2-line mode, cap to max for line 1
        }
        else if (num_lines == 1 && address > 0x4F)
        {
            address = 0x4F; // 1-line mode, cap to max for line 1
        }
    }
    else
    {
        address = 0x40 + column; // Line 2 starts at address 0x40
        if (num_lines > 1 && address > 0x67)
        {
            address = 0x67; // 2-line mode, cap to max for line 2
        }
    }

    set_ddram_address(address);
}

void set_cursor_position_1line(uint8_t column)
{
    set_cursor_position(0, column, 1);
}

void set_cursor_position_2line(uint8_t line, uint8_t column)
{
    set_cursor_position(line, column, 2);
}

void write_data_to_ram(uint8_t data)
{
    // Addressing mode must be already set by either set_cgram_address or set_ddram_address
    GPIO_PinState reg_select = GPIO_PIN_SET;   // Data mode
    GPIO_PinState read_write = GPIO_PIN_RESET; // Write mode
    write_lcd_command(reg_select, read_write, data);
}

void write_data_to_cgram(uint8_t address, uint8_t data)
{
    // Set the address in CGRAM then write data
    set_cgram_address(address);
    write_data_to_ram(data);
}

void write_data_to_ddram(uint8_t address, uint8_t data)
{
    // Set the address in DDRAM then write data
    set_ddram_address(address);
    write_data_to_ram(data);
}

void read_busy_flag_and_address()
{
    // Not implemented
    // Reading from the LCD requires setting data pins as inputs and managing R/W and EN signals accordingly.
    // This function is a placeholder for future implementation.
}

void read_data_from_ram()
{
    // Not implemented
    // Reading from the LCD requires setting data pins as inputs and managing R/W and EN signals accordingly.
    // This function is a placeholder for future implementation.
    // Requires setting the addressing mode by either set_cgram_address or set_ddram_address.
}