/*******************************************************************************
* File Name: Decoder_Right_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
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

#include "Decoder_Right.h"

static Decoder_Right_BACKUP_STRUCT Decoder_Right_backup = {0u};


/*******************************************************************************
* Function Name: Decoder_Right_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Decoder_Right_SaveConfig(void) 
{
    #if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT)
        Decoder_Right_Cnt8_SaveConfig();
    #else 
        /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_16_BIT) || 
         * (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT)
         */
        Decoder_Right_Cnt16_SaveConfig();
    #endif /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Decoder_Right_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Decoder_Right_RestoreConfig(void) 
{
    #if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT)
        Decoder_Right_Cnt8_RestoreConfig();
    #else 
        /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_16_BIT) || 
         * (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT) 
         */
        Decoder_Right_Cnt16_RestoreConfig();
    #endif /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Decoder_Right_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Decoder_Right_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Decoder_Right_Sleep(void) 
{
    if (0u != (Decoder_Right_SR_AUX_CONTROL & Decoder_Right_INTERRUPTS_ENABLE))
    {
        Decoder_Right_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        Decoder_Right_backup.enableState = 0u;
    }

    Decoder_Right_Stop();
    Decoder_Right_SaveConfig();
}


/*******************************************************************************
* Function Name: Decoder_Right_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Decoder_Right_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void Decoder_Right_Wakeup(void) 
{
    Decoder_Right_RestoreConfig();

    if (Decoder_Right_backup.enableState != 0u)
    {
        #if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT)
            Decoder_Right_Cnt8_Enable();
        #else 
            /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_16_BIT) || 
            *  (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT) 
            */
            Decoder_Right_Cnt16_Enable();
        #endif /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        Decoder_Right_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

