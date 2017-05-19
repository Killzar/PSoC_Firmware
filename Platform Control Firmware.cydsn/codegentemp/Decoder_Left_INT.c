/*******************************************************************************
* File Name: Decoder_Left_INT.c
* Version 3.0
*
* Description:
*  This file contains the Interrupt Service Routine (ISR) for the Quadrature
*  Decoder component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Decoder_Left.h"
#include "Decoder_Left_PVT.h"


volatile int32 Decoder_Left_count32SoftPart = 0;


/*******************************************************************************
* FUNCTION NAME: void Decoder_Left_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for Quadrature Decoder Component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Decoder_Left_count32SoftPart - modified to update hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*  Decoder_Left_swStatus - modified with the updated values of STATUS
*  register.
*
*******************************************************************************/
CY_ISR( Decoder_Left_ISR )
{
   uint8 Decoder_Left_swStatus;

   Decoder_Left_swStatus = Decoder_Left_STATUS_REG;

    #ifdef Decoder_Left_ISR_ENTRY_CALLBACK
        Decoder_Left_ISR_EntryCallback();
    #endif /* Decoder_Left_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START Decoder_Left_ISR_START` */

    /* `#END` */

    if (0u != (Decoder_Left_swStatus & Decoder_Left_COUNTER_OVERFLOW))
    {
        Decoder_Left_count32SoftPart += (int32) Decoder_Left_COUNTER_MAX_VALUE;
    }
    else if (0u != (Decoder_Left_swStatus & Decoder_Left_COUNTER_UNDERFLOW))
    {
        Decoder_Left_count32SoftPart -= (int32) Decoder_Left_COUNTER_INIT_VALUE;
    }
    else
    {
        /* Nothing to do here */
    }
    
    if (0u != (Decoder_Left_swStatus & Decoder_Left_COUNTER_RESET))
    {
        Decoder_Left_count32SoftPart = 0;
    }
    
    /* User code required at end of ISR */
    /* `#START Decoder_Left_ISR_END` */

    /* `#END` */

    #ifdef Decoder_Left_ISR_EXIT_CALLBACK
        Decoder_Left_ISR_ExitCallback();
    #endif /* Decoder_Left_ISR_EXIT_CALLBACK */
}


/* [] END OF FILE */
