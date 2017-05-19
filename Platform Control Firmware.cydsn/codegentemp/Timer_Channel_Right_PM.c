/*******************************************************************************
* File Name: Timer_Channel_Right_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Timer_Channel_Right.h"

static Timer_Channel_Right_backupStruct Timer_Channel_Right_backup;


/*******************************************************************************
* Function Name: Timer_Channel_Right_SaveConfig
********************************************************************************
*
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
*  Timer_Channel_Right_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_Channel_Right_SaveConfig(void) 
{
    #if (!Timer_Channel_Right_UsingFixedFunction)
        Timer_Channel_Right_backup.TimerUdb = Timer_Channel_Right_ReadCounter();
        Timer_Channel_Right_backup.InterruptMaskValue = Timer_Channel_Right_STATUS_MASK;
        #if (Timer_Channel_Right_UsingHWCaptureCounter)
            Timer_Channel_Right_backup.TimerCaptureCounter = Timer_Channel_Right_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_Channel_Right_UDB_CONTROL_REG_REMOVED)
            Timer_Channel_Right_backup.TimerControlRegister = Timer_Channel_Right_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_Channel_Right_RestoreConfig
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
*  Timer_Channel_Right_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_Channel_Right_RestoreConfig(void) 
{   
    #if (!Timer_Channel_Right_UsingFixedFunction)

        Timer_Channel_Right_WriteCounter(Timer_Channel_Right_backup.TimerUdb);
        Timer_Channel_Right_STATUS_MASK =Timer_Channel_Right_backup.InterruptMaskValue;
        #if (Timer_Channel_Right_UsingHWCaptureCounter)
            Timer_Channel_Right_SetCaptureCount(Timer_Channel_Right_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_Channel_Right_UDB_CONTROL_REG_REMOVED)
            Timer_Channel_Right_WriteControlRegister(Timer_Channel_Right_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_Channel_Right_Sleep
********************************************************************************
*
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
*  Timer_Channel_Right_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_Channel_Right_Sleep(void) 
{
    #if(!Timer_Channel_Right_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_Channel_Right_CTRL_ENABLE == (Timer_Channel_Right_CONTROL & Timer_Channel_Right_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_Channel_Right_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_Channel_Right_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_Channel_Right_Stop();
    Timer_Channel_Right_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_Channel_Right_Wakeup
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
*  Timer_Channel_Right_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_Channel_Right_Wakeup(void) 
{
    Timer_Channel_Right_RestoreConfig();
    #if(!Timer_Channel_Right_UDB_CONTROL_REG_REMOVED)
        if(Timer_Channel_Right_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_Channel_Right_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
