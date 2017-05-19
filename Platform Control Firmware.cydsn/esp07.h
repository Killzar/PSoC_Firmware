/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       esp07.h                                                       ///
/// Function:   4DShoot Controller ESP07 Wireless Header                      ///
/// Date:       28 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 28/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
#ifndef _ESP07_H                                                              ///
#define _ESP07_H                                                              ///
#include <stdint.h>                                                           ///
#include "project.h"                                                          ///
#include "generic.h"                                                          ///
/////////////////////////////////////////////////////////////////////////////////

// Definitions, globals
#define ESP07_MAX_PAYLOAD 64

static char ESP07_Data[ESP07_MAX_PAYLOAD] = {};
static uint8_t ESP07_Ready = 0;
static uint8_t ESP07_Connected = 0;

// Incoming message data storage object
struct ESP07_Datagram
{
    char payload[ESP07_MAX_PAYLOAD];
    char raw[ESP07_MAX_PAYLOAD];
    uint8_t id;
    uint8_t length_payload;
    uint8_t length_raw;
};

// Function declarations
void ESP07_Boot(void);
struct ESP07_Datagram ESP07_Parse(const char data[]);
void ESP07_SendAck();

/////////////////////////////////////////////////////////////////////////////////
#endif                                                                        ///
/////////////////////////////////////////////////////////////////////////////////

