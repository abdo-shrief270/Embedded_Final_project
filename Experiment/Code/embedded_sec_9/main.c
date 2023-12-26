/*
 * SEC_8_LCD.c
 *
 * Created: 11/28/2023 8:55:30 AM
 * Author : AbdelRaHmaN ShRieF
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>


// Commends Macros

#define LCD_CLEAR_SCREEN					0x01
#define LCD_RETURN_HOME		        		0x02
#define LCD_DISP_ON_CURSOR_BLINK			0x0C
#define LCD_SHIFT_CURSOR_POS_RIGHT			0x14
#define LCD_FUNCTION_8BIT_2LINES			0x38


// DIO Write PINS Macros

#define SET_BIT(REG,BIT) (REG |= (1<<BIT))
#define CLR_BIT(REG,BIT) (REG &= ~(1<<BIT))
#define GET_BIT(REG,BIT) (REG>>BIT & 1 )

// LCD Configurations Macros
#define ADC_VOLT_REF		3
#define ADC_STORE_MODE		0
#define LCD_DATA_PORT       PORTD
#define LCD_DATA_DIR        DDRD
#define LCD_COMMAND_DIR     DDRC
#define LCD_COMMAND_PORT    PORTC
#define LCD_RS_PIN          0
#define LCD_RW_PIN          1
#define LCD_EN_PIN          2
#define degree_sysmbol		0xdf

void LCD_vidInit(void)
{
	_delay_ms(200);
	LCD_DATA_DIR=0xff;
	SET_BIT(LCD_COMMAND_DIR,LCD_RS_PIN);
	SET_BIT(LCD_COMMAND_DIR,LCD_RW_PIN);
	SET_BIT(LCD_COMMAND_DIR,LCD_EN_PIN);
	LCD_vidSendCmd(LCD_RETURN_HOME);
	_delay_ms(50);
	LCD_vidSendCmd(LCD_FUNCTION_8BIT_2LINES);
	_delay_ms(2);
	LCD_vidSendCmd(LCD_DISP_ON_CURSOR_BLINK);
	_delay_ms(2);
	LCD_vidSendCmd(LCD_CLEAR_SCREEN);
	_delay_ms(2);

}

void LCD_vidSendCmd(unsigned char enuCmd)
{
	CLR_BIT(LCD_COMMAND_PORT,LCD_RS_PIN);
	LCD_DATA_PORT=enuCmd;
	LCD_vidSendEnPulse(LCD_COMMAND_PORT,LCD_EN_PIN);
}

void LCD_vidSendChar(unsigned char u8Char)
{
	SET_BIT(LCD_COMMAND_PORT,LCD_RS_PIN);
	LCD_DATA_PORT=u8Char;
	LCD_vidSendEnPulse(LCD_COMMAND_PORT,LCD_EN_PIN);
}

void LCD_vidSendEnPulse(void)
{
	SET_BIT(LCD_COMMAND_PORT,LCD_EN_PIN);
	_delay_ms(10);
	CLR_BIT(LCD_COMMAND_PORT,LCD_EN_PIN);
}


void LCD_vidMoveCursor(char u8Line, char u8Position)
{
	if(u8Line < 1 || u8Line > 2 || u8Position < 1 ||u8Position > 16)
	{
		LCD_vidSendCmd(0x80);
	}
	else if(u8Line == 1)
	{
		LCD_vidSendCmd(0x80 +(u8Position - 1));
	}
	else if(u8Line == 2)
	{
		LCD_vidSendCmd(0xC0 +(u8Position - 1));
	}
	
	_delay_ms(1);
}


void LCD_vidSendString(char* u8String)
{
	while((*u8String)!= '\0')
	{
		LCD_vidSendChar(*u8String);
		u8String++;
	}
}


void LCD_vidSendNumber(int Num){
	char str[20];
	sprintf(str,"%d",Num);
	LCD_vidSendString(str);
}

void ADC_Init(){
	DDRA = 0x00;
	/* Make ADC port as input */
	ADCSRA = 0x87;          /* Enable ADC, with freq/128  */
	ADMUX = 0x40;           /* Vref: Avcc, ADC channel: 0 */
}
int ADC_Read(char channel)
{
	ADMUX = 0x40 | (channel & 0x07);   /* set input channel to read */
	ADCSRA |= (1<<ADSC);               /* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));     /* Wait until end of conversion by polling ADC interrupt flag */ ADCSRA |= (1<<ADIF);               /* Clear interrupt flag */
	_delay_ms(1);                      /* Wait a little bit */
	return ADCW;                       /* Return ADC word */
}
int main()
{
	
	char Temperature[5];
	float celsius;
	LCD_vidInit();                 /* initialize 16x2 LCD*/
	ADC_Init();                 /* initialize ADC*/
	LCD_vidMoveCursor(1,1);
	LCD_vidSendString("Temp = ");
	while(1)
	{
		celsius = (ADC_Read(0)*4.88);
		celsius = (celsius/10.00);
		sprintf(Temperature,"%d%cC  ", (int)celsius, degree_sysmbol);/* convert integer value to ASCII string */ 
		LCD_vidMoveCursor(1,8);
		LCD_vidSendString(Temperature);/* send string data for printing */
		_delay_ms(50); memset(Temperature,0,5);
	}
}