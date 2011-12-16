#ifndef __LCD_H_INCLUDED__
#define __LCD_H_INCLUDED__

// function prototypes
void clear_lcd(void);
void lcd_goto(int x, int y);
void lcd_command(int command_code);
void lcd_puts(char* string);
#endif __LCD_H_INCLUDED__