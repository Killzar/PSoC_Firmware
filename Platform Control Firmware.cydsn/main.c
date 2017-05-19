/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/// Copyright Chad Hensley 2017                                               ///
/// All Rights Reserved                                                       ///
/// UNPUBLISHED, LICENSED SOFTWARE.                                           ///
///                                                                           ///
/// CONFIDENTIAL AND PROPRIETARY INFORMATION                                  ///
/// WHICH IS THE PROPERTY OF THE COPYRIGHT HOLDER                             ///
///                                                                           ///
/// File:       main.c                                                        ///
/// Function:   4DShoot Controller Primary Firmware                           ///
/// Date:       2 January 2017                                                ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 02/01/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "project.h"
#include "generic.h"

// Peripheral libraries
#include "esp07.h"  // Wireless module
#include "lsm303.h" // Magnetometer/accelerometer
#include "gps.h"    // Linx RMX GPS module

// Definitions
#define RADIO_ACTIVE_STATUS (scaledCounts > 30 || scaledCounts < -30)
#define RADIO_OVERRIDE (radioActiveLeft == 1 || radioActiveRight == 1)
#define SYSTEM_HALT (radioActiveLeft == 1 || radioActiveRight == 1 || ESP07_Connected == 0 || ESP07_Ready == 1)
#define FORWARD 1
#define REVERSE -1
#define BRAKE 0
#define TRUE 1
#define FALSE 0

// Global objects
uint8_t radioActiveLeft = 0;            // Radio activity signals
uint8_t radioActiveRight = 0;           // ////
char BUFFER_TEMP[64];                   // Temporary sprintf() storage buffer

void System_Init(void);
void Run_CMD(void);
void Run_Radio(void);
void RotateCCW(uint16_t angle, uint8_t speed);
void RotateCW(uint16_t angle, uint8_t speed);
void eRotateCCW(uint16_t angle, uint8_t speed);
void eRotateCW(uint16_t angle, uint8_t speed);

// Interrupt prototypes
CY_ISR_PROTO(ESP07_ISR);
CY_ISR_PROTO(Radio_Left_ISR);
CY_ISR_PROTO(Radio_Right_ISR);
CY_ISR_PROTO(GPS_ISR);

/////////////////////////////////////////////////////////////////////////////////
/// Main Function                                                             ///
/////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    CyDelay(500);         // Attempt bootload; enable interrupts
    CyGlobalIntEnable;    // and init peripherals
    System_Init();        ////

    for(;;)
    {        
        if (!RADIO_OVERRIDE)
        {
            Run_CMD();
            
            /*
            if(SYSTEM_HALT)
            {
                setSpeed(1, 0, BRAKE);
                setSpeed(2, 0, BRAKE);
                setSpeed(3, 0, BRAKE);
                setSpeed(4, 0, BRAKE);
            }            
            */
        }        
    } /* for(;;) */
}

CY_ISR(ESP07_ISR)
{
    ESP07_UART_ReadRxStatus();
    ESP07_Interrupt_ClearPending();
    
    static uint8_t index = 0;
    
    char incoming = ESP07_UART_GetChar();
        
    if      (incoming == '\r')
    {
        // Carriage return ignored
    }
    else if (incoming == '\n')
    {
        // Linefeed (CTRL+J); CR = 0x0D (CTRL+M)
        // Terminate and reset
        ESP07_Data[index] = '\0';
        if(strncmp(ESP07_Data, "+IPD,0,2:", 9) != 0) // if not "\r\n"
            ESP07_Ready = 1;
        index = 0;
    }
    else
    {   
        // Increment index and continue
        ESP07_Data[index] = incoming;
        
        if(index != ESP07_MAX_PAYLOAD)
            index++;
    }

    return;
}

CY_ISR(Radio_Left_ISR)
{
    Interrupt_Channel_Left_ClearPending();
    Timer_Channel_Left_ReadStatusRegister();
    
    if(RX_PWM_Left_Read() == 0) // falling edge capture
    {
        int16_t rawCounts = 65535 - Timer_Channel_Left_ReadCapture();
        int16_t scaledCounts = (rawCounts - 1600) / 3;
        
        if(scaledCounts < -100) scaledCounts = -100;
        if(scaledCounts > 100) scaledCounts = 100;
        
        if(RADIO_ACTIVE_STATUS && scaledCounts > 0)
        {
            radioActiveLeft = 1;
            setSpeed(1, scaledCounts, FORWARD);
            setSpeed(4, scaledCounts, FORWARD);
        }
        else if(RADIO_ACTIVE_STATUS && scaledCounts < 0)
        {
            scaledCounts = scaledCounts * -1;
            radioActiveLeft = 1;
            setSpeed(1, scaledCounts, REVERSE);
            setSpeed(4, scaledCounts, REVERSE);
        }
        else
        {
            if (radioActiveLeft == 1)
            {
                radioActiveLeft = 0;
                setSpeed(1, 0, BRAKE);
                setSpeed(4, 0, BRAKE);
            }
        }
    }
    else
        Timer_Channel_Left_WriteCounter(65535);
    return;                
}

CY_ISR(Radio_Right_ISR)
{
    Interrupt_Channel_Right_ClearPending();
    Timer_Channel_Right_ReadStatusRegister();

    if(RX_PWM_Right_Read() == 0) // falling edge capture
    {
        int16_t rawCounts = 65535 - Timer_Channel_Right_ReadCapture();
        int16_t scaledCounts = (rawCounts - 1600) / 3;
        
        if(scaledCounts < -100) scaledCounts = -100;
        if(scaledCounts > 100) scaledCounts = 100;
        
        if(RADIO_ACTIVE_STATUS && scaledCounts > 0)
        {
            radioActiveRight = 1;
            setSpeed(2, scaledCounts, FORWARD);
            setSpeed(3, scaledCounts, FORWARD);
        }
        else if(RADIO_ACTIVE_STATUS && scaledCounts < 0)
        {
            scaledCounts = scaledCounts * -1;
            radioActiveRight = 1;
            setSpeed(2, scaledCounts, REVERSE);
            setSpeed(3, scaledCounts, REVERSE);
        }
        else
        {
            if (radioActiveRight == 1)
            {
                radioActiveRight = 0;
                setSpeed(2, 0, BRAKE);
                setSpeed(3, 0, BRAKE);
            }
        }
    }
    else
        Timer_Channel_Right_WriteCounter(65535);
    return;                
}

CY_ISR(GPS_ISR)
{   
    GPS_UART_ReadRxStatus();
    GPS_Interrupt_ClearPending();
    
    static uint8_t index = 0;
    
    char incoming = GPS_UART_GetChar();

    if      (incoming == '\r')
    {
        // Carriage return ignored
    }
    else if (incoming == '\n')
    {
        // Linefeed (CTRL+J); CR = 0x0D (CTRL+M)
        // Terminate and reset
        GPS_GPRMC[index] = '\0';
        GPS_Ready = 1;
        index = 0;
    }
    else
    {   
        // Increment index and continue
        GPS_GPRMC[index] = incoming;
        index++;
    }

    return; 
}

void System_Init(void)
{
    FT232_UART_PutString("Starting hardware peripherals...\r\n");
    Onboard_LED_Write(1);
    ESP07_UART_Start();
    FT232_UART_Start();
    GPS_UART_Start();
    I2C_Start();
    Timer_Channel_Left_Start();
    Timer_Channel_Right_Start();
    PWM_Left_Front_Start();
    PWM_Left_Rear_Start();
    PWM_Right_Front_Start();
    PWM_Right_Rear_Start();
    Decoder_Left_Start();
    Decoder_Right_Start();
    
    ESP07_Interrupt_StartEx(ESP07_ISR);
    Interrupt_Channel_Left_StartEx(Radio_Left_ISR);
    Interrupt_Channel_Right_StartEx(Radio_Right_ISR);
    GPS_Interrupt_StartEx(GPS_ISR);
   
    FT232_UART_PutString("Done. Init ESP07 module...\r\n");
    ESP07_Boot();
    FT232_UART_PutString("Done. Init LSM303 module...\r\n");
    LSM303_Init();
    FT232_UART_PutString("Done.  Init GPS module...\r\n");
    GPS_Init();
    
    Onboard_LED_Write(0);
    FT232_UART_PutString("Done. Boot sequence complete.\r\n");
    return;
}

void Run_CMD(void)
{
    if (ESP07_Ready == 1)
    {
        ESP07_Ready = 0;
        struct ESP07_Datagram command = ESP07_Parse(ESP07_Data);
        switch(command.id)
        {
            // Unknown command
            case 0:  { FT232_UART_PutString("Unknown command: ");
                       FT232_UART_PutString(command.payload);
                       FT232_UART_PutString("\r\n");
                       break; }
            // Onboard LED enable
            case 2:  { Onboard_LED_Write(1);
                       FT232_UART_PutString("Onboard LED enabled.\r\n");
                       ESP07_SendAck();
                       break; }
            // Onboard LED disable
            case 3:  { Onboard_LED_Write(0);
                       FT232_UART_PutString("Onboard LED disabled.\r\n");
                       ESP07_SendAck();
                       break; }
            // Received 0,CONNECT (Connection established)
            case 10: { ESP07_Connected = 1;
                       ESP07_UART_PutString("AT+CIPSEND=0,16\r\n");
                       CyDelay(20);
                       ESP07_UART_PutString("ESP07> CONNECT\r\n");
                       FT232_UART_PutString("Connection 0 established.\r\n");
                       break; }
            // Received 0,CLOSE (Connection terminated)
            case 11: { ESP07_Connected = 0;
                       FT232_UART_PutString("Connection 0 terminated.\r\n");
                       break; }
            // Dump globals
            case 12: { struct LSM303_Datagram temp = LSM303_Refresh();
                       FT232_UART_PutString("Dumping...\r\n");
                       FT232_UART_PutString("  command.raw:            ");
                       FT232_UART_PutString(command.raw);
                       FT232_UART_PutString("\r\n  ESP07_Data[]:           ");
                       FT232_UART_PutString(ESP07_Data);
                       FT232_UART_PutString("\r\n  LSM303 Heading:         ");
                       sprintf(BUFFER_TEMP, "%d", temp.heading);
                       FT232_UART_PutString(BUFFER_TEMP);
                       FT232_UART_PutString("\r\n  LSM303 X-Magnitude:     ");
                       sprintf(BUFFER_TEMP, "%d", temp.x);
                       FT232_UART_PutString(BUFFER_TEMP);
                       FT232_UART_PutString("\r\n  LSM303 Y-Magnitude:     ");
                       sprintf(BUFFER_TEMP, "%d", temp.y);
                       FT232_UART_PutString(BUFFER_TEMP);
                       FT232_UART_PutString("\r\n  Left Decoder:           ");
                       sprintf(BUFFER_TEMP, "%ld", Decoder_Left_GetCounter());
                       FT232_UART_PutString(BUFFER_TEMP);
                       Decoder_Left_SetCounter(0);
                       FT232_UART_PutString("\r\n  Right Decoder:          ");
                       sprintf(BUFFER_TEMP, "%ld", Decoder_Right_GetCounter());
                       FT232_UART_PutString(BUFFER_TEMP);
                       Decoder_Right_SetCounter(0);
                       FT232_UART_PutString("\r\n  ESP07_Connected:         ");
                       FT232_UART_PutChar(ESP07_Connected + '0');
                       FT232_UART_PutString("\r\n  ESP07_Ready:             ");
                       FT232_UART_PutChar(ESP07_Ready + '0');
                       FT232_UART_PutString("\r\n  radioActiveLeft:         ");
                       FT232_UART_PutChar(radioActiveLeft + '0');
                       FT232_UART_PutString("\r\n  radioActiveRight:        ");
                       FT232_UART_PutChar(radioActiveRight + '0');
                       FT232_UART_PutString("\r\n");
                       ESP07_SendAck();
                       break; }
            case 13: { ESP07_Connected = 0;
                       FT232_UART_PutString("Connection 0 failed.\r\n");
                       break; }
            case 14: { FT232_UART_PutString("Software reset called.\r\n");
                       FT232_UART_PutString("Dropping to bootloader...\r\n");
                       CyDelay(10);
                       CySoftwareReset();
                       break; }
            case 16: { char* speedString = command.payload + 8;
                       setSpeed(1, (uint8_t)atoi(speedString), FORWARD);
                       setSpeed(4, (uint8_t)atoi(speedString), FORWARD);
                       break; }
            case 17: { char* speedString = command.payload + 8;
                       setSpeed(1, (uint8_t)atoi(speedString), REVERSE);
                       setSpeed(4, (uint8_t)atoi(speedString), REVERSE);
                       break; }
            case 18: { char* speedString = command.payload + 8;
                       setSpeed(2, (uint8_t)atoi(speedString), FORWARD);
                       setSpeed(3, (uint8_t)atoi(speedString), FORWARD);
                       break; }
            case 19: { char* speedString = command.payload + 8;
                       setSpeed(2, (uint8_t)atoi(speedString), REVERSE);
                       setSpeed(3, (uint8_t)atoi(speedString), REVERSE);
                       break; }
            case 20: { char* speedString = command.payload + 8;
                       setSpeed(1, (uint8_t)atoi(speedString), FORWARD);
                       setSpeed(2, (uint8_t)atoi(speedString), FORWARD);
                       setSpeed(3, (uint8_t)atoi(speedString), FORWARD);
                       setSpeed(4, (uint8_t)atoi(speedString), FORWARD);
                       break; }
            case 21: { char* speedString = command.payload + 8;
                       setSpeed(1, (uint8_t)atoi(speedString), REVERSE);
                       setSpeed(2, (uint8_t)atoi(speedString), REVERSE);
                       setSpeed(3, (uint8_t)atoi(speedString), REVERSE);
                       setSpeed(4, (uint8_t)atoi(speedString), REVERSE);
                       break; }
            case 22: { char * speedAddr = command.payload + 7;
                       char * angleAddr = command.payload + 13;
                       char speedStr[4]; strncpy(speedStr, speedAddr, 3); speedStr[3] = '\0';
                       char direction = command.payload[11];
                       char angleStr[32]; strcpy(angleStr, angleAddr);
                       uint8_t speed = atoi(speedStr);
                       uint32_t angle = atoi(angleStr);
                       if(direction == 'L' || direction == 'l')
                           eRotateCCW(angle, speed);
                       if(direction == 'R' || direction == 'r')
                           eRotateCW(angle, speed);
                       ESP07_SendAck();
                       break; }
            case 23: { char * speedAddr = command.payload + 5;
                       char * distAddr = command.payload + 9;
                       char speedStr[4]; strncpy(speedStr, speedAddr, 3); speedStr[3] = '\0';
                       char distStr[32]; strcpy(distStr, distAddr);
                       uint8_t speed = atoi(speedStr);
                       int32_t dist = atoi(distStr);
                       int32_t avgDist = 0;
                       Decoder_Left_SetCounter(0);
                       Decoder_Right_SetCounter(0);
                       if(dist < 0)
                       {
                           setSpeed(1, speed, REVERSE);
                           setSpeed(2, speed, REVERSE);
                           setSpeed(3, speed, REVERSE);
                           setSpeed(4, speed, REVERSE);
                           while((avgDist > (dist * 491) / 65) && !SYSTEM_HALT)
                           {
                               avgDist = (Decoder_Left_GetCounter()+Decoder_Right_GetCounter()) >> 1;
                               CyDelay(10);
                           }
                       }
                       else
                       {
                           setSpeed(1, speed, FORWARD);
                           setSpeed(2, speed, FORWARD);
                           setSpeed(3, speed, FORWARD);
                           setSpeed(4, speed, FORWARD);                            
                           while((avgDist < (dist * 491) / 65) && !SYSTEM_HALT)
                           {
                               avgDist = (Decoder_Left_GetCounter()+Decoder_Right_GetCounter()) >> 1;
                               CyDelay(10);
                           }
                       }
                       setSpeed(1, 0, BRAKE);
                       setSpeed(2, 0, BRAKE);
                       setSpeed(3, 0, BRAKE);
                       setSpeed(4, 0, BRAKE);
                       ESP07_SendAck();
                       break; }
            case 24: { uint8_t refresh = 0;                
                       while(refresh != 1){refresh = GPS_Ready;}
                       struct GPS_Datagram temp = GPS_Parse(GPS_GPRMC);
                       if(temp.valid == 1)
                       {
                           ESP07_UART_PutString("AT+CIPSEND=0,34\r\n");
                           CyDelay(20);
                           ESP07_UART_PutString("ESP07> ");
                           ESP07_UART_PutString(temp.latitudeStr);
                           ESP07_UART_PutChar(' ');
                           ESP07_UART_PutString(temp.latitudePol);
                           ESP07_UART_PutChar(' ');
                           ESP07_UART_PutString(temp.longitudeStr);
                           ESP07_UART_PutChar(' ');
                           ESP07_UART_PutString(temp.longitudePol);
                           ESP07_UART_PutString("\r\n");
                       } 
                       else
                       {
                           ESP07_UART_PutString("AT+CIPSEND=0,12\r\n");
                           CyDelay(20);
                           ESP07_UART_PutString("ESP07> NAK\r\n");
                       }
                       break; }
            case 25: { struct LSM303_Datagram temp = LSM303_Refresh();
                       ESP07_UART_PutString("AT+CIPSEND=0,13\r\n");
                       CyDelay(20);
                       ESP07_UART_PutString("ESP07> ");
                       sprintf(BUFFER_TEMP, "%04d", temp.heading);
                       ESP07_UART_PutString(BUFFER_TEMP);
                       ESP07_UART_PutString("\r\n");
                       break; }
            case 26: { 
                    char * speedAddr = "075";
                    char * speedAddr2 = "075";
                    char direction = 'L';
                    int q = 0;
                    char * angleAddr = command.payload + 7;
                    char angleStr[4]; strncpy(angleStr, angleAddr, 3); angleAddr[3] = '\0';
                    
                    char * distAddr = command.payload + 11;
                    char distStr[4]; strncpy(distStr, distAddr, 3); distAddr[3] = '\0';
                    int32_t dist = atoi(distStr);
                    
                    char * nthAddr = command.payload + 15;
                    char nthStr[32]; strcpy(nthStr, nthAddr);
                    int32_t nth = atoi(nthStr);
                
                    while(q < nth)
                    {
                       char speedStr[4]; strncpy(speedStr, speedAddr, 3); speedStr[3] = '\0';
                       uint8_t speed = atoi(speedStr);
                       int32_t avgDist = 0;
                       Decoder_Left_SetCounter(0);
                       Decoder_Right_SetCounter(0);
                       if(dist < 0)
                       {
                           setSpeed(1, speed, REVERSE);
                           setSpeed(2, speed, REVERSE);
                           setSpeed(3, speed, REVERSE);
                           setSpeed(4, speed, REVERSE);
                           while((avgDist > (dist * 491) / 65) && !SYSTEM_HALT)
                           {
                               avgDist = (Decoder_Left_GetCounter()+Decoder_Right_GetCounter()) >> 1;
                               CyDelay(10);
                           }
                       }
                       else
                       {
                           setSpeed(1, speed, FORWARD);
                           setSpeed(2, speed, FORWARD);
                           setSpeed(3, speed, FORWARD);
                           setSpeed(4, speed, FORWARD);                            
                           while((avgDist < (dist * 491) / 65) && !SYSTEM_HALT)
                           {
                               avgDist = (Decoder_Left_GetCounter()+Decoder_Right_GetCounter()) >> 1;
                               CyDelay(10);
                           }
                       }
                       setSpeed(1, 0, BRAKE);
                       setSpeed(2, 0, BRAKE);
                       setSpeed(3, 0, BRAKE);
                       setSpeed(4, 0, BRAKE);
                       //ESP07_SendAck();
                        
                        CyDelay(100);
                        
                       char speedStr2[4]; strncpy(speedStr2, speedAddr2, 3); speedStr2[3] = '\0';
                       
                       uint8_t speed2 = atoi(speedStr2);
                       uint32_t angle = atoi(angleStr);
                       if(direction == 'L' || direction == 'l')
                           eRotateCCW(angle, speed2);
                       if(direction == 'R' || direction == 'r')
                           eRotateCW(angle, speed2);
                       //ESP07_SendAck();
                        
                        CyDelay(100);
                        
                        q = q + 1;
                    }
                        
                       break; }
             case 27: { 
                
               setSpeed(1, 0, BRAKE);
               setSpeed(2, 0, BRAKE);
               setSpeed(3, 0, BRAKE);
               setSpeed(4, 0, BRAKE);
            
                break; }
            default: { break; }
        }
    }        
    return;
}

void RotateCCW(uint16_t angle, uint8_t speed)
{
    setSpeed(1, speed, FORWARD);
    setSpeed(2, speed, REVERSE);
    setSpeed(3, speed, REVERSE);
    setSpeed(4, speed, FORWARD);
    
    struct LSM303_Datagram compassData = LSM303_Refresh();
    int16_t target = compassData.heading + (angle * 10);
    int16_t current = compassData.heading;
    uint8_t originPassed = FALSE;   
    
    while(current < target && !SYSTEM_HALT)
    {
        current = compassData.heading;
        if(target > 3600 && (current < (target - 3600) && current >= 0))
            originPassed = TRUE;
        if(originPassed == TRUE)
            current += 3600;
        compassData = LSM303_Refresh();
    }
    
    setSpeed(1, 0, BRAKE);
    setSpeed(2, 0, BRAKE);
    setSpeed(3, 0, BRAKE);
    setSpeed(4, 0, BRAKE);
        
    return;
}

void RotateCW(uint16_t angle, uint8_t speed)
{
    struct LSM303_Datagram compassData = LSM303_Refresh();
    
    int16_t target = compassData.heading - (angle * 10);
    int16_t current = compassData.heading;
    uint8_t originPassed = FALSE;
    
    setSpeed(1, speed, REVERSE);
    setSpeed(2, speed, FORWARD);
    setSpeed(3, speed, FORWARD);
    setSpeed(4, speed, REVERSE);
    
    while(current > target && !SYSTEM_HALT)
    {        
        current = compassData.heading;
        if(target < 0 && (current > (target - 3600) && current <= 0))
            originPassed = TRUE;
        if(originPassed == TRUE)
            current -= 3600;
        compassData = LSM303_Refresh();
    }
        
    setSpeed(1, 0, BRAKE);
    setSpeed(2, 0, BRAKE);
    setSpeed(3, 0, BRAKE);
    setSpeed(4, 0, BRAKE);
        
    return;
}

void eRotateCCW(uint16_t angle, uint8_t speed)
{
    Decoder_Left_SetCounter(0);
    Decoder_Right_SetCounter(0);
    
    setSpeed(1, speed, REVERSE);
    setSpeed(2, speed, FORWARD);
    setSpeed(3, speed, FORWARD);
    setSpeed(4, speed, REVERSE);
    
    int32_t limit = (angle * 27) / 20;
    int32_t average = ((Decoder_Left_GetCounter() * -1) +
                        Decoder_Right_GetCounter() ) >> 2;
    
    while(average < limit && !SYSTEM_HALT)
    {
        average = ((Decoder_Left_GetCounter() * -1) +
                    Decoder_Right_GetCounter() ) >> 2;
    }
    
    setSpeed(1, 0, BRAKE);
    setSpeed(2, 0, BRAKE);
    setSpeed(3, 0, BRAKE);
    setSpeed(4, 0, BRAKE);
    
    return;
}

void eRotateCW(uint16_t angle, uint8_t speed)
{
    Decoder_Left_SetCounter(0);
    Decoder_Right_SetCounter(0);
    
    setSpeed(1, speed, FORWARD);
    setSpeed(2, speed, REVERSE);
    setSpeed(3, speed, REVERSE);
    setSpeed(4, speed, FORWARD);
    
    int32_t limit = (angle * 27) / 20;
    int32_t average = ((Decoder_Right_GetCounter() * -1) +
                        Decoder_Left_GetCounter() ) >> 2;
    
    while(average < limit && !SYSTEM_HALT)
    {
        average = ((Decoder_Right_GetCounter() * -1) +
                    Decoder_Left_GetCounter() ) >> 2;
    }
    
    setSpeed(1, 0, BRAKE);
    setSpeed(2, 0, BRAKE);
    setSpeed(3, 0, BRAKE);
    setSpeed(4, 0, BRAKE);
    
    return;
}

/* [] END OF FILE */
