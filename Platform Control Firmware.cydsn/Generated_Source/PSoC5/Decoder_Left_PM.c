/*******************************************************************************
* File Name: Decoder_Left_PM.c
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

#include "Decoder_Left.h"

static Decoder_Left_BACKUP_STRUCT Decoder_Left_backup = {0u};


/*******************************************************************************
* Function Name: Decoder_Left_SaveConfig
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
void Decoder_Left_SaveConfig(void) 
{
    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
        Decoder_Left_Cnt8_SaveConfig();
    #else 
        /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) || 
         * (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT)
         */
        Decoder_Left_Cnt16_SaveConfig();
    #endif /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Decoder_Left_RestoreConfig
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
void Decoder_Left_RestoreConfig(void) 
{
    #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
        Decoder_Left_Cnt8_RestoreConfig();
    #else 
        /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) || 
         * (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT) 
         */
        Decoder_Left_Cnt16_RestoreConfig();
    #endif /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Decoder_Left_Sleep
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
*  Decoder_Left_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Decoder_Left_Sleep(void) 
{
    if (0u != (Decoder_Left_SR_AUX_CONTROL & Decoder_Left_INTERRUPTS_ENABLE))
    {
        Decoder_Left_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        Decoder_Left_backup.enableState = 0u;
    }

    Decoder_Left_Stop();
    Decoder_Left_SaveConfig();
}


/*******************************************************************************
* Function Name: Decoder_Left_Wakeup
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
*  Decoder_Left_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void Decoder_Left_Wakeup(void) 
{
    Decoder_Left_RestoreConfig();

    if (Decoder_Left_backup.enableState != 0u)
    {
        #if (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT)
            Decoder_Left_Cnt8_Enable();
        #else 
            /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_16_BIT) || 
            *  (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_32_BIT) 
            */
            Decoder_Left_Cnt16_Enable();
        #endif /* (Decoder_Left_COUNTER_SIZE == Decoder_Left_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        Decoder_Left_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

