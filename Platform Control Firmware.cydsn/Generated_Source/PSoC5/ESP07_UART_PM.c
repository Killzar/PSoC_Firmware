/*******************************************************************************
* File Name: ESP07_UART_PM.c
* Version 2.50
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ESP07_UART.h"


/***************************************
* Local data allocation
***************************************/

static ESP07_UART_BACKUP_STRUCT  ESP07_UART_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: ESP07_UART_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the ESP07_UART_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ESP07_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ESP07_UART_SaveConfig(void)
{
    #if(ESP07_UART_CONTROL_REG_REMOVED == 0u)
        ESP07_UART_backup.cr = ESP07_UART_CONTROL_REG;
    #endif /* End ESP07_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: ESP07_UART_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ESP07_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling ESP07_UART_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void ESP07_UART_RestoreConfig(void)
{
    #if(ESP07_UART_CONTROL_REG_REMOVED == 0u)
        ESP07_UART_CONTROL_REG = ESP07_UART_backup.cr;
    #endif /* End ESP07_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: ESP07_UART_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The ESP07_UART_Sleep() API saves the current component state. Then it
*  calls the ESP07_UART_Stop() function and calls 
*  ESP07_UART_SaveConfig() to save the hardware configuration.
*  Call the ESP07_UART_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ESP07_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ESP07_UART_Sleep(void)
{
    #if(ESP07_UART_RX_ENABLED || ESP07_UART_HD_ENABLED)
        if((ESP07_UART_RXSTATUS_ACTL_REG  & ESP07_UART_INT_ENABLE) != 0u)
        {
            ESP07_UART_backup.enableState = 1u;
        }
        else
        {
            ESP07_UART_backup.enableState = 0u;
        }
    #else
        if((ESP07_UART_TXSTATUS_ACTL_REG  & ESP07_UART_INT_ENABLE) !=0u)
        {
            ESP07_UART_backup.enableState = 1u;
        }
        else
        {
            ESP07_UART_backup.enableState = 0u;
        }
    #endif /* End ESP07_UART_RX_ENABLED || ESP07_UART_HD_ENABLED*/

    ESP07_UART_Stop();
    ESP07_UART_SaveConfig();
}


/*******************************************************************************
* Function Name: ESP07_UART_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  ESP07_UART_Sleep() was called. The ESP07_UART_Wakeup() function
*  calls the ESP07_UART_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  ESP07_UART_Sleep() function was called, the ESP07_UART_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  ESP07_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ESP07_UART_Wakeup(void)
{
    ESP07_UART_RestoreConfig();
    #if( (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) )
        ESP07_UART_ClearRxBuffer();
    #endif /* End (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) */
    #if(ESP07_UART_TX_ENABLED || ESP07_UART_HD_ENABLED)
        ESP07_UART_ClearTxBuffer();
    #endif /* End ESP07_UART_TX_ENABLED || ESP07_UART_HD_ENABLED */

    if(ESP07_UART_backup.enableState != 0u)
    {
        ESP07_UART_Enable();
    }
}


/* [] END OF FILE */
