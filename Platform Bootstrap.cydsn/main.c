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
/// Function:   4DShoot Controller Bootstrap Loader                           ///
/// Date:       3 February 2017                                               ///
/// Target:     CY8C5888LTI-LP097                                             ///
/// Software:   PSoC Creator 4.0 Update 1                                     ///
/// Author:     Matt Cauvel, Ryan Kelley                                      ///
/// Revisions:                                                                ///
///             0.A - 03/02/2017 - File Created                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

#include "project.h"

int main(void)
{
    CyGlobalIntEnable;
    FT232_UART_Start();
    
    CyDelay(500);
    
    // Wait for bootloader host indefinitely if FT232_Detect indicates
    // attached serial host.  Otherwise, exit to bootloadable.
    /////////////////////////////////////////////////////////////////////
     
    if (FT232_Detect_Read() == 1)
    {
        Status_LED_Write(1);
        Bootloader_Start();
    }
    else
    {
        Status_LED_Write(0);
        Bootloader_Exit(Bootloader_EXIT_TO_BTLDB);
    }
    
    for(;;)
    {
    }
}
/* [] END OF FILE */
