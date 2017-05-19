/*******************************************************************************
* File Name: RX_PWM_Left.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_RX_PWM_Left_H) /* Pins RX_PWM_Left_H */
#define CY_PINS_RX_PWM_Left_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RX_PWM_Left_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RX_PWM_Left__PORT == 15 && ((RX_PWM_Left__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    RX_PWM_Left_Write(uint8 value);
void    RX_PWM_Left_SetDriveMode(uint8 mode);
uint8   RX_PWM_Left_ReadDataReg(void);
uint8   RX_PWM_Left_Read(void);
void    RX_PWM_Left_SetInterruptMode(uint16 position, uint16 mode);
uint8   RX_PWM_Left_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the RX_PWM_Left_SetDriveMode() function.
     *  @{
     */
        #define RX_PWM_Left_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define RX_PWM_Left_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define RX_PWM_Left_DM_RES_UP          PIN_DM_RES_UP
        #define RX_PWM_Left_DM_RES_DWN         PIN_DM_RES_DWN
        #define RX_PWM_Left_DM_OD_LO           PIN_DM_OD_LO
        #define RX_PWM_Left_DM_OD_HI           PIN_DM_OD_HI
        #define RX_PWM_Left_DM_STRONG          PIN_DM_STRONG
        #define RX_PWM_Left_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define RX_PWM_Left_MASK               RX_PWM_Left__MASK
#define RX_PWM_Left_SHIFT              RX_PWM_Left__SHIFT
#define RX_PWM_Left_WIDTH              1u

/* Interrupt constants */
#if defined(RX_PWM_Left__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RX_PWM_Left_SetInterruptMode() function.
     *  @{
     */
        #define RX_PWM_Left_INTR_NONE      (uint16)(0x0000u)
        #define RX_PWM_Left_INTR_RISING    (uint16)(0x0001u)
        #define RX_PWM_Left_INTR_FALLING   (uint16)(0x0002u)
        #define RX_PWM_Left_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define RX_PWM_Left_INTR_MASK      (0x01u) 
#endif /* (RX_PWM_Left__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RX_PWM_Left_PS                     (* (reg8 *) RX_PWM_Left__PS)
/* Data Register */
#define RX_PWM_Left_DR                     (* (reg8 *) RX_PWM_Left__DR)
/* Port Number */
#define RX_PWM_Left_PRT_NUM                (* (reg8 *) RX_PWM_Left__PRT) 
/* Connect to Analog Globals */                                                  
#define RX_PWM_Left_AG                     (* (reg8 *) RX_PWM_Left__AG)                       
/* Analog MUX bux enable */
#define RX_PWM_Left_AMUX                   (* (reg8 *) RX_PWM_Left__AMUX) 
/* Bidirectional Enable */                                                        
#define RX_PWM_Left_BIE                    (* (reg8 *) RX_PWM_Left__BIE)
/* Bit-mask for Aliased Register Access */
#define RX_PWM_Left_BIT_MASK               (* (reg8 *) RX_PWM_Left__BIT_MASK)
/* Bypass Enable */
#define RX_PWM_Left_BYP                    (* (reg8 *) RX_PWM_Left__BYP)
/* Port wide control signals */                                                   
#define RX_PWM_Left_CTL                    (* (reg8 *) RX_PWM_Left__CTL)
/* Drive Modes */
#define RX_PWM_Left_DM0                    (* (reg8 *) RX_PWM_Left__DM0) 
#define RX_PWM_Left_DM1                    (* (reg8 *) RX_PWM_Left__DM1)
#define RX_PWM_Left_DM2                    (* (reg8 *) RX_PWM_Left__DM2) 
/* Input Buffer Disable Override */
#define RX_PWM_Left_INP_DIS                (* (reg8 *) RX_PWM_Left__INP_DIS)
/* LCD Common or Segment Drive */
#define RX_PWM_Left_LCD_COM_SEG            (* (reg8 *) RX_PWM_Left__LCD_COM_SEG)
/* Enable Segment LCD */
#define RX_PWM_Left_LCD_EN                 (* (reg8 *) RX_PWM_Left__LCD_EN)
/* Slew Rate Control */
#define RX_PWM_Left_SLW                    (* (reg8 *) RX_PWM_Left__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RX_PWM_Left_PRTDSI__CAPS_SEL       (* (reg8 *) RX_PWM_Left__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RX_PWM_Left_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RX_PWM_Left__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RX_PWM_Left_PRTDSI__OE_SEL0        (* (reg8 *) RX_PWM_Left__PRTDSI__OE_SEL0) 
#define RX_PWM_Left_PRTDSI__OE_SEL1        (* (reg8 *) RX_PWM_Left__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RX_PWM_Left_PRTDSI__OUT_SEL0       (* (reg8 *) RX_PWM_Left__PRTDSI__OUT_SEL0) 
#define RX_PWM_Left_PRTDSI__OUT_SEL1       (* (reg8 *) RX_PWM_Left__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RX_PWM_Left_PRTDSI__SYNC_OUT       (* (reg8 *) RX_PWM_Left__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(RX_PWM_Left__SIO_CFG)
    #define RX_PWM_Left_SIO_HYST_EN        (* (reg8 *) RX_PWM_Left__SIO_HYST_EN)
    #define RX_PWM_Left_SIO_REG_HIFREQ     (* (reg8 *) RX_PWM_Left__SIO_REG_HIFREQ)
    #define RX_PWM_Left_SIO_CFG            (* (reg8 *) RX_PWM_Left__SIO_CFG)
    #define RX_PWM_Left_SIO_DIFF           (* (reg8 *) RX_PWM_Left__SIO_DIFF)
#endif /* (RX_PWM_Left__SIO_CFG) */

/* Interrupt Registers */
#if defined(RX_PWM_Left__INTSTAT)
    #define RX_PWM_Left_INTSTAT            (* (reg8 *) RX_PWM_Left__INTSTAT)
    #define RX_PWM_Left_SNAP               (* (reg8 *) RX_PWM_Left__SNAP)
    
	#define RX_PWM_Left_0_INTTYPE_REG 		(* (reg8 *) RX_PWM_Left__0__INTTYPE)
#endif /* (RX_PWM_Left__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RX_PWM_Left_H */


/* [] END OF FILE */
