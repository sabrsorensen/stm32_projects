#include "lcd_config.h"
#include "stm32f1xx_hal.h"

// Low-level pin management for LCD command functions
void write_lcd_data_bitbang(uint8_t data);
void write_lcd_data_bitmask(uint8_t data);
void write_lcd_data(uint8_t data);
void write_lcd_command(GPIO_PinState reg_select, GPIO_PinState read_write, uint8_t command);

// High-level LCD command functions
void clear_display();
void return_home();
void set_entry_mode(GPIO_PinState increment, GPIO_PinState shift);
void set_display_control(GPIO_PinState display_on, GPIO_PinState cursor_on, GPIO_PinState blink_on);
void cursor_or_display_shift(GPIO_PinState shift_or_cursor, GPIO_PinState right_or_left);
void function_set(GPIO_PinState data_length, GPIO_PinState num_lines, GPIO_PinState font);
void set_cgram_address(uint8_t address);
void set_ddram_address(uint8_t address);
void set_cursor_position(uint8_t line, uint8_t column, uint8_t num_lines);
void set_cursor_position_1line(uint8_t column);
void set_cursor_position_2line(uint8_t line, uint8_t column);
void write_data_to_ram(uint8_t data);
void write_data_to_cgram(uint8_t address, uint8_t data);
void write_data_to_ddram(uint8_t address, uint8_t data);

// Unimplemented placeholders for future functionality
void read_busy_flag_and_address();
void read_data_from_ram();
