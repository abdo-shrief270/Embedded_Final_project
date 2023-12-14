#include "Types.h"
#include "BIT_MATH.h"

#include "KPD_private.h"
#include "KPD_interface.h"
#include "KPD_config.h"



void KPD_vidInit(_enuGPIOX enuPortNum, _enuNIBBLE enuOutNibble, _enuNIBBLE enuInNibble)
{
    if (enuOutNibble == LOW_NIBBLE)
    {
        DIO_vidSetLowNibbleMode(enuPortNum, OUTPUT);
        DIO_vidSetHighNibbleMode(enuPortNum, INPUT);
        DIO_vidWritePort(enuPortNum, 0xF0);
    }
    else
    {
        DIO_vidSetLowNibbleMode(enuPortNum, INPUT);
        DIO_vidSetHighNibbleMode(enuPortNum, OUTPUT);
        DIO_vidWriteLowNibble(enuPortNum, 0x0F);
    }
    
}
u8 KPD_u8GetPressedKey(_enuGPIOX enuPortNum, _enuNIBBLE enuOutNibble, _enuNIBBLE enuInNibble)
{
    u8 arr[4][4]={{'1','2','3','%'},
                  {'4','5','6','*'},
                  {'7','8','9','-'},
                  {'C','0','=','+'}};
	u8 u8Row , u8Coloumn ;
    _enuOUTPUT enuReadValue;
	u8 Return_val= NOT_PRESSRD;
    if (enuOutNibble == LOW_NIBBLE)
    {
        for (u8Row = 0; u8Row < 4; u8Row++)
        {
            DIO_vidWriteLowNibble(enuPortNum, 0x0F);
            DIO_vidWritePin(enuPortNum, u8Row, LOW);
            for (u8Coloumn = 0; u8Coloumn < 4; u8Coloumn++)
            {
                enuReadValue = DIO_enuReadPin(enuPortNum,(u8Coloumn+4));
                if (enuReadValue == LOW)
                {
                    Return_val = arr[u8Row][u8Coloumn];
                    break;
                }
				else
				{
					Return_val = 0xFF;
				}
                
            }
            if (enuReadValue == LOW)
            {
                break;
            }  
        }
    }
    else
    {
       for (u8Row = 0; u8Row < 4; u8Row++)
        {
            DIO_vidWriteHighNibble(enuPortNum, 0xF0);
            DIO_vidWritePin(enuPortNum, (u8Row+4), LOW);
            for (u8Coloumn = 0; u8Coloumn < 4; u8Coloumn++)
            {
                enuReadValue = DIO_enuReadPin(enuPortNum,(u8Coloumn));
                if (enuReadValue == LOW)
                {
                    Return_val = arr[u8Row][u8Coloumn];
                    break;
                }
                
            }
            if (enuReadValue == LOW)
            {
                break;
            }  
        } 
    }
    return Return_val;
    
}