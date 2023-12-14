#include "Types.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"

#include "LCD_interface.h"
#include "LCD_config.h"
#include "LCD_private.h"

#define  F_CPU 8000000UL
#include "util/delay.h"

#include <stdio.h>


void LCD_vidInit()
{
    DIO_vidSetPortMode(enuDataPortNum, OUTPUT);
    DIO_vidSetPinMode(enuRSPortNum, enuRSPinNum, OUTPUT);    
    DIO_vidSetPinMode(enuENPortNum, enuENPinNum, OUTPUT);
    _delay_ms(200);
    LCD_vidSendCmd(  LCD_Function_8BIT_2Lines);
    _delay_ms(1);
    LCD_vidSendCmd(  LCD_DISP_ON_CURSOR_ON);
    _delay_ms(1);
    LCD_vidSendCmd(  LCD_CLEAR_SCREEN);
    _delay_ms(2);
    LCD_vidSendCmd(  LCD_ENTRY_MODE);
    _delay_ms(10);

    
}



void LCD_vidSendCmd( u8 u8Cmd)
{
    DIO_vidWritePin(enuRSPortNum, enuRSPinNum, LOW);
    DIO_vidWritePort(enuDataPortNum, u8Cmd);
    LCD_vidSendEnablePulse(enuENPortNum, enuENPinNum);
}

static void LCD_vidSendEnablePulse()
{
    DIO_vidWritePin(enuENPortNum, enuENPinNum, HIGH);
    _delay_ms(10);
    DIO_vidWritePin(enuENPortNum, enuENPinNum, LOW);
}

void LCD_vidDisplayChar( u8 u8char)
{
    DIO_vidWritePort(enuDataPortNum, u8char);
    DIO_vidWritePin(enuRSPortNum, enuRSPinNum, HIGH);
    LCD_vidSendEnablePulse(enuENPortNum, enuENPinNum);
}

void LCD_vidDisplayString( u8 *ptru8String)
{
	while((*ptru8String) != '\0')
	{
		LCD_vidDisplayChar(  *ptru8String);
		ptru8String++;
	}
}

void LCD_vidClearScreen( )
{
	LCD_vidSendCmd(  LCD_CLEAR_SCREEN);
	_delay_ms(10);
}

void LCD_vidMoveCursor(  u8 u8Line, u8 u8Position)
{
	u8 u8data =0;
	if(u8Line < 1 || u8Line > 2 || u8Position < 1 || u8Position > 16)
	{
		u8data = 0x80;
	}
	else if(u8Line == 1)
	{
		u8data = 0x80 + (u8Position - 1);
	}
	else if(u8Line == 2)
	{
		u8data = 0xC0 + (u8Position - 1);
	}
	LCD_vidSendCmd(  u8data);
	_delay_ms(1);
}

void LCD_vidDisplayNumber(  u32 copy_u32Number)
{
	u8 str[7];
	sprintf(str,"%d",copy_u32Number);
	LCD_vidDisplayString(  str);
}

void LCD_vidDisplayRealNumber(  double Number)
{
	u8 str [16];
	u8 *tmpsign = (Number < 0)? "-" : "";
	float tmpval = (Number < 0)? -Number : Number;
	u16 tmpint1 = (u16)tmpval;
	float tmpfrac = tmpval - (float)tmpint1;
	u16 tmpint2 = tmpfrac * 10000;
	sprintf(str,"%s%d.%04d", tmpsign, tmpint1, tmpint2);
	LCD_vidDisplayString(  str);
}


void LCD_vidInit4bit()
{
	_delay_ms(100);
	DIO_vidSetPinMode(enuRSPortNum, enuRSPinNum, OUTPUT);    
   	DIO_vidSetPinMode(enuENPortNum, enuENPinNum, OUTPUT);

	DIO_vidSetHighNibbleMode(enuDataPortNum, OUTPUT);
	LCD_vidSendCmd4bit( LCD_RETURN_HOME);
	_delay_ms(15);
	LCD_vidSendCmd4bit( LCD_Function_4BIT_2Lines);
	_delay_ms(1);
	LCD_vidSendCmd4bit( LCD_DISP_ON_CURSOR_BLINK);
	_delay_ms(1);
	LCD_vidSendCmd4bit( LCD_CLEAR_SCREEN);
	_delay_ms(10);
	LCD_vidSendCmd4bit( LCD_ENTRY_MODE);
	_delay_ms(1);
	
}

void LCD_vidSendCmd4bit( u8 u8Cmd)
{
		DIO_vidWriteHighNibble(enuDataPortNum, u8Cmd);
		DIO_vidWritePin(enuRSPortNum, enuRSPinNum, LOW);
		LCD_vidSendEnablePulse(enuENPortNum, enuENPinNum);
		DIO_vidWriteHighNibble(enuDataPortNum, u8Cmd<<4);
		DIO_vidWritePin(enuRSPortNum, enuRSPinNum, LOW);
		LCD_vidSendEnablePulse(enuENPortNum, enuENPinNum);
		_delay_ms(2);
}

void LCD_vidDisplayChar4bit( u8 u8char)
{

	DIO_vidWriteHighNibble(enuDataPortNum, u8char);
	DIO_vidWritePin(enuRSPortNum, enuRSPinNum, HIGH);
	LCD_vidSendEnablePulse(enuENPortNum, enuENPinNum);
	DIO_vidWriteHighNibble(enuDataPortNum, u8char<<4);
	DIO_vidWritePin(enuRSPortNum, enuRSPinNum, HIGH);
	LCD_vidSendEnablePulse(enuENPortNum, enuENPinNum);
	_delay_ms(2);
	
}

void LCD_voidDisplayString4bit( u8 *ptru8String)
{
	while((*ptru8String) != '\0')
	{
		LCD_vidDisplayChar4bit(*ptru8String);
		ptru8String++;
	}
}

void LCD_vidClearScreen4bit( )
{
	LCD_vidSendCmd4bit(LCD_CLEAR_SCREEN);
	_delay_ms(10);
}

void LCD_vidMoveCursor4bit(u8 u8Line, u8 u8Position)
{
	u8 u8data =0;
	if(u8Line < 1 || u8Line > 2 || u8Position < 1 || u8Position > 16)
	{
		u8data = 0x80;
	}
	else if(u8Line == 1)
	{
		u8data = 0x80 + (u8Position - 1);
	}
	else if(u8Line == 2)
	{
		u8data = 0xC0 + (u8Position - 1);
	}
	LCD_vidSendCmd4bit( u8data);
	_delay_ms(1);
}

void LCD_vidDisplayNumber4bit(s64 copy_u32Number)
{
	u8 str[11];
	sprintf(str,"%d",copy_u32Number);
	LCD_voidDisplayString4bit(str);
}

void LCD_vidDisplayRealNumber4bit(s64 Number)
{
	u8 str [16];
	u8 *tmpsign = (Number < 0)? "-" : "";
	u32 tmpval = (Number < 0)? -Number : Number;
	sprintf(str,"%s%d", tmpsign, tmpval);
	LCD_voidDisplayString4bit(str);
}