/*******************************************************************************
* File Name: Direction_Left_Front.h  
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

#if !defined(CY_CONTROL_REG_Direction_Left_Front_H) /* CY_CONTROL_REG_Direction_Left_Front_H */
#define CY_CONTROL_REG_Direction_Left_Front_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} Direction_Left_Front_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    Direction_Left_Front_Write(uint8 control) ;
uint8   Direction_Left_Front_Read(void) ;

void Direction_Left_Front_SaveConfig(void) ;
void Direction_Left_Front_RestoreConfig(void) ;
void Direction_Left_Front_Sleep(void) ; 
void Direction_Left_Front_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Direction_Left_Front_Control        (* (reg8 *) Direction_Left_Front_Sync_ctrl_reg__CONTROL_REG )
#define Direction_Left_Front_Control_PTR    (  (reg8 *) Direction_Left_Front_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_Direction_Left_Front_H */


/* [] END OF FILE */
