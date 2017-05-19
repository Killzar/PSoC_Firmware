/*******************************************************************************
* File Name: Decoder_Right.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
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

#if !defined(CY_QUADRATURE_DECODER_Decoder_Right_H)
#define CY_QUADRATURE_DECODER_Decoder_Right_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define Decoder_Right_COUNTER_SIZE               (32u)
#define Decoder_Right_COUNTER_SIZE_8_BIT         (8u)
#define Decoder_Right_COUNTER_SIZE_16_BIT        (16u)
#define Decoder_Right_COUNTER_SIZE_32_BIT        (32u)

#if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT)
    #include "Decoder_Right_Cnt8.h"
#else 
    /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_16_BIT) || 
    *  (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT) 
    */
    #include "Decoder_Right_Cnt16.h"
#endif /* Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT */

extern uint8 Decoder_Right_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define Decoder_Right_COUNTER_RESOLUTION         (1u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} Decoder_Right_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  Decoder_Right_Init(void) ;
void  Decoder_Right_Start(void) ;
void  Decoder_Right_Stop(void) ;
void  Decoder_Right_Enable(void) ;
uint8 Decoder_Right_GetEvents(void) ;
void  Decoder_Right_SetInterruptMask(uint8 mask) ;
uint8 Decoder_Right_GetInterruptMask(void) ;
int32 Decoder_Right_GetCounter(void) ;
void  Decoder_Right_SetCounter(int32 value)
;
void  Decoder_Right_Sleep(void) ;
void  Decoder_Right_Wakeup(void) ;
void  Decoder_Right_SaveConfig(void) ;
void  Decoder_Right_RestoreConfig(void) ;

#if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(Decoder_Right_ISR);
#endif /* Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT)
    #define Decoder_Right_ISR_NUMBER             ((uint8) Decoder_Right_isr__INTC_NUMBER)
    #define Decoder_Right_ISR_PRIORITY           ((uint8) Decoder_Right_isr__INTC_PRIOR_NUM)
#endif /* Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Decoder_Right_GLITCH_FILTERING           (1u)
#define Decoder_Right_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT)
    #define Decoder_Right_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_16_BIT) ||
    *  (Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_32_BIT)
    */
    #define Decoder_Right_COUNTER_INIT_VALUE    (0x8000u)
    #define Decoder_Right_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* Decoder_Right_COUNTER_SIZE == Decoder_Right_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define Decoder_Right_STATUS_REG                 (* (reg8 *) Decoder_Right_bQuadDec_Stsreg__STATUS_REG)
#define Decoder_Right_STATUS_PTR                 (  (reg8 *) Decoder_Right_bQuadDec_Stsreg__STATUS_REG)
#define Decoder_Right_STATUS_MASK                (* (reg8 *) Decoder_Right_bQuadDec_Stsreg__MASK_REG)
#define Decoder_Right_STATUS_MASK_PTR            (  (reg8 *) Decoder_Right_bQuadDec_Stsreg__MASK_REG)
#define Decoder_Right_SR_AUX_CONTROL             (* (reg8 *) Decoder_Right_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define Decoder_Right_SR_AUX_CONTROL_PTR         (  (reg8 *) Decoder_Right_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define Decoder_Right_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define Decoder_Right_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define Decoder_Right_COUNTER_RESET_SHIFT        (0x02u)
#define Decoder_Right_INVALID_IN_SHIFT           (0x03u)
#define Decoder_Right_COUNTER_OVERFLOW           ((uint8) (0x01u << Decoder_Right_COUNTER_OVERFLOW_SHIFT))
#define Decoder_Right_COUNTER_UNDERFLOW          ((uint8) (0x01u << Decoder_Right_COUNTER_UNDERFLOW_SHIFT))
#define Decoder_Right_COUNTER_RESET              ((uint8) (0x01u << Decoder_Right_COUNTER_RESET_SHIFT))
#define Decoder_Right_INVALID_IN                 ((uint8) (0x01u << Decoder_Right_INVALID_IN_SHIFT))

#define Decoder_Right_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define Decoder_Right_INTERRUPTS_ENABLE          ((uint8)(0x01u << Decoder_Right_INTERRUPTS_ENABLE_SHIFT))
#define Decoder_Right_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define Decoder_Right_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_Decoder_Right_H */


/* [] END OF FILE */
