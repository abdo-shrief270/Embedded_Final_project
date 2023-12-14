#ifndef _HAL_LCD_INTERFACE_H_
#define _HAL_LCD_INTERFACE_H_
#include "DIO_interface.h"

void LCD_vidInit( );

void LCD_vidSendCmd(  u8 u8Cmd);

void LCD_vidDisplayChar(  u8 u8char);

void LCD_voidDisplayString(  u8 *ptru8String);

void LCD_vidClearScreen( );

void LCD_vidMoveCursor(  u8 u8Line, u8 u8Position);

void LCD_vidDisplayNumber(  u32 copy_u32Number);

void LCD_vidDisplayRealNumber(  double Number);


void LCD_vidInit4bit();

void LCD_vidSendCmd4bit(u8 u8Cmd);

void LCD_vidDisplayChar4bit(u8 u8char);

void LCD_voidDisplayString4bit(u8 *ptru8String);

void LCD_vidClearScreen4bit();

void LCD_vidMoveCursor4bit(u8 u8Line, u8 u8Position);

void LCD_vidDisplayNumber4bit(s64 copy_u32Number);

void LCD_vidDisplayRealNumber4bit( s64 Number);

#endif