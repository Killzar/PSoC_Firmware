/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       esp07.c                                                       ///
/// Function:   4DShoot Controller ESP07 Wireless Firmware                    ///
/// Date:       28 January 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 28/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
#include "esp07.h"                                                            ///
/////////////////////////////////////////////////////////////////////////////////

void ESP07_Boot(void)
{
    ESP07_UART_PutString("AT+RST\r\n");
    CyDelay(500);

    ESP07_UART_PutString("AT\r\n");
    CyDelay(50);
    
    ESP07_UART_PutString("ATE0\r\n");
    CyDelay(50);
    
    ESP07_UART_PutString("AT+CIPMODE=0\r\n");
    CyDelay(50);
    
    ESP07_UART_PutString("AT+CIPMUX=1\r\n");
    CyDelay(50);
    
    ESP07_UART_PutString("AT+CWSAP=\"4DShoot Prototype\",\"hhwwguak\",1,3,4,0\r\n");
    CyDelay(50);
    
    ESP07_UART_PutString("AT+CIPSERVER=1,80\r\n");
    CyDelay(50);
    
    return;
}

struct ESP07_Datagram ESP07_Parse(const char data[])
{
    struct ESP07_Datagram working;
    
    // Copy raw string to datagram object
    strcpy(working.raw, data);
    working.length_raw = strlen(data);
    
    // Check for +IPD header substring
    if(strstr(data, "+IPD,") != NULL)
    {   
        // Get payload substring, length after ':'
        working.length_payload = (data + strlen(data)) - strchr(data, ':') - 1;
        strcpy(working.payload, strchr(data, ':') + 1);
    }
    else
    {
        strcpy(working.payload, data);
        working.length_payload = strlen(data);
    }
    
    // Get command identifier
    if      (!strcmp(working.payload, ""))
        working.id = 1;
    else if (!strcmp(working.payload, "LED ON"))
        working.id = 2;
    else if (!strcmp(working.payload, "LED OFF"))
        working.id = 3;
    else if (!strcmp(working.payload, "OK"))
        working.id = 4;
    else if (!strcmp(working.payload, "ready"))
        working.id = 5;
    else if (!strcmp(working.payload, "> "))
        working.id = 6;
    else if (!strncmp(working.payload, "Recv", 4))
        working.id = 7;
    else if (!strcmp(working.payload, "SEND OK"))
        working.id = 8;
    else if (!strcmp(working.payload, "ERROR"))
        working.id = 9;
    else if (!strcmp(working.payload, "0,CONNECT"))
        working.id = 10;
    else if (!strcmp(working.payload, "0,CLOSED"))
        working.id = 11;
    else if (!strcmp(working.payload, "DUMP"))
        working.id = 12;
    else if (!strcmp(working.payload, "0,CONNECT FAIL"))
        working.id = 13;
    else if (!strcmp(working.payload, "RESET"))
        working.id = 14;
    else if (!strncmp(working.payload, "LFT_FWD", 7))
        working.id = 16;
    else if (!strncmp(working.payload, "LFT_BCK", 7))
        working.id = 17;
    else if (!strncmp(working.payload, "RGT_FWD", 7))
        working.id = 18;
    else if (!strncmp(working.payload, "RGT_BCK", 7))
        working.id = 19;
    else if (!strncmp(working.payload, "ALL_FWD", 7))
        working.id = 20;
    else if (!strncmp(working.payload, "ALL_BCK", 7))
        working.id = 21;
    else if (!strncmp(working.payload, "ROTATE", 6))
        working.id = 22;
    else if (!strncmp(working.payload, "MOVE", 4)) //Uses Command - Speed - Distance
        working.id = 23;
    else if (!strcmp(working.payload, "COORD"))
        working.id = 24;
    else if (!strcmp(working.payload, "HEADING"))
        working.id = 25;
    else if (!strncmp(working.payload, "NPOLYG", 6))
        working.id = 26;
    else if (!strcmp(working.payload, "STOP"))
        working.id = 27;
    else
        working.id = 0;   

    return working;
}

void ESP07_SendAck()
{
    ESP07_UART_PutString("AT+CIPSEND=0,12\r\n");
    CyDelay(20);
    ESP07_UART_PutString("ESP07> ACK\r\n");
    return;
}