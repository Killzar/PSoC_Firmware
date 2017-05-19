/*******************************************************************************
* File Name: ESP07_UART.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_ESP07_UART_H)
#define CY_UART_ESP07_UART_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

#define ESP07_UART_RX_ENABLED                     (1u)
#define ESP07_UART_TX_ENABLED                     (1u)
#define ESP07_UART_HD_ENABLED                     (0u)
#define ESP07_UART_RX_INTERRUPT_ENABLED           (0u)
#define ESP07_UART_TX_INTERRUPT_ENABLED           (0u)
#define ESP07_UART_INTERNAL_CLOCK_USED            (1u)
#define ESP07_UART_RXHW_ADDRESS_ENABLED           (0u)
#define ESP07_UART_OVER_SAMPLE_COUNT              (8u)
#define ESP07_UART_PARITY_TYPE                    (0u)
#define ESP07_UART_PARITY_TYPE_SW                 (0u)
#define ESP07_UART_BREAK_DETECT                   (0u)
#define ESP07_UART_BREAK_BITS_TX                  (13u)
#define ESP07_UART_BREAK_BITS_RX                  (13u)
#define ESP07_UART_TXCLKGEN_DP                    (1u)
#define ESP07_UART_USE23POLLING                   (1u)
#define ESP07_UART_FLOW_CONTROL                   (0u)
#define ESP07_UART_CLK_FREQ                       (0u)
#define ESP07_UART_TX_BUFFER_SIZE                 (4u)
#define ESP07_UART_RX_BUFFER_SIZE                 (4u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define ESP07_UART_CONTROL_REG_REMOVED            (0u)
#else
    #define ESP07_UART_CONTROL_REG_REMOVED            (1u)
#endif /* End ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct ESP07_UART_backupStruct_
{
    uint8 enableState;

    #if(ESP07_UART_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End ESP07_UART_CONTROL_REG_REMOVED */

} ESP07_UART_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void ESP07_UART_Start(void) ;
void ESP07_UART_Stop(void) ;
uint8 ESP07_UART_ReadControlRegister(void) ;
void ESP07_UART_WriteControlRegister(uint8 control) ;

void ESP07_UART_Init(void) ;
void ESP07_UART_Enable(void) ;
void ESP07_UART_SaveConfig(void) ;
void ESP07_UART_RestoreConfig(void) ;
void ESP07_UART_Sleep(void) ;
void ESP07_UART_Wakeup(void) ;

/* Only if RX is enabled */
#if( (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) )

    #if (ESP07_UART_RX_INTERRUPT_ENABLED)
        #define ESP07_UART_EnableRxInt()  CyIntEnable (ESP07_UART_RX_VECT_NUM)
        #define ESP07_UART_DisableRxInt() CyIntDisable(ESP07_UART_RX_VECT_NUM)
        CY_ISR_PROTO(ESP07_UART_RXISR);
    #endif /* ESP07_UART_RX_INTERRUPT_ENABLED */

    void ESP07_UART_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void ESP07_UART_SetRxAddress1(uint8 address) ;
    void ESP07_UART_SetRxAddress2(uint8 address) ;

    void  ESP07_UART_SetRxInterruptMode(uint8 intSrc) ;
    uint8 ESP07_UART_ReadRxData(void) ;
    uint8 ESP07_UART_ReadRxStatus(void) ;
    uint8 ESP07_UART_GetChar(void) ;
    uint16 ESP07_UART_GetByte(void) ;
    uint8 ESP07_UART_GetRxBufferSize(void)
                                                            ;
    void ESP07_UART_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define ESP07_UART_GetRxInterruptSource   ESP07_UART_ReadRxStatus

#endif /* End (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) */

/* Only if TX is enabled */
#if(ESP07_UART_TX_ENABLED || ESP07_UART_HD_ENABLED)

    #if(ESP07_UART_TX_INTERRUPT_ENABLED)
        #define ESP07_UART_EnableTxInt()  CyIntEnable (ESP07_UART_TX_VECT_NUM)
        #define ESP07_UART_DisableTxInt() CyIntDisable(ESP07_UART_TX_VECT_NUM)
        #define ESP07_UART_SetPendingTxInt() CyIntSetPending(ESP07_UART_TX_VECT_NUM)
        #define ESP07_UART_ClearPendingTxInt() CyIntClearPending(ESP07_UART_TX_VECT_NUM)
        CY_ISR_PROTO(ESP07_UART_TXISR);
    #endif /* ESP07_UART_TX_INTERRUPT_ENABLED */

    void ESP07_UART_SetTxInterruptMode(uint8 intSrc) ;
    void ESP07_UART_WriteTxData(uint8 txDataByte) ;
    uint8 ESP07_UART_ReadTxStatus(void) ;
    void ESP07_UART_PutChar(uint8 txDataByte) ;
    void ESP07_UART_PutString(const char8 string[]) ;
    void ESP07_UART_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void ESP07_UART_PutCRLF(uint8 txDataByte) ;
    void ESP07_UART_ClearTxBuffer(void) ;
    void ESP07_UART_SetTxAddressMode(uint8 addressMode) ;
    void ESP07_UART_SendBreak(uint8 retMode) ;
    uint8 ESP07_UART_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define ESP07_UART_PutStringConst         ESP07_UART_PutString
    #define ESP07_UART_PutArrayConst          ESP07_UART_PutArray
    #define ESP07_UART_GetTxInterruptSource   ESP07_UART_ReadTxStatus

#endif /* End ESP07_UART_TX_ENABLED || ESP07_UART_HD_ENABLED */

#if(ESP07_UART_HD_ENABLED)
    void ESP07_UART_LoadRxConfig(void) ;
    void ESP07_UART_LoadTxConfig(void) ;
#endif /* End ESP07_UART_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_ESP07_UART) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    ESP07_UART_CyBtldrCommStart(void) CYSMALL ;
    void    ESP07_UART_CyBtldrCommStop(void) CYSMALL ;
    void    ESP07_UART_CyBtldrCommReset(void) CYSMALL ;
    cystatus ESP07_UART_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus ESP07_UART_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_ESP07_UART)
        #define CyBtldrCommStart    ESP07_UART_CyBtldrCommStart
        #define CyBtldrCommStop     ESP07_UART_CyBtldrCommStop
        #define CyBtldrCommReset    ESP07_UART_CyBtldrCommReset
        #define CyBtldrCommWrite    ESP07_UART_CyBtldrCommWrite
        #define CyBtldrCommRead     ESP07_UART_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_ESP07_UART) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define ESP07_UART_BYTE2BYTE_TIME_OUT (25u)
    #define ESP07_UART_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define ESP07_UART_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define ESP07_UART_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define ESP07_UART_SET_SPACE      (0x00u)
#define ESP07_UART_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (ESP07_UART_TX_ENABLED) || (ESP07_UART_HD_ENABLED) )
    #if(ESP07_UART_TX_INTERRUPT_ENABLED)
        #define ESP07_UART_TX_VECT_NUM            (uint8)ESP07_UART_TXInternalInterrupt__INTC_NUMBER
        #define ESP07_UART_TX_PRIOR_NUM           (uint8)ESP07_UART_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* ESP07_UART_TX_INTERRUPT_ENABLED */

    #define ESP07_UART_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define ESP07_UART_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define ESP07_UART_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(ESP07_UART_TX_ENABLED)
        #define ESP07_UART_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (ESP07_UART_HD_ENABLED) */
        #define ESP07_UART_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (ESP07_UART_TX_ENABLED) */

    #define ESP07_UART_TX_STS_COMPLETE            (uint8)(0x01u << ESP07_UART_TX_STS_COMPLETE_SHIFT)
    #define ESP07_UART_TX_STS_FIFO_EMPTY          (uint8)(0x01u << ESP07_UART_TX_STS_FIFO_EMPTY_SHIFT)
    #define ESP07_UART_TX_STS_FIFO_FULL           (uint8)(0x01u << ESP07_UART_TX_STS_FIFO_FULL_SHIFT)
    #define ESP07_UART_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << ESP07_UART_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (ESP07_UART_TX_ENABLED) || (ESP07_UART_HD_ENABLED)*/

#if( (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) )
    #if(ESP07_UART_RX_INTERRUPT_ENABLED)
        #define ESP07_UART_RX_VECT_NUM            (uint8)ESP07_UART_RXInternalInterrupt__INTC_NUMBER
        #define ESP07_UART_RX_PRIOR_NUM           (uint8)ESP07_UART_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* ESP07_UART_RX_INTERRUPT_ENABLED */
    #define ESP07_UART_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define ESP07_UART_RX_STS_BREAK_SHIFT             (0x01u)
    #define ESP07_UART_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define ESP07_UART_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define ESP07_UART_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define ESP07_UART_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define ESP07_UART_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define ESP07_UART_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define ESP07_UART_RX_STS_MRKSPC           (uint8)(0x01u << ESP07_UART_RX_STS_MRKSPC_SHIFT)
    #define ESP07_UART_RX_STS_BREAK            (uint8)(0x01u << ESP07_UART_RX_STS_BREAK_SHIFT)
    #define ESP07_UART_RX_STS_PAR_ERROR        (uint8)(0x01u << ESP07_UART_RX_STS_PAR_ERROR_SHIFT)
    #define ESP07_UART_RX_STS_STOP_ERROR       (uint8)(0x01u << ESP07_UART_RX_STS_STOP_ERROR_SHIFT)
    #define ESP07_UART_RX_STS_OVERRUN          (uint8)(0x01u << ESP07_UART_RX_STS_OVERRUN_SHIFT)
    #define ESP07_UART_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << ESP07_UART_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define ESP07_UART_RX_STS_ADDR_MATCH       (uint8)(0x01u << ESP07_UART_RX_STS_ADDR_MATCH_SHIFT)
    #define ESP07_UART_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << ESP07_UART_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define ESP07_UART_RX_HW_MASK                     (0x7Fu)
#endif /* End (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) */

/* Control Register definitions */
#define ESP07_UART_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define ESP07_UART_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define ESP07_UART_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define ESP07_UART_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define ESP07_UART_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define ESP07_UART_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define ESP07_UART_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define ESP07_UART_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define ESP07_UART_CTRL_HD_SEND               (uint8)(0x01u << ESP07_UART_CTRL_HD_SEND_SHIFT)
#define ESP07_UART_CTRL_HD_SEND_BREAK         (uint8)(0x01u << ESP07_UART_CTRL_HD_SEND_BREAK_SHIFT)
#define ESP07_UART_CTRL_MARK                  (uint8)(0x01u << ESP07_UART_CTRL_MARK_SHIFT)
#define ESP07_UART_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << ESP07_UART_CTRL_PARITY_TYPE0_SHIFT)
#define ESP07_UART_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << ESP07_UART_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define ESP07_UART_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define ESP07_UART_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define ESP07_UART_SEND_BREAK                         (0x00u)
#define ESP07_UART_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define ESP07_UART_REINIT                             (0x02u)
#define ESP07_UART_SEND_WAIT_REINIT                   (0x03u)

#define ESP07_UART_OVER_SAMPLE_8                      (8u)
#define ESP07_UART_OVER_SAMPLE_16                     (16u)

#define ESP07_UART_BIT_CENTER                         (ESP07_UART_OVER_SAMPLE_COUNT - 2u)

#define ESP07_UART_FIFO_LENGTH                        (4u)
#define ESP07_UART_NUMBER_OF_START_BIT                (1u)
#define ESP07_UART_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define ESP07_UART_TXBITCTR_BREAKBITS8X   ((ESP07_UART_BREAK_BITS_TX * ESP07_UART_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define ESP07_UART_TXBITCTR_BREAKBITS ((ESP07_UART_BREAK_BITS_TX * ESP07_UART_OVER_SAMPLE_COUNT) - 1u)

#define ESP07_UART_HALF_BIT_COUNT   \
                            (((ESP07_UART_OVER_SAMPLE_COUNT / 2u) + (ESP07_UART_USE23POLLING * 1u)) - 2u)
#if (ESP07_UART_OVER_SAMPLE_COUNT == ESP07_UART_OVER_SAMPLE_8)
    #define ESP07_UART_HD_TXBITCTR_INIT   (((ESP07_UART_BREAK_BITS_TX + \
                            ESP07_UART_NUMBER_OF_START_BIT) * ESP07_UART_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define ESP07_UART_RXBITCTR_INIT  ((((ESP07_UART_BREAK_BITS_RX + ESP07_UART_NUMBER_OF_START_BIT) \
                            * ESP07_UART_OVER_SAMPLE_COUNT) + ESP07_UART_HALF_BIT_COUNT) - 1u)

#else /* ESP07_UART_OVER_SAMPLE_COUNT == ESP07_UART_OVER_SAMPLE_16 */
    #define ESP07_UART_HD_TXBITCTR_INIT   ((8u * ESP07_UART_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define ESP07_UART_RXBITCTR_INIT      (((7u * ESP07_UART_OVER_SAMPLE_COUNT) - 1u) + \
                                                      ESP07_UART_HALF_BIT_COUNT)
#endif /* End ESP07_UART_OVER_SAMPLE_COUNT */

#define ESP07_UART_HD_RXBITCTR_INIT                   ESP07_UART_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 ESP07_UART_initVar;
#if (ESP07_UART_TX_INTERRUPT_ENABLED && ESP07_UART_TX_ENABLED)
    extern volatile uint8 ESP07_UART_txBuffer[ESP07_UART_TX_BUFFER_SIZE];
    extern volatile uint8 ESP07_UART_txBufferRead;
    extern uint8 ESP07_UART_txBufferWrite;
#endif /* (ESP07_UART_TX_INTERRUPT_ENABLED && ESP07_UART_TX_ENABLED) */
#if (ESP07_UART_RX_INTERRUPT_ENABLED && (ESP07_UART_RX_ENABLED || ESP07_UART_HD_ENABLED))
    extern uint8 ESP07_UART_errorStatus;
    extern volatile uint8 ESP07_UART_rxBuffer[ESP07_UART_RX_BUFFER_SIZE];
    extern volatile uint8 ESP07_UART_rxBufferRead;
    extern volatile uint8 ESP07_UART_rxBufferWrite;
    extern volatile uint8 ESP07_UART_rxBufferLoopDetect;
    extern volatile uint8 ESP07_UART_rxBufferOverflow;
    #if (ESP07_UART_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 ESP07_UART_rxAddressMode;
        extern volatile uint8 ESP07_UART_rxAddressDetected;
    #endif /* (ESP07_UART_RXHW_ADDRESS_ENABLED) */
#endif /* (ESP07_UART_RX_INTERRUPT_ENABLED && (ESP07_UART_RX_ENABLED || ESP07_UART_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define ESP07_UART__B_UART__AM_SW_BYTE_BYTE 1
#define ESP07_UART__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define ESP07_UART__B_UART__AM_HW_BYTE_BY_BYTE 3
#define ESP07_UART__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define ESP07_UART__B_UART__AM_NONE 0

#define ESP07_UART__B_UART__NONE_REVB 0
#define ESP07_UART__B_UART__EVEN_REVB 1
#define ESP07_UART__B_UART__ODD_REVB 2
#define ESP07_UART__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define ESP07_UART_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define ESP07_UART_NUMBER_OF_STOP_BITS    (1u)

#if (ESP07_UART_RXHW_ADDRESS_ENABLED)
    #define ESP07_UART_RX_ADDRESS_MODE    (0u)
    #define ESP07_UART_RX_HW_ADDRESS1     (0u)
    #define ESP07_UART_RX_HW_ADDRESS2     (0u)
#endif /* (ESP07_UART_RXHW_ADDRESS_ENABLED) */

#define ESP07_UART_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << ESP07_UART_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << ESP07_UART_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << ESP07_UART_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << ESP07_UART_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << ESP07_UART_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << ESP07_UART_RX_STS_BREAK_SHIFT) \
                                        | (0 << ESP07_UART_RX_STS_OVERRUN_SHIFT))

#define ESP07_UART_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << ESP07_UART_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << ESP07_UART_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << ESP07_UART_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << ESP07_UART_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define ESP07_UART_CONTROL_REG \
                            (* (reg8 *) ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define ESP07_UART_CONTROL_PTR \
                            (  (reg8 *) ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(ESP07_UART_TX_ENABLED)
    #define ESP07_UART_TXDATA_REG          (* (reg8 *) ESP07_UART_BUART_sTX_TxShifter_u0__F0_REG)
    #define ESP07_UART_TXDATA_PTR          (  (reg8 *) ESP07_UART_BUART_sTX_TxShifter_u0__F0_REG)
    #define ESP07_UART_TXDATA_AUX_CTL_REG  (* (reg8 *) ESP07_UART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define ESP07_UART_TXDATA_AUX_CTL_PTR  (  (reg8 *) ESP07_UART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define ESP07_UART_TXSTATUS_REG        (* (reg8 *) ESP07_UART_BUART_sTX_TxSts__STATUS_REG)
    #define ESP07_UART_TXSTATUS_PTR        (  (reg8 *) ESP07_UART_BUART_sTX_TxSts__STATUS_REG)
    #define ESP07_UART_TXSTATUS_MASK_REG   (* (reg8 *) ESP07_UART_BUART_sTX_TxSts__MASK_REG)
    #define ESP07_UART_TXSTATUS_MASK_PTR   (  (reg8 *) ESP07_UART_BUART_sTX_TxSts__MASK_REG)
    #define ESP07_UART_TXSTATUS_ACTL_REG   (* (reg8 *) ESP07_UART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define ESP07_UART_TXSTATUS_ACTL_PTR   (  (reg8 *) ESP07_UART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(ESP07_UART_TXCLKGEN_DP)
        #define ESP07_UART_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define ESP07_UART_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define ESP07_UART_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define ESP07_UART_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define ESP07_UART_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define ESP07_UART_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define ESP07_UART_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define ESP07_UART_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define ESP07_UART_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define ESP07_UART_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) ESP07_UART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* ESP07_UART_TXCLKGEN_DP */

#endif /* End ESP07_UART_TX_ENABLED */

#if(ESP07_UART_HD_ENABLED)

    #define ESP07_UART_TXDATA_REG             (* (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__F1_REG )
    #define ESP07_UART_TXDATA_PTR             (  (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__F1_REG )
    #define ESP07_UART_TXDATA_AUX_CTL_REG     (* (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define ESP07_UART_TXDATA_AUX_CTL_PTR     (  (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define ESP07_UART_TXSTATUS_REG           (* (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_REG )
    #define ESP07_UART_TXSTATUS_PTR           (  (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_REG )
    #define ESP07_UART_TXSTATUS_MASK_REG      (* (reg8 *) ESP07_UART_BUART_sRX_RxSts__MASK_REG )
    #define ESP07_UART_TXSTATUS_MASK_PTR      (  (reg8 *) ESP07_UART_BUART_sRX_RxSts__MASK_REG )
    #define ESP07_UART_TXSTATUS_ACTL_REG      (* (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define ESP07_UART_TXSTATUS_ACTL_PTR      (  (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End ESP07_UART_HD_ENABLED */

#if( (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) )
    #define ESP07_UART_RXDATA_REG             (* (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__F0_REG )
    #define ESP07_UART_RXDATA_PTR             (  (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__F0_REG )
    #define ESP07_UART_RXADDRESS1_REG         (* (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__D0_REG )
    #define ESP07_UART_RXADDRESS1_PTR         (  (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__D0_REG )
    #define ESP07_UART_RXADDRESS2_REG         (* (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__D1_REG )
    #define ESP07_UART_RXADDRESS2_PTR         (  (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__D1_REG )
    #define ESP07_UART_RXDATA_AUX_CTL_REG     (* (reg8 *) ESP07_UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define ESP07_UART_RXBITCTR_PERIOD_REG    (* (reg8 *) ESP07_UART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define ESP07_UART_RXBITCTR_PERIOD_PTR    (  (reg8 *) ESP07_UART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define ESP07_UART_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) ESP07_UART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define ESP07_UART_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) ESP07_UART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define ESP07_UART_RXBITCTR_COUNTER_REG   (* (reg8 *) ESP07_UART_BUART_sRX_RxBitCounter__COUNT_REG )
    #define ESP07_UART_RXBITCTR_COUNTER_PTR   (  (reg8 *) ESP07_UART_BUART_sRX_RxBitCounter__COUNT_REG )

    #define ESP07_UART_RXSTATUS_REG           (* (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_REG )
    #define ESP07_UART_RXSTATUS_PTR           (  (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_REG )
    #define ESP07_UART_RXSTATUS_MASK_REG      (* (reg8 *) ESP07_UART_BUART_sRX_RxSts__MASK_REG )
    #define ESP07_UART_RXSTATUS_MASK_PTR      (  (reg8 *) ESP07_UART_BUART_sRX_RxSts__MASK_REG )
    #define ESP07_UART_RXSTATUS_ACTL_REG      (* (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define ESP07_UART_RXSTATUS_ACTL_PTR      (  (reg8 *) ESP07_UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) */

#if(ESP07_UART_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define ESP07_UART_INTCLOCK_CLKEN_REG     (* (reg8 *) ESP07_UART_IntClock__PM_ACT_CFG)
    #define ESP07_UART_INTCLOCK_CLKEN_PTR     (  (reg8 *) ESP07_UART_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define ESP07_UART_INTCLOCK_CLKEN_MASK    ESP07_UART_IntClock__PM_ACT_MSK
#endif /* End ESP07_UART_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(ESP07_UART_TX_ENABLED)
    #define ESP07_UART_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End ESP07_UART_TX_ENABLED */

#if(ESP07_UART_HD_ENABLED)
    #define ESP07_UART_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End ESP07_UART_HD_ENABLED */

#if( (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) )
    #define ESP07_UART_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define ESP07_UART_WAIT_1_MS      ESP07_UART_BL_CHK_DELAY_MS   

#define ESP07_UART_TXBUFFERSIZE   ESP07_UART_TX_BUFFER_SIZE
#define ESP07_UART_RXBUFFERSIZE   ESP07_UART_RX_BUFFER_SIZE

#if (ESP07_UART_RXHW_ADDRESS_ENABLED)
    #define ESP07_UART_RXADDRESSMODE  ESP07_UART_RX_ADDRESS_MODE
    #define ESP07_UART_RXHWADDRESS1   ESP07_UART_RX_HW_ADDRESS1
    #define ESP07_UART_RXHWADDRESS2   ESP07_UART_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define ESP07_UART_RXAddressMode  ESP07_UART_RXADDRESSMODE
#endif /* (ESP07_UART_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define ESP07_UART_initvar                    ESP07_UART_initVar

#define ESP07_UART_RX_Enabled                 ESP07_UART_RX_ENABLED
#define ESP07_UART_TX_Enabled                 ESP07_UART_TX_ENABLED
#define ESP07_UART_HD_Enabled                 ESP07_UART_HD_ENABLED
#define ESP07_UART_RX_IntInterruptEnabled     ESP07_UART_RX_INTERRUPT_ENABLED
#define ESP07_UART_TX_IntInterruptEnabled     ESP07_UART_TX_INTERRUPT_ENABLED
#define ESP07_UART_InternalClockUsed          ESP07_UART_INTERNAL_CLOCK_USED
#define ESP07_UART_RXHW_Address_Enabled       ESP07_UART_RXHW_ADDRESS_ENABLED
#define ESP07_UART_OverSampleCount            ESP07_UART_OVER_SAMPLE_COUNT
#define ESP07_UART_ParityType                 ESP07_UART_PARITY_TYPE

#if( ESP07_UART_TX_ENABLED && (ESP07_UART_TXBUFFERSIZE > ESP07_UART_FIFO_LENGTH))
    #define ESP07_UART_TXBUFFER               ESP07_UART_txBuffer
    #define ESP07_UART_TXBUFFERREAD           ESP07_UART_txBufferRead
    #define ESP07_UART_TXBUFFERWRITE          ESP07_UART_txBufferWrite
#endif /* End ESP07_UART_TX_ENABLED */
#if( ( ESP07_UART_RX_ENABLED || ESP07_UART_HD_ENABLED ) && \
     (ESP07_UART_RXBUFFERSIZE > ESP07_UART_FIFO_LENGTH) )
    #define ESP07_UART_RXBUFFER               ESP07_UART_rxBuffer
    #define ESP07_UART_RXBUFFERREAD           ESP07_UART_rxBufferRead
    #define ESP07_UART_RXBUFFERWRITE          ESP07_UART_rxBufferWrite
    #define ESP07_UART_RXBUFFERLOOPDETECT     ESP07_UART_rxBufferLoopDetect
    #define ESP07_UART_RXBUFFER_OVERFLOW      ESP07_UART_rxBufferOverflow
#endif /* End ESP07_UART_RX_ENABLED */

#ifdef ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define ESP07_UART_CONTROL                ESP07_UART_CONTROL_REG
#endif /* End ESP07_UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(ESP07_UART_TX_ENABLED)
    #define ESP07_UART_TXDATA                 ESP07_UART_TXDATA_REG
    #define ESP07_UART_TXSTATUS               ESP07_UART_TXSTATUS_REG
    #define ESP07_UART_TXSTATUS_MASK          ESP07_UART_TXSTATUS_MASK_REG
    #define ESP07_UART_TXSTATUS_ACTL          ESP07_UART_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(ESP07_UART_TXCLKGEN_DP)
        #define ESP07_UART_TXBITCLKGEN_CTR        ESP07_UART_TXBITCLKGEN_CTR_REG
        #define ESP07_UART_TXBITCLKTX_COMPLETE    ESP07_UART_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define ESP07_UART_TXBITCTR_PERIOD        ESP07_UART_TXBITCTR_PERIOD_REG
        #define ESP07_UART_TXBITCTR_CONTROL       ESP07_UART_TXBITCTR_CONTROL_REG
        #define ESP07_UART_TXBITCTR_COUNTER       ESP07_UART_TXBITCTR_COUNTER_REG
    #endif /* ESP07_UART_TXCLKGEN_DP */
#endif /* End ESP07_UART_TX_ENABLED */

#if(ESP07_UART_HD_ENABLED)
    #define ESP07_UART_TXDATA                 ESP07_UART_TXDATA_REG
    #define ESP07_UART_TXSTATUS               ESP07_UART_TXSTATUS_REG
    #define ESP07_UART_TXSTATUS_MASK          ESP07_UART_TXSTATUS_MASK_REG
    #define ESP07_UART_TXSTATUS_ACTL          ESP07_UART_TXSTATUS_ACTL_REG
#endif /* End ESP07_UART_HD_ENABLED */

#if( (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) )
    #define ESP07_UART_RXDATA                 ESP07_UART_RXDATA_REG
    #define ESP07_UART_RXADDRESS1             ESP07_UART_RXADDRESS1_REG
    #define ESP07_UART_RXADDRESS2             ESP07_UART_RXADDRESS2_REG
    #define ESP07_UART_RXBITCTR_PERIOD        ESP07_UART_RXBITCTR_PERIOD_REG
    #define ESP07_UART_RXBITCTR_CONTROL       ESP07_UART_RXBITCTR_CONTROL_REG
    #define ESP07_UART_RXBITCTR_COUNTER       ESP07_UART_RXBITCTR_COUNTER_REG
    #define ESP07_UART_RXSTATUS               ESP07_UART_RXSTATUS_REG
    #define ESP07_UART_RXSTATUS_MASK          ESP07_UART_RXSTATUS_MASK_REG
    #define ESP07_UART_RXSTATUS_ACTL          ESP07_UART_RXSTATUS_ACTL_REG
#endif /* End  (ESP07_UART_RX_ENABLED) || (ESP07_UART_HD_ENABLED) */

#if(ESP07_UART_INTERNAL_CLOCK_USED)
    #define ESP07_UART_INTCLOCK_CLKEN         ESP07_UART_INTCLOCK_CLKEN_REG
#endif /* End ESP07_UART_INTERNAL_CLOCK_USED */

#define ESP07_UART_WAIT_FOR_COMLETE_REINIT    ESP07_UART_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_ESP07_UART_H */


/* [] END OF FILE */
