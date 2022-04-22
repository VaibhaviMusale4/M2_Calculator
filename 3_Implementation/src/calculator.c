#include "calculator.h"

uint16_t scan_and_eval(void){
  uint16_t result;
  uint8_t key_press;
  uint8_t i=0,j=0;
  uint16_t num_1=0,num_2=0,op_temp=0;
  struct Stack * op = create_stack(100);
  struct Stack * number = create_stack(100);
  uint8_t temp[20]={0};
  uint8_t precedence [50]={0};
  precedence[42]=2;				
  precedence[47]=2;				
  precedence[43]=1;				
  precedence[45]=1;				
  push(op,0);

  while(1){

    key_press=keypad_scan();
	if(key_press !=0){			
		lcd_write_character(key_press);

		if(key_press=='='){
			temp[j]='\0';
			input[i]=string_to_number(temp);
			push(number,input[i++]);
			while(peek(op)!=0){
			  num_2=pop(number);
			  num_1=pop(number);
			  op_temp=pop(op);
			  result=do_math(num_1,num_2,op_temp);
			  push(number,result);
			}
			return peek(number);
		}

		else if (key_press=='+'||key_press=='*'||key_press=='-'||key_press=='/'){
		  temp[j]='\0';
		  input[i]=string_to_number(temp);
		  push(number,input[i++]);
		  if(precedence[key_press] < precedence[peek(op)]){
			  num_2=pop(number);
			  num_1=pop(number);
			  op_temp=pop(op);
			  result=do_math(num_1,num_2,op_temp);
			  push(number,result);
			  push(op,key_press);
			}
			else{
			 push(op,key_press);
			}
			j=0;
		}
		else
		  temp[j++]=key_press;
		}
	}
}

uint16_t string_to_number(uint8_t* str_num){
  uint16_t number=0;
  uint8_t i=0;
  while(str_num[i]!='\0'){
    number = 10*number + (str_num[i++]-48);
  }

  return number;
}

uint8_t* number_to_string(uint16_t number){
   uint8_t i=0;
  struct Stack * num=create_stack(20);
  while(number){
    push(num,(number%10)+48);
    number/=10;
  }
  while(!is_empty(num)){
	str_num[i++]=pop(num);
  }
  str_num[i]='\0';
  return str_num;
}

uint16_t do_math(uint16_t num_1,uint16_t num_2,uint8_t op){
  uint16_t result=0;
  switch (op){
    case '+':
      result=num_1+num_2;
      break;
    case '-':
      result=num_1-num_2;
      break;
    case '*':
      result=num_1*num_2;
      break;
    case '/':
        result=num_1/num_2;
        break;
  }

  return result;
}


void print_result(uint16_t result){
  uint8_t * str_num;
  str_num = number_to_string(result);
  lcd_write_word(str_num);
}

/**
 * @brief Create a stack object
 * 
 * @param s 
 * @return struct Stack* 
 */

struct Stack* create_stack(uint16_t s){
  struct Stack* stack = (struct Stack *) malloc(sizeof(struct Stack));
  stack ->size=s;
  stack ->top=-1;
  stack  -> data= (uint16_t*) malloc(stack-> size *sizeof(uint16_t));
  return stack;
}

void push(struct Stack* stack,uint16_t item){
  if(!is_full(stack)){
    stack->data[++stack->top]=item;
  }
  else
    return;
}

uint16_t peek(struct Stack* stack){
  if(!is_empty(stack)){
  return stack->data[stack->top];
  }
  return -1;
}

uint16_t pop(struct Stack* stack){
  if(!is_empty(stack)){
    return stack->data[stack->top--];
  }
  return -1;
}

uint8_t is_empty(struct Stack* stack){
  if(stack->top==-1)
    return IS_EMPTY;
  else
    return IS_NOT_EMPTY;
}

uint8_t is_full(struct Stack* stack){
  if(stack->top==stack->size)
    return IS_FULL;
  else
    return IS_NOT_FULL;
}

/**
 * @brief LCD
 * 
 */

void lcd_init(void)
{

	DATA_DDR = (1<<LCD_D7) | (1<<LCD_D6) | (1<<LCD_D5)| (1<<LCD_D4);
	CTL_DDR |= (1<<LCD_EN)|(1<<LCD_RW)|(1<<LCD_RS);
	
	DATA_BUS = (0<<LCD_D7)|(0<<LCD_D6)|(1<<LCD_D5)|(0<<LCD_D4);
	CTL_BUS|= (1<<LCD_EN)|(0<<LCD_RW)|(0<<LCD_RS);
	
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(1);

	lcd_send_command(LCD_CMD_4BIT_2ROW_5X7);
	_delay_ms(1);
	lcd_send_command(LCD_CMD_DISPLAY_CURSOR_BLINK);
	_delay_ms(1);
	lcd_send_command(0x80);

}

void lcd_send_command (uint8_t command)
{
	DATA_BUS=((command&0b11110000)>>4);
	CTL_BUS &=~(1<<LCD_RS);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(1);
	DATA_BUS=(command&0b00001111);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(1);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(1);
}

void lcd_write_word(uint8_t word[20])
{
	int i=0;
	while(word[i]!='\0')
	{
		lcd_write_character(word[i]);
		i++;
	}
}

void lcd_write_character(uint8_t character)
{

	DATA_BUS=((character & 0b11110000)>>4);
	CTL_BUS|=(1<<LCD_RS);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(2);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(2);
	DATA_BUS=(character & 0b00001111);
	CTL_BUS |=(1<<LCD_EN);
	_delay_ms(2);
	CTL_BUS &=~(1<<LCD_EN);
	_delay_ms(2);
}

void lcd_clear(void)
{
	lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
	_delay_ms(5);
}
void lcd_goto_xy (uint8_t line,uint8_t pos)
{
	lcd_send_command((0x80|(line<<6))+pos);
	_delay_us (50);
}

/**
 * @brief Keypad
 * 
 */

void keypad_init(void)
{

ROW0_DDR |=(1U<<ROW0);
ROW1_DDR |=(1U<<ROW1);
ROW2_DDR |=(1U<<ROW2);
ROW3_DDR |=(1U<<ROW3);

LINE0_DDR &=~(1U<<LINE0);
LINE1_DDR &=~(1U<<LINE1);
LINE2_DDR &=~(1U<<LINE2);
LINE3_DDR &=~(1U<<LINE3);

LINE0_PULLUP |=(1U<<LINE0);
LINE1_PULLUP |=(1U<<LINE1);
LINE2_PULLUP |=(1U<<LINE2);
LINE3_PULLUP |=(1U<<LINE3);

ROW0_PORT |=(1U<<ROW0);
ROW1_PORT |=(1U<<ROW1);
ROW2_PORT |=(1U<<ROW2);
ROW3_PORT |=(1U<<ROW3);
}

/**
 * @brief scan the keypad and return the pressed key and returns null if no key pressed
 * 
 * @return uint8_t 
 */
uint8_t keypad_scan(void)
{
uint8_t row=0,input;

	for(row=0;row<4;row++)
	{
	
	ROW0_PORT |=(1<<ROW0);
	ROW1_PORT |=(1<<ROW1);
	ROW2_PORT |=(1<<ROW2);
	ROW3_PORT |=(1<<ROW3);

	switch(row)
		{
		case 0:ROW0_PORT &=~(1<<ROW0);
				break;
		case 1:ROW1_PORT &=~(1<<ROW1);
				break;
		case 2:ROW2_PORT &=~(1<<ROW2);
				break;
		case 3:ROW3_PORT &=~(1<<ROW3);
				break;
		}

		input  = (LINE0_PIN&(1<<LINE0));
		input |= (LINE1_PIN&(1<<LINE1));
		input |= (LINE2_PIN&(1<<LINE2));
		input |= (LINE3_PIN&(1<<LINE3));
		
		if(ALL_LINES != 1)
		{
			while(ALL_LINES != 1);
			break;
		}
	}
	if(row == 4) 
	{
		return 0;
	}
	
	switch(row)
	{
		case 0:
				switch(input)
					{
					case 0x70: return '/';
					case 0xB0: return '9';
					case 0xD0: return '8';
					case 0xE0: return '7';
					}
		case 1:
					switch(input)
					{
						case 0x70: return '*';
						case 0xB0: return '6';
						case 0xD0: return '5';
						case 0xE0: return '4';
					}
		case 2:
					switch(input)
					{
						case 0x70: return '-';
						case 0xB0: return '3';
						case 0xD0: return '2';
						case 0xE0: return '1';
					}
		case 3:
					switch(input)
					{
						case 0x70: return '+';
						case 0xB0: return '=';
						case 0xD0: return '0';
						case 0xE0: return 'c';
					}
	}
	return 0;
}