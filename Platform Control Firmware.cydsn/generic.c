/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       generic.h                                                     ///
/// Function:   4DShoot Controller General Purpose Operations                 ///
/// Date:       28 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 28/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#include "generic.h"                                                          ///
/////////////////////////////////////////////////////////////////////////////////

// Convert array of characters to unsigned integer
uint8_t cArray_toUnsigned(const char a[], uint8_t length)
{
    uint8_t index;
    uint8_t power;
    uint8_t value = 0;
    uint8_t temp = 0;
    for(index = 0; index < length; index++)
    {
        temp = a[index] - '0';
        for(power = 1; power < length - index; power++)
            temp = temp * 10;
        value = value + temp;
    }    
    return value;
}

// Set motor PWM duty cycle, direction
// n - Motor number:        spd: Speed          dir: Direction
//      1: Left Front           0 -> 100            0:  Short Brake
//      2: Right Front                              1:  Forward
//      3: Right Rear                               -1: Reverse
//      4: Left Rear
void setSpeed(uint8_t n, uint8_t spd, int8_t dir)
{  
    uint8_t direction = 0b00000000;
    if      (dir == -1) {direction = 0b00000010;}
    else if (dir == 1)  {direction = 0b00000001;}
    else                {direction = 0b00000011;}
    
    switch(n)
    {
        case 1:  { Direction_Left_Front_Write(direction & 0x03);
                   PWM_Left_Front_WriteCompare(spd);
                   break; }        
        case 2:  { Direction_Right_Front_Write(direction & 0x03);
                   PWM_Right_Front_WriteCompare(spd);
                   break; }        
        case 3:  { Direction_Right_Rear_Write(direction & 0x03);
                   PWM_Right_Rear_WriteCompare(spd);
                   break; }        
        case 4:  { Direction_Left_Rear_Write(direction & 0x03);
                   PWM_Left_Rear_WriteCompare(spd);
                   break; }        
        default: { Direction_Left_Front_Write(0x03);
                   Direction_Right_Front_Write(0x03);
                   Direction_Left_Rear_Write(0x03);
                   Direction_Right_Rear_Write(0x03);
                   PWM_Left_Front_WriteCompare(0);
                   PWM_Right_Front_WriteCompare(0);
                   PWM_Right_Rear_WriteCompare(0);
                   PWM_Left_Rear_WriteCompare(0);
                   break; }
    }
    return;    
}

uint16_t atan_Approx(int16_t x, int16_t y)
{
	uint16_t heading = 0;
	uint16_t vector = 0;
	if (x != 0 && y != 0)
	{
		float z = y * 1.0 / x;
		float b = 0.596227;
		if (z < 0) { z = -z; }
		//vector = 896 * (b*z + z*z) / (1 + 2 * b*z + z*z);
        vector = (900) * (b*z + z*z) / (1 + 2*b*z + z*z);
	}
	
	if (y == 0 && x == 0)
	{
		heading = 0;
	}
	else if (y == 0 && x > 0)
	{
		heading = 0;
	}
	else if (y > 0 && x > 0)
	{
		heading = (uint16_t)vector;
	}
	else if (y > 0 && x == 0)
	{
		heading = 900;
	}
	else if (y > 0 && x < 0)
	{
		heading = 1800 - (uint16_t)vector;
	}
	else if (y == 0 && x < 0)
	{
		heading = 1800;
	}
	else if (y < 0 && x < 0)
	{
		heading = 1800 + (uint16_t)vector;
	}
	else if (y < 0 && x == 0)
	{
		heading = 2700;
	}
	else if (y < 0 && x > 0)
	{
		heading = 3600 - (uint16_t)vector;
	}

	return heading;
}