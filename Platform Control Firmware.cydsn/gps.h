/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       gps.h                                                         ///
/// Function:   4DShoot Controller GPS Header                                 ///
/// Date:       28 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 28/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
#ifndef _GPS_H                                                                ///
#define _GPS_H                                                                ///
#include <stdint.h>                                                           ///
#include "project.h"                                                          ///
#include "generic.h"                                                          ///
/////////////////////////////////////////////////////////////////////////////////
    
    
// Definitions, globals
static char GPS_GPRMC[74] = {};
static uint8_t GPS_Ready = 0;

// Incoming message data storage object
struct GPS_Datagram
{
    char raw[74];
    uint8_t valid; // raw[18] - 1: VALID
    char latitudeStr[11];
    char latitudePol[2];
    char longitudeStr[11];
    char longitudePol[2];
};

// Function declarations
struct GPS_Datagram GPS_Parse(const char data[]);
void GPS_Init();

/////////////////////////////////////////////////////////////////////////////////
#endif                                                                        ///
/////////////////////////////////////////////////////////////////////////////////