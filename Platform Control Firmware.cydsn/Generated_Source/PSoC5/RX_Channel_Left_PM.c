/*******************************************************************************
* File Name: RX_Channel_Left_PM.c  
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

#include "RX_Channel_Left.h"

static RX_Channel_Left_backupStruct RX_Channel_Left_backup;


/*******************************************************************************
* Function Name: RX_Channel_Left_SaveConfig
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
*  RX_Channel_Left_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void RX_Channel_Left_SaveConfig(void) 
{
    #if (!RX_Channel_Left_UsingFixedFunction)

        RX_Channel_Left_backup.CounterUdb = RX_Channel_Left_ReadCounter();

        #if(!RX_Channel_Left_ControlRegRemoved)
            RX_Channel_Left_backup.CounterControlRegister = RX_Channel_Left_ReadControlRegister();
        #endif /* (!RX_Channel_Left_ControlRegRemoved) */

    #endif /* (!RX_Channel_Left_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Left_RestoreConfig
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
*  RX_Channel_Left_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RX_Channel_Left_RestoreConfig(void) 
{      
    #if (!RX_Channel_Left_UsingFixedFunction)

       RX_Channel_Left_WriteCounter(RX_Channel_Left_backup.CounterUdb);

        #if(!RX_Channel_Left_ControlRegRemoved)
            RX_Channel_Left_WriteControlRegister(RX_Channel_Left_backup.CounterControlRegister);
        #endif /* (!RX_Channel_Left_ControlRegRemoved) */

    #endif /* (!RX_Channel_Left_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Left_Sleep
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
*  RX_Channel_Left_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void RX_Channel_Left_Sleep(void) 
{
    #if(!RX_Channel_Left_ControlRegRemoved)
        /* Save Counter's enable state */
        if(RX_Channel_Left_CTRL_ENABLE == (RX_Channel_Left_CONTROL & RX_Channel_Left_CTRL_ENABLE))
        {
            /* Counter is enabled */
            RX_Channel_Left_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            RX_Channel_Left_backup.CounterEnableState = 0u;
        }
    #else
        RX_Channel_Left_backup.CounterEnableState = 1u;
        if(RX_Channel_Left_backup.CounterEnableState != 0u)
        {
            RX_Channel_Left_backup.CounterEnableState = 0u;
        }
    #endif /* (!RX_Channel_Left_ControlRegRemoved) */
    
    RX_Channel_Left_Stop();
    RX_Channel_Left_SaveConfig();
}


/*******************************************************************************
* Function Name: RX_Channel_Left_Wakeup
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
*  RX_Channel_Left_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RX_Channel_Left_Wakeup(void) 
{
    RX_Channel_Left_RestoreConfig();
    #if(!RX_Channel_Left_ControlRegRemoved)
        if(RX_Channel_Left_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            RX_Channel_Left_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!RX_Channel_Left_ControlRegRemoved) */
    
}


/* [] END OF FILE */
