/*******************************************************************************
* File Name: RX_Channel_Right.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

#include "RX_Channel_Right.h"

uint8 RX_Channel_Right_initVar = 0u;


/*******************************************************************************
* Function Name: RX_Channel_Right_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void RX_Channel_Right_Init(void) 
{
        #if (!RX_Channel_Right_UsingFixedFunction && !RX_Channel_Right_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!RX_Channel_Right_UsingFixedFunction && !RX_Channel_Right_ControlRegRemoved) */
        
        #if(!RX_Channel_Right_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 RX_Channel_Right_interruptState;
        #endif /* (!RX_Channel_Right_UsingFixedFunction) */
        
        #if (RX_Channel_Right_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            RX_Channel_Right_CONTROL &= RX_Channel_Right_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                RX_Channel_Right_CONTROL2 &= ((uint8)(~RX_Channel_Right_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                RX_Channel_Right_CONTROL3 &= ((uint8)(~RX_Channel_Right_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (RX_Channel_Right_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                RX_Channel_Right_CONTROL |= RX_Channel_Right_ONESHOT;
            #endif /* (RX_Channel_Right_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            RX_Channel_Right_CONTROL2 |= RX_Channel_Right_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            RX_Channel_Right_RT1 &= ((uint8)(~RX_Channel_Right_RT1_MASK));
            RX_Channel_Right_RT1 |= RX_Channel_Right_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            RX_Channel_Right_RT1 &= ((uint8)(~RX_Channel_Right_SYNCDSI_MASK));
            RX_Channel_Right_RT1 |= RX_Channel_Right_SYNCDSI_EN;

        #else
            #if(!RX_Channel_Right_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = RX_Channel_Right_CONTROL & ((uint8)(~RX_Channel_Right_CTRL_CMPMODE_MASK));
            RX_Channel_Right_CONTROL = ctrl | RX_Channel_Right_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = RX_Channel_Right_CONTROL & ((uint8)(~RX_Channel_Right_CTRL_CAPMODE_MASK));
            
            #if( 0 != RX_Channel_Right_CAPTURE_MODE_CONF)
                RX_Channel_Right_CONTROL = ctrl | RX_Channel_Right_DEFAULT_CAPTURE_MODE;
            #else
                RX_Channel_Right_CONTROL = ctrl;
            #endif /* 0 != RX_Channel_Right_CAPTURE_MODE */ 
            
            #endif /* (!RX_Channel_Right_ControlRegRemoved) */
        #endif /* (RX_Channel_Right_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!RX_Channel_Right_UsingFixedFunction)
            RX_Channel_Right_ClearFIFO();
        #endif /* (!RX_Channel_Right_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        RX_Channel_Right_WritePeriod(RX_Channel_Right_INIT_PERIOD_VALUE);
        #if (!(RX_Channel_Right_UsingFixedFunction && (CY_PSOC5A)))
            RX_Channel_Right_WriteCounter(RX_Channel_Right_INIT_COUNTER_VALUE);
        #endif /* (!(RX_Channel_Right_UsingFixedFunction && (CY_PSOC5A))) */
        RX_Channel_Right_SetInterruptMode(RX_Channel_Right_INIT_INTERRUPTS_MASK);
        
        #if (!RX_Channel_Right_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)RX_Channel_Right_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            RX_Channel_Right_WriteCompare(RX_Channel_Right_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            RX_Channel_Right_interruptState = CyEnterCriticalSection();
            
            RX_Channel_Right_STATUS_AUX_CTRL |= RX_Channel_Right_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(RX_Channel_Right_interruptState);
            
        #endif /* (!RX_Channel_Right_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Right_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void RX_Channel_Right_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (RX_Channel_Right_UsingFixedFunction)
        RX_Channel_Right_GLOBAL_ENABLE |= RX_Channel_Right_BLOCK_EN_MASK;
        RX_Channel_Right_GLOBAL_STBY_ENABLE |= RX_Channel_Right_BLOCK_STBY_EN_MASK;
    #endif /* (RX_Channel_Right_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!RX_Channel_Right_ControlRegRemoved || RX_Channel_Right_UsingFixedFunction)
        RX_Channel_Right_CONTROL |= RX_Channel_Right_CTRL_ENABLE;                
    #endif /* (!RX_Channel_Right_ControlRegRemoved || RX_Channel_Right_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: RX_Channel_Right_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  RX_Channel_Right_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void RX_Channel_Right_Start(void) 
{
    if(RX_Channel_Right_initVar == 0u)
    {
        RX_Channel_Right_Init();
        
        RX_Channel_Right_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    RX_Channel_Right_Enable();        
}


/*******************************************************************************
* Function Name: RX_Channel_Right_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void RX_Channel_Right_Stop(void) 
{
    /* Disable Counter */
    #if(!RX_Channel_Right_ControlRegRemoved || RX_Channel_Right_UsingFixedFunction)
        RX_Channel_Right_CONTROL &= ((uint8)(~RX_Channel_Right_CTRL_ENABLE));        
    #endif /* (!RX_Channel_Right_ControlRegRemoved || RX_Channel_Right_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (RX_Channel_Right_UsingFixedFunction)
        RX_Channel_Right_GLOBAL_ENABLE &= ((uint8)(~RX_Channel_Right_BLOCK_EN_MASK));
        RX_Channel_Right_GLOBAL_STBY_ENABLE &= ((uint8)(~RX_Channel_Right_BLOCK_STBY_EN_MASK));
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Right_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void RX_Channel_Right_SetInterruptMode(uint8 interruptsMask) 
{
    RX_Channel_Right_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: RX_Channel_Right_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   RX_Channel_Right_ReadStatusRegister(void) 
{
    return RX_Channel_Right_STATUS;
}


#if(!RX_Channel_Right_ControlRegRemoved)
/*******************************************************************************
* Function Name: RX_Channel_Right_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   RX_Channel_Right_ReadControlRegister(void) 
{
    return RX_Channel_Right_CONTROL;
}


/*******************************************************************************
* Function Name: RX_Channel_Right_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    RX_Channel_Right_WriteControlRegister(uint8 control) 
{
    RX_Channel_Right_CONTROL = control;
}

#endif  /* (!RX_Channel_Right_ControlRegRemoved) */


#if (!(RX_Channel_Right_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: RX_Channel_Right_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void RX_Channel_Right_WriteCounter(uint16 counter) \
                                   
{
    #if(RX_Channel_Right_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (RX_Channel_Right_GLOBAL_ENABLE & RX_Channel_Right_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        RX_Channel_Right_GLOBAL_ENABLE |= RX_Channel_Right_BLOCK_EN_MASK;
        CY_SET_REG16(RX_Channel_Right_COUNTER_LSB_PTR, (uint16)counter);
        RX_Channel_Right_GLOBAL_ENABLE &= ((uint8)(~RX_Channel_Right_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(RX_Channel_Right_COUNTER_LSB_PTR, counter);
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}
#endif /* (!(RX_Channel_Right_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: RX_Channel_Right_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 RX_Channel_Right_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(RX_Channel_Right_UsingFixedFunction)
		(void)CY_GET_REG16(RX_Channel_Right_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(RX_Channel_Right_COUNTER_LSB_PTR_8BIT);
	#endif/* (RX_Channel_Right_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(RX_Channel_Right_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(RX_Channel_Right_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(RX_Channel_Right_STATICCOUNT_LSB_PTR));
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Right_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 RX_Channel_Right_ReadCapture(void) 
{
    #if(RX_Channel_Right_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(RX_Channel_Right_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(RX_Channel_Right_STATICCOUNT_LSB_PTR));
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Right_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void RX_Channel_Right_WritePeriod(uint16 period) 
{
    #if(RX_Channel_Right_UsingFixedFunction)
        CY_SET_REG16(RX_Channel_Right_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(RX_Channel_Right_PERIOD_LSB_PTR, period);
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Right_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 RX_Channel_Right_ReadPeriod(void) 
{
    #if(RX_Channel_Right_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(RX_Channel_Right_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(RX_Channel_Right_PERIOD_LSB_PTR));
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}


#if (!RX_Channel_Right_UsingFixedFunction)
/*******************************************************************************
* Function Name: RX_Channel_Right_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void RX_Channel_Right_WriteCompare(uint16 compare) \
                                   
{
    #if(RX_Channel_Right_UsingFixedFunction)
        CY_SET_REG16(RX_Channel_Right_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(RX_Channel_Right_COMPARE_LSB_PTR, compare);
    #endif /* (RX_Channel_Right_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RX_Channel_Right_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 RX_Channel_Right_ReadCompare(void) 
{
    return (CY_GET_REG16(RX_Channel_Right_COMPARE_LSB_PTR));
}


#if (RX_Channel_Right_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: RX_Channel_Right_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void RX_Channel_Right_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    RX_Channel_Right_CONTROL &= ((uint8)(~RX_Channel_Right_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    RX_Channel_Right_CONTROL |= compareMode;
}
#endif  /* (RX_Channel_Right_COMPARE_MODE_SOFTWARE) */


#if (RX_Channel_Right_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: RX_Channel_Right_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void RX_Channel_Right_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    RX_Channel_Right_CONTROL &= ((uint8)(~RX_Channel_Right_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    RX_Channel_Right_CONTROL |= ((uint8)((uint8)captureMode << RX_Channel_Right_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (RX_Channel_Right_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: RX_Channel_Right_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void RX_Channel_Right_ClearFIFO(void) 
{

    while(0u != (RX_Channel_Right_ReadStatusRegister() & RX_Channel_Right_STATUS_FIFONEMP))
    {
        (void)RX_Channel_Right_ReadCapture();
    }

}
#endif  /* (!RX_Channel_Right_UsingFixedFunction) */


/* [] END OF FILE */

