/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       lsm303.c                                                      ///
/// Function:   4DShoot Controller LSM303 Compass/Accelerometer               ///
/// Date:       30 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 30/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LSM303_H                                                             ///
#define _LSM303_H                                                             ///
#include <stdint.h>                                                           ///
#include "project.h"                                                          ///
#include "generic.h"                                                          ///
/////////////////////////////////////////////////////////////////////////////////    

#define LSM303_MAGNETOMETER 0b0011110
#define LSM303_ACCELEROMETER 0b0011001
    
struct LSM303_Datagram
{
    int16_t x;
    int16_t y;
    int16_t z;
    uint16_t heading;
};
        
struct LSM303_Datagram LSM303_Mag_Read();
struct LSM303_Datagram LSM303_Acc_Read();
void LSM303_Init();
void LSM303_WriteByte(uint8_t dev, uint8_t addr, uint8_t data);
uint8_t LSM303_ReadByte(uint8_t dev, uint8_t addr);
struct LSM303_Datagram LSM303_Refresh();

/////////////////////////////////////////////////////////////////////////////////
#endif                                                                        ///
/////////////////////////////////////////////////////////////////////////////////

