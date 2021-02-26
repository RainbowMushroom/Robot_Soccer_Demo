/*******************************************************************************
* File Name: Encoder_L1.h  
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

#if !defined(CY_PINS_Encoder_L1_H) /* Pins Encoder_L1_H */
#define CY_PINS_Encoder_L1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Encoder_L1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Encoder_L1__PORT == 15 && ((Encoder_L1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Encoder_L1_Write(uint8 value);
void    Encoder_L1_SetDriveMode(uint8 mode);
uint8   Encoder_L1_ReadDataReg(void);
uint8   Encoder_L1_Read(void);
void    Encoder_L1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Encoder_L1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Encoder_L1_SetDriveMode() function.
     *  @{
     */
        #define Encoder_L1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Encoder_L1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Encoder_L1_DM_RES_UP          PIN_DM_RES_UP
        #define Encoder_L1_DM_RES_DWN         PIN_DM_RES_DWN
        #define Encoder_L1_DM_OD_LO           PIN_DM_OD_LO
        #define Encoder_L1_DM_OD_HI           PIN_DM_OD_HI
        #define Encoder_L1_DM_STRONG          PIN_DM_STRONG
        #define Encoder_L1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Encoder_L1_MASK               Encoder_L1__MASK
#define Encoder_L1_SHIFT              Encoder_L1__SHIFT
#define Encoder_L1_WIDTH              1u

/* Interrupt constants */
#if defined(Encoder_L1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Encoder_L1_SetInterruptMode() function.
     *  @{
     */
        #define Encoder_L1_INTR_NONE      (uint16)(0x0000u)
        #define Encoder_L1_INTR_RISING    (uint16)(0x0001u)
        #define Encoder_L1_INTR_FALLING   (uint16)(0x0002u)
        #define Encoder_L1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Encoder_L1_INTR_MASK      (0x01u) 
#endif /* (Encoder_L1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Encoder_L1_PS                     (* (reg8 *) Encoder_L1__PS)
/* Data Register */
#define Encoder_L1_DR                     (* (reg8 *) Encoder_L1__DR)
/* Port Number */
#define Encoder_L1_PRT_NUM                (* (reg8 *) Encoder_L1__PRT) 
/* Connect to Analog Globals */                                                  
#define Encoder_L1_AG                     (* (reg8 *) Encoder_L1__AG)                       
/* Analog MUX bux enable */
#define Encoder_L1_AMUX                   (* (reg8 *) Encoder_L1__AMUX) 
/* Bidirectional Enable */                                                        
#define Encoder_L1_BIE                    (* (reg8 *) Encoder_L1__BIE)
/* Bit-mask for Aliased Register Access */
#define Encoder_L1_BIT_MASK               (* (reg8 *) Encoder_L1__BIT_MASK)
/* Bypass Enable */
#define Encoder_L1_BYP                    (* (reg8 *) Encoder_L1__BYP)
/* Port wide control signals */                                                   
#define Encoder_L1_CTL                    (* (reg8 *) Encoder_L1__CTL)
/* Drive Modes */
#define Encoder_L1_DM0                    (* (reg8 *) Encoder_L1__DM0) 
#define Encoder_L1_DM1                    (* (reg8 *) Encoder_L1__DM1)
#define Encoder_L1_DM2                    (* (reg8 *) Encoder_L1__DM2) 
/* Input Buffer Disable Override */
#define Encoder_L1_INP_DIS                (* (reg8 *) Encoder_L1__INP_DIS)
/* LCD Common or Segment Drive */
#define Encoder_L1_LCD_COM_SEG            (* (reg8 *) Encoder_L1__LCD_COM_SEG)
/* Enable Segment LCD */
#define Encoder_L1_LCD_EN                 (* (reg8 *) Encoder_L1__LCD_EN)
/* Slew Rate Control */
#define Encoder_L1_SLW                    (* (reg8 *) Encoder_L1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Encoder_L1_PRTDSI__CAPS_SEL       (* (reg8 *) Encoder_L1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Encoder_L1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Encoder_L1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Encoder_L1_PRTDSI__OE_SEL0        (* (reg8 *) Encoder_L1__PRTDSI__OE_SEL0) 
#define Encoder_L1_PRTDSI__OE_SEL1        (* (reg8 *) Encoder_L1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Encoder_L1_PRTDSI__OUT_SEL0       (* (reg8 *) Encoder_L1__PRTDSI__OUT_SEL0) 
#define Encoder_L1_PRTDSI__OUT_SEL1       (* (reg8 *) Encoder_L1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Encoder_L1_PRTDSI__SYNC_OUT       (* (reg8 *) Encoder_L1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Encoder_L1__SIO_CFG)
    #define Encoder_L1_SIO_HYST_EN        (* (reg8 *) Encoder_L1__SIO_HYST_EN)
    #define Encoder_L1_SIO_REG_HIFREQ     (* (reg8 *) Encoder_L1__SIO_REG_HIFREQ)
    #define Encoder_L1_SIO_CFG            (* (reg8 *) Encoder_L1__SIO_CFG)
    #define Encoder_L1_SIO_DIFF           (* (reg8 *) Encoder_L1__SIO_DIFF)
#endif /* (Encoder_L1__SIO_CFG) */

/* Interrupt Registers */
#if defined(Encoder_L1__INTSTAT)
    #define Encoder_L1_INTSTAT            (* (reg8 *) Encoder_L1__INTSTAT)
    #define Encoder_L1_SNAP               (* (reg8 *) Encoder_L1__SNAP)
    
	#define Encoder_L1_0_INTTYPE_REG 		(* (reg8 *) Encoder_L1__0__INTTYPE)
#endif /* (Encoder_L1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Encoder_L1_H */


/* [] END OF FILE */
