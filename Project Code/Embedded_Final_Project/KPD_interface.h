#ifndef _HAL_KPD_INTERFACE_H_
#define _HAL_KPD_INTERFACE_H_

#include "DIO_interface.h"
#define NOT_PRESSRD 0xFF
void KPD_vidInit(_enuGPIOX enuPortNum, _enuNIBBLE enuOutNibble, _enuNIBBLE enuInNibble);
u8 KPD_u8GetPressedKey(_enuGPIOX enuPortNum, _enuNIBBLE enuOutNibble, _enuNIBBLE enuInNibble);

#endif