#ifndef _HAL_LCD_INTERFACE_H_
#define _HAL_LCD_INTERFACE_H_
#include "DIO_interface.h"

void LCD_vidInit4bit();

void LCD_vidSendCmd4bit(u8 u8Cmd);

void LCD_vidDisplayChar4bit(u8 u8char);

void LCD_voidDisplayString4bit(u8 *ptru8String);

void LCD_vidClearScreen4bit();

void LCD_vidMoveCursor4bit(u8 u8Line, u8 u8Position);

void LCD_vidDisplayNumber4bit(s64 copy_u32Number);

void LCD_vidDisplayRealNumber4bit( s64 Number);

#endif