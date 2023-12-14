#ifndef _HAL_LCD_PRIVATE_H_
#define _HAL_LCD_PRIVATE_H_

    #define LCD_Function_4BIT_2Lines   0x28
	#define LCD_Function_8BIT_2Lines   0x38

	//===================LCD Commands===================
		#define LCD_MOVE_DISP_RIGHT            0x1C
		#define LCD_MOVE_DISP_LEFT             0x18
		#define LCD_MOVE_CURSOR_RIGHT          0x14
		#define LCD_MOVE_CURSOR_LEFT           0x10
		#define LCD_DISP_OFF_CURSOR_OFF        0X08
		#define LCD_DISP_ON_CURSOR_ON          0x0E
		#define LCD_DISP_ON_CURSOR_BLINK       0x0F
		#define LCD_ENTRY_MODE				   0x06
		#define LCD_ENTRY_DEC				   0x04
		#define LCD_ENTRY_DEC_SHIFT			   0x05
		#define LCD_ENTRY_INC_SHIFT			   0x07
		#define LCD_BEGIN_AT_FIRST_RAW		   0x80
		#define LCD_BEGIN_AT_SECOND_RAW		   0xC0
		#define LCD_CLEAR_SCREEN			   0X01
		#define LCD_RETURN_HOME                0x02
	//=================================================

    static void LCD_vidSendEnablePulse();


#endif