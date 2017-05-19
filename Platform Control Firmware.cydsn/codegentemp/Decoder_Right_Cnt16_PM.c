/*******************************************************************************
* File Name: Decoder_Right_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Decoder_Right_Cnt16.h"

static Decoder_Right_Cnt16_backupStruct Decoder_Right_Cnt16_backup;


/*******************************************************************************
* Function Name: Decoder_Right_Cnt16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Decoder_Right_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Decoder_Right_Cnt16_SaveConfig(void) 
{
    #if (!Decoder_Right_Cnt16_UsingFixedFunction)

        Decoder_Right_Cnt16_backup.CounterUdb = Decoder_Right_Cnt16_ReadCounter();

        #if(!Decoder_Right_Cnt16_ControlRegRemoved)
            Decoder_Right_Cnt16_backup.CounterControlRegister = Decoder_Right_Cnt16_ReadControlRegister();
        #endif /* (!Decoder_Right_Cnt16_ControlRegRemoved) */

    #endif /* (!Decoder_Right_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Decoder_Right_Cnt16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Decoder_Right_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Decoder_Right_Cnt16_RestoreConfig(void) 
{      
    #if (!Decoder_Right_Cnt16_UsingFixedFunction)

       Decoder_Right_Cnt16_WriteCounter(Decoder_Right_Cnt16_backup.CounterUdb);

        #if(!Decoder_Right_Cnt16_ControlRegRemoved)
            Decoder_Right_Cnt16_WriteControlRegister(Decoder_Right_Cnt16_backup.CounterControlRegister);
        #endif /* (!Decoder_Right_Cnt16_ControlRegRemoved) */

    #endif /* (!Decoder_Right_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Decoder_Right_Cnt16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Decoder_Right_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Decoder_Right_Cnt16_Sleep(void) 
{
    #if(!Decoder_Right_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Decoder_Right_Cnt16_CTRL_ENABLE == (Decoder_Right_Cnt16_CONTROL & Decoder_Right_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Decoder_Right_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Decoder_Right_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        Decoder_Right_Cnt16_backup.CounterEnableState = 1u;
        if(Decoder_Right_Cnt16_backup.CounterEnableState != 0u)
        {
            Decoder_Right_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!Decoder_Right_Cnt16_ControlRegRemoved) */
    
    Decoder_Right_Cnt16_Stop();
    Decoder_Right_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: Decoder_Right_Cnt16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Decoder_Right_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Decoder_Right_Cnt16_Wakeup(void) 
{
    Decoder_Right_Cnt16_RestoreConfig();
    #if(!Decoder_Right_Cnt16_ControlRegRemoved)
        if(Decoder_Right_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Decoder_Right_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Decoder_Right_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
