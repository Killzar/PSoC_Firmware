/*******************************************************************************
* File Name: Direction_Right_Front_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Direction_Right_Front.h"

/* Check for removal by optimization */
#if !defined(Direction_Right_Front_Sync_ctrl_reg__REMOVED)

static Direction_Right_Front_BACKUP_STRUCT  Direction_Right_Front_backup = {0u};

    
/*******************************************************************************
* Function Name: Direction_Right_Front_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Direction_Right_Front_SaveConfig(void) 
{
    Direction_Right_Front_backup.controlState = Direction_Right_Front_Control;
}


/*******************************************************************************
* Function Name: Direction_Right_Front_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void Direction_Right_Front_RestoreConfig(void) 
{
     Direction_Right_Front_Control = Direction_Right_Front_backup.controlState;
}


/*******************************************************************************
* Function Name: Direction_Right_Front_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Direction_Right_Front_Sleep(void) 
{
    Direction_Right_Front_SaveConfig();
}


/*******************************************************************************
* Function Name: Direction_Right_Front_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Direction_Right_Front_Wakeup(void)  
{
    Direction_Right_Front_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
