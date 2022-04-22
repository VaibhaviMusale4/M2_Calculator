#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

uint16_t input[20];
uint8_t str_num[20];


uint16_t scan_and_eval(void);
uint16_t string_to_number(uint8_t*);
uint8_t* number_to_string(uint16_t);
uint16_t do_math(uint16_t,uint16_t,uint8_t);
void print_result(uint16_t);

#define ALL_LINES	    (((LINE0_PIN&(1U<<LINE0))>>LINE0)\
					   &((LINE1_PIN&(1U<<LINE1))>>LINE1)\
					   &((LINE2_PIN&(1U<<LINE2))>>LINE2)\
					   &((LINE3_PIN&(1U<<LINE3))>>LINE3))

void keypad_init(void);
uint8_t keypad_scan(void);

#define LCD_CMD_CLEAR_DISPLAY	             0x01
#define LCD_CMD_CURSOR_HOME		             0x02

#define LCD_CMD_DISPLAY_OFF                0x08
#define LCD_CMD_DISPLAY_NO_CURSOR          0x0c
#define LCD_CMD_DISPLAY_CURSOR_NO_BLINK    0x0E
#define LCD_CMD_DISPLAY_CURSOR_BLINK       0x0F

#define LCD_CMD_4BIT_2ROW_5X7              0x28
#define LCD_CMD_8BIT_2ROW_5X7              0x38

void lcd_init(void);
void lcd_send_command (uint8_t );
void lcd_write_character(uint8_t );
void lcd_write_word(uint8_t[]);
void lcd_clear(void);
void lcd_set_courser(uint8_t,uint8_t);
void lcd_goto_xy (uint8_t , uint8_t );

#define		ROW0			   0
#define		ROW0_DDR		 DDRD
#define		ROW0_PORT		 PORTD

#define		ROW1			    1
#define		ROW1_DDR		  DDRD
#define		ROW1_PORT		  PORTD

#define		ROW2		     	 2
#define		ROW2_DDR		  DDRD
#define		ROW2_PORT		  PORTD

#define		ROW3			     3
#define		ROW3_DDR	  	  DDRD
#define		ROW3_PORT		  PORTD

#define		LINE0			     4
#define		LINE0_DDR		  DDRD
#define		LINE0_PIN		  PIND
#define		LINE0_PULLUP	  PORTD


#define		LINE1		    	 5
#define		LINE1_DDR		  DDRD
#define		LINE1_PIN		  PIND
#define		LINE1_PULLUP	  PORTD

#define		LINE2			     6
#define		LINE2_DDR		  DDRD
#define		LINE2_PIN		  PIND
#define		LINE2_PULLUP	  PORTD

#define		LINE3			     7
#define		LINE3_DDR		  DDRD
#define		LINE3_PIN		  PIND
#define		LINE3_PULLUP	  PORTD

#define   DATA_BUS 	    	 PORTC
#define   CTL_BUS		     PORTB
#define   DATA_DDR	    	 DDRC
#define   CTL_DDR		     DDRB

#define   LCD_D4			   0
#define   LCD_D5			   1
#define   LCD_D6			   2
#define   LCD_D7			   3

#define   LCD_EN			   0
#define	  LCD_RW			   1
#define	  LCD_RS			   2

#define   IS_FULL         1
#define   IS_NOT_FULL     0
#define   IS_EMPTY        1
#define   IS_NOT_EMPTY    0

struct Stack{
  uint16_t size;
  uint16_t top;
  uint16_t * data;
};

struct   Stack* create_stack(uint16_t);
void     push(struct Stack*,uint16_t);
uint16_t peek(struct Stack*);
uint16_t pop(struct Stack*);
uint8_t  is_empty(struct Stack*);
uint8_t  is_full(struct Stack*);

#endif