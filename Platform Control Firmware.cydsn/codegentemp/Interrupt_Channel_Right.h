/*******************************************************************************
* File Name: Interrupt_Channel_Right.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Interrupt_Channel_Right_H)
#define CY_ISR_Interrupt_Channel_Right_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Interrupt_Channel_Right_Start(void);
void Interrupt_Channel_Right_StartEx(cyisraddress address);
void Interrupt_Channel_Right_Stop(void);

CY_ISR_PROTO(Interrupt_Channel_Right_Interrupt);

void Interrupt_Channel_Right_SetVector(cyisraddress address);
cyisraddress Interrupt_Channel_Right_GetVector(void);

void Interrupt_Channel_Right_SetPriority(uint8 priority);
uint8 Interrupt_Channel_Right_GetPriority(void);

void Interrupt_Channel_Right_Enable(void);
uint8 Interrupt_Channel_Right_GetState(void);
void Interrupt_Channel_Right_Disable(void);

void Interrupt_Channel_Right_SetPending(void);
void Interrupt_Channel_Right_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Interrupt_Channel_Right ISR. */
#define Interrupt_Channel_Right_INTC_VECTOR            ((reg32 *) Interrupt_Channel_Right__INTC_VECT)

/* Address of the Interrupt_Channel_Right ISR priority. */
#define Interrupt_Channel_Right_INTC_PRIOR             ((reg8 *) Interrupt_Channel_Right__INTC_PRIOR_REG)

/* Priority of the Interrupt_Channel_Right interrupt. */
#define Interrupt_Channel_Right_INTC_PRIOR_NUMBER      Interrupt_Channel_Right__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Interrupt_Channel_Right interrupt. */
#define Interrupt_Channel_Right_INTC_SET_EN            ((reg32 *) Interrupt_Channel_Right__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Interrupt_Channel_Right interrupt. */
#define Interrupt_Channel_Right_INTC_CLR_EN            ((reg32 *) Interrupt_Channel_Right__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Interrupt_Channel_Right interrupt state to pending. */
#define Interrupt_Channel_Right_INTC_SET_PD            ((reg32 *) Interrupt_Channel_Right__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Interrupt_Channel_Right interrupt. */
#define Interrupt_Channel_Right_INTC_CLR_PD            ((reg32 *) Interrupt_Channel_Right__INTC_CLR_PD_REG)


#endif /* CY_ISR_Interrupt_Channel_Right_H */


/* [] END OF FILE */
