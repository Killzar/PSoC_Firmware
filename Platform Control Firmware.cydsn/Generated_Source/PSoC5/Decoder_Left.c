/*******************************************************************************
* File Name: Decoder_Left.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
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

#if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
    #include "Decoder_Left_PVT.h"
#endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */

uint8 Decoder_Left_initVar = 0u;


/*******************************************************************************
* Function Name: Decoder_Left_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Decoder_Left_Init(void) 
{
    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(Decoder_Left_ISR_NUMBER);
        /* Set the ISR to point to the Decoder_Left_isr Interrupt. */
        (void) CyIntSetVector(Decoder_Left_ISR_NUMBER, & Decoder_Left_ISR);
        /* Set the priority. */
        CyIntSetPriority(Decoder_Left_ISR_NUMBER, Decoder_Left_ISR_PRIORITY);
    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Decoder_Left_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Decoder_Left_Enable(void) 
{
    uint8 enableInterrupts;

    Decoder_Left_SetInterruptMask(Decoder_Left_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) Decoder_Left_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    Decoder_Left_SR_AUX_CONTROL |= Decoder_Left_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(Decoder_Left_ISR_NUMBER);
    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Decoder_Left_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Decoder_Left_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Decoder_Left_Start(void) 
{
    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
        Decoder_Left_Cnt8_Start();
        Decoder_Left_Cnt8_WriteCounter(Decoder_Left_COUNTER_INIT_VALUE);
    #else
        /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) || 
        *  (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT) 
        */
        Decoder_Left_Cnt16_Start();
        Decoder_Left_Cnt16_WriteCounter(Decoder_Left_COUNTER_INIT_VALUE);
    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT */
    
    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)        
       Decoder_Left_count32SoftPart = 0;
    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */

    if (Decoder_Left_initVar == 0u)
    {
        Decoder_Left_Init();
        Decoder_Left_initVar = 1u;
    }

    Decoder_Left_Enable();
}


/*******************************************************************************
* Function Name: Decoder_Left_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Decoder_Left_Stop(void) 
{
    uint8 enableInterrupts;

    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
        Decoder_Left_Cnt8_Stop();
    #else 
        /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) ||
        *  (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
        */
        Decoder_Left_Cnt16_Stop();    /* counter disable */
    #endif /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    Decoder_Left_SR_AUX_CONTROL &= (uint8) (~Decoder_Left_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
        CyIntDisable(Decoder_Left_ISR_NUMBER);    /* interrupt disable */
    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Decoder_Left_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  Decoder_Left_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int32 Decoder_Left_GetCounter(void) 
{
    int32 count;
    uint16 tmpCnt;

    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(Decoder_Left_ISR_NUMBER);

        tmpCnt = Decoder_Left_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) Decoder_Left_COUNTER_INIT_VALUE);
        count = Decoder_Left_count32SoftPart + hwCount;

        CyIntEnable(Decoder_Left_ISR_NUMBER);
    #else 
        /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) || 
        *  (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT)
        */
        #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
            tmpCnt = Decoder_Left_Cnt8_ReadCounter();
        #else /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) */
            tmpCnt = Decoder_Left_Cnt16_ReadCounter();
        #endif  /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT */

        count = (int32) ((int32) tmpCnt -
                (int32) Decoder_Left_COUNTER_INIT_VALUE);

    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: Decoder_Left_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  Decoder_Left_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Decoder_Left_SetCounter(int32 value) 
{
    #if ((Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) || \
         (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + Decoder_Left_COUNTER_INIT_VALUE;
        }
        else
        {
            count = Decoder_Left_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
            Decoder_Left_Cnt8_WriteCounter(count);
        #else /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) */
            Decoder_Left_Cnt16_WriteCounter(count);
        #endif  /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT */
    #else /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT) */
        CyIntDisable(Decoder_Left_ISR_NUMBER);

        Decoder_Left_Cnt16_WriteCounter(Decoder_Left_COUNTER_INIT_VALUE);
        Decoder_Left_count32SoftPart = value;

        CyIntEnable(Decoder_Left_ISR_NUMBER);
    #endif  /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) ||
             * (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: Decoder_Left_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 Decoder_Left_GetEvents(void) 
{
    return (Decoder_Left_STATUS_REG & Decoder_Left_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: Decoder_Left_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void Decoder_Left_SetInterruptMask(uint8 mask) 
{
    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (Decoder_Left_COUNTER_OVERFLOW | Decoder_Left_COUNTER_UNDERFLOW |
                 Decoder_Left_COUNTER_RESET);
    #endif /* Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT */

    Decoder_Left_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: Decoder_Left_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 Decoder_Left_GetInterruptMask(void) 
{
    return (Decoder_Left_STATUS_MASK & Decoder_Left_INIT_INT_MASK);
}


/* [] END OF FILE */
