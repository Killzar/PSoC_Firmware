/*******************************************************************************
* File Name: Debug_LED.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_Debug_LED_H) /* CY_CONTROL_REG_Debug_LED_H */
#define CY_CONTROL_REG_Debug_LED_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} Debug_LED_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    Debug_LED_Write(uint8 control) ;
uint8   Debug_LED_Read(void) ;

void Debug_LED_SaveConfig(void) ;
void Debug_LED_RestoreConfig(void) ;
void Debug_LED_Sleep(void) ; 
void Debug_LED_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Debug_LED_Control        (* (reg8 *) Debug_LED_Sync_ctrl_reg__CONTROL_REG )
#define Debug_LED_Control_PTR    (  (reg8 *) Debug_LED_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_Debug_LED_H */


/* [] END OF FILE */
