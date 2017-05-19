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
///             0.A - 01/28/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#include "lsm303.h"                                                           ///
/////////////////////////////////////////////////////////////////////////////////

void LSM303_Init()
{
    // CRA_REG_M
    // Magnetometer Temp Disable, 75Hz
    LSM303_WriteByte(LSM303_MAGNETOMETER, 0x00, 0b00011100);
    
    // CRB_REG_M
    // Magnetometer Gain Control
    LSM303_WriteByte(LSM303_MAGNETOMETER, 0x01, 0b00000000);
    
    // MR_REG_M
    // Magnetometer Continuous Operation Mode
    LSM303_WriteByte(LSM303_MAGNETOMETER, 0x02, 0b00000000);
    
    // CTRL_REG1_A
    // Accelerometer 400Hz Refresh, Enable X, Y, Z Axis
    LSM303_WriteByte(LSM303_ACCELEROMETER, 0x20, 0b01110111);
    return;
}

void LSM303_WriteByte(uint8_t dev, uint8_t addr, uint8_t data)
{
    I2C_MasterSendStart(dev, I2C_WRITE_XFER_MODE);
    I2C_MasterWriteByte(addr);
    I2C_MasterWriteByte(data);
    I2C_MasterSendStop();
    return;
}

uint8_t LSM303_ReadByte(uint8_t dev, uint8_t addr)
{
    uint8_t data;
    I2C_MasterSendStart(dev, I2C_WRITE_XFER_MODE);
    I2C_MasterWriteByte(addr);
    I2C_MasterSendRestart(dev, I2C_READ_XFER_MODE);
    data = I2C_MasterReadByte(I2C_NAK_DATA);
    I2C_MasterSendStop();
    return data;
}

struct LSM303_Datagram LSM303_Refresh()
{
    struct LSM303_Datagram working;
    struct LSM303_Datagram filter[8];
    
    int16_t xUpper, xLower;
    int16_t yUpper, yLower;
    int16_t zUpper, zLower;
    
    uint8_t index;
    for(index = 0; index < 8; index++)
    {        
        xUpper = LSM303_ReadByte(LSM303_MAGNETOMETER, 0x03);
        xLower = LSM303_ReadByte(LSM303_MAGNETOMETER, 0x04);
        zUpper = LSM303_ReadByte(LSM303_MAGNETOMETER, 0x05);
        zLower = LSM303_ReadByte(LSM303_MAGNETOMETER, 0x06);
        yUpper = LSM303_ReadByte(LSM303_MAGNETOMETER, 0x07);
        yLower = LSM303_ReadByte(LSM303_MAGNETOMETER, 0x08);
        
        filter[index].x = (xUpper << 8) | (xLower & 0x00FF);
        filter[index].y = (yUpper << 8) | (yLower & 0x00FF);
        filter[index].z = (zUpper << 8) | (zLower & 0x00FF);
    }
    
    working.x = (filter[0].x + filter[1].x + filter[2].x + filter[3].x +
                 filter[4].x + filter[5].x + filter[6].x + filter[7].x) >> 3;
    
    working.y = (filter[0].y + filter[1].y + filter[2].y + filter[3].y +
                 filter[4].y + filter[5].y + filter[6].y + filter[7].y) >> 3;
    
    working.z = (filter[0].z + filter[1].z + filter[2].z + filter[3].z +
                 filter[4].z + filter[5].z + filter[6].z + filter[7].z) >> 3;
    
    working.heading = atan_Approx(working.x,working.y);
    
    return working;
}
    