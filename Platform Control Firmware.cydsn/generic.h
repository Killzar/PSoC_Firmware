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
/// Function:   4DShoot Controller General Purpose Definitions Header         ///
/// Date:       28 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 28/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#ifndef _GENERIC_H                                                            ///
#define _GENERIC_H                                                            ///
#include <stdint.h>                                                           ///
#include "project.h"                                                          ///
#include "lsm303.h"                                                           ///
/////////////////////////////////////////////////////////////////////////////////
    
// Function declarations
uint8_t cArray_toUnsigned(const char a[], uint8_t length);
void setSpeed(uint8_t n, uint8_t spd, int8_t dir);
uint16_t atan_Approx(int16_t x, int16_t y);

/////////////////////////////////////////////////////////////////////////////////
#endif                                                                        ///
/////////////////////////////////////////////////////////////////////////////////

