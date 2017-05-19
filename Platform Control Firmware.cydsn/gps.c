/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       gps.c                                                         ///
/// Function:   4DShoot Controller GPS Functions                              ///
/// Date:       30 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 01/28/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#include "gps.h"                                                              ///
/////////////////////////////////////////////////////////////////////////////////

// $GPRMC,231608.000,A,4207.1584,N,07958.8390,W,0.26,329.64,210417,,,A*75
// $GPRMC,021840.000,A,4207.1292,N,07958.8003,W,0.37,173.91,220417,,,A*79
// 0         1         2         3         4         5         6         7 

struct GPS_Datagram GPS_Parse(const char data[])
{
    struct GPS_Datagram working;
    
    strcpy(working.raw, data);
    
    if(!strncmp(working.raw, "$GPRMC", 6))
    {
        if(working.raw[18] == 'A')
        {
            strncpy(working.latitudeStr + 1, data + 20, 9);
            working.latitudeStr[0] = '0';
            working.latitudeStr[10] = '\0';
            working.latitudePol[0] = data[30];
            working.latitudePol[1] = '\0';        
            strncpy(working.longitudeStr, data + 32, 10);
            working.longitudeStr[10] = '\0';
            working.longitudePol[0] = data[43];
            working.longitudePol[1] = '\0';    
            working.valid = 1;
        }
        else
        {
            strcpy(working.latitudeStr, "00000000\0");
            strcpy(working.longitudeStr, "00000000\0");
            working.latitudePol[0] = '0';
            working.latitudePol[1] = '\0';
            working.longitudePol[0] = '0';
            working.longitudePol[1] = '\0';        
            working.valid = 0;
            working.raw[0] = '\0';
        }
    }
    return working;
}

void GPS_Init()
{
    // Refresh 500ms
    GPS_UART_PutString("$PMTK220,500*2B\r\n");
    // Disable all output except RMC string
    GPS_UART_PutString("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n");
    return;
}