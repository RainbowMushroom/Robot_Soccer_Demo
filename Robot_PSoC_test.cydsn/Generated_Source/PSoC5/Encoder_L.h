/*******************************************************************************
* File Name: Encoder_L.h  
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

#if !defined(CY_PINS_Encoder_L_H) /* Pins Encoder_L_H */
#define CY_PINS_Encoder_L_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Encoder_L_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Encoder_L__PORT == 15 && ((Encoder_L__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Encoder_L_Write(uint8 value);
void    Encoder_L_SetDriveMode(uint8 mode);
uint8   Encoder_L_ReadDataReg(void);
uint8   Encoder_L_Read(void);
void    Encoder_L_SetInterruptMode(uint16 position, uint16 mode);
uint8   Encoder_L_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Encoder_L_SetDriveMode() function.
     *  @{
     */
        #define Encoder_L_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Encoder_L_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Encoder_L_DM_RES_UP          PIN_DM_RES_UP
        #define Encoder_L_DM_RES_DWN         PIN_DM_RES_DWN
        #define Encoder_L_DM_OD_LO           PIN_DM_OD_LO
        #define Encoder_L_DM_OD_HI           PIN_DM_OD_HI
        #define Encoder_L_DM_STRONG          PIN_DM_STRONG
        #define Encoder_L_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Encoder_L_MASK               Encoder_L__MASK
#define Encoder_L_SHIFT              Encoder_L__SHIFT
#define Encoder_L_WIDTH              1u

/* Interrupt constants */
#if defined(Encoder_L__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Encoder_L_SetInterruptMode() function.
     *  @{
     */
        #define Encoder_L_INTR_NONE      (uint16)(0x0000u)
        #define Encoder_L_INTR_RISING    (uint16)(0x0001u)
        #define Encoder_L_INTR_FALLING   (uint16)(0x0002u)
        #define Encoder_L_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Encoder_L_INTR_MASK      (0x01u) 
#endif /* (Encoder_L__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Encoder_L_PS                     (* (reg8 *) Encoder_L__PS)
/* Data Register */
#define Encoder_L_DR                     (* (reg8 *) Encoder_L__DR)
/* Port Number */
#define Encoder_L_PRT_NUM                (* (reg8 *) Encoder_L__PRT) 
/* Connect to Analog Globals */                                                  
#define Encoder_L_AG                     (* (reg8 *) Encoder_L__AG)                       
/* Analog MUX bux enable */
#define Encoder_L_AMUX                   (* (reg8 *) Encoder_L__AMUX) 
/* Bidirectional Enable */                                                        
#define Encoder_L_BIE                    (* (reg8 *) Encoder_L__BIE)
/* Bit-mask for Aliased Register Access */
#define Encoder_L_BIT_MASK               (* (reg8 *) Encoder_L__BIT_MASK)
/* Bypass Enable */
#define Encoder_L_BYP                    (* (reg8 *) Encoder_L__BYP)
/* Port wide control signals */                                                   
#define Encoder_L_CTL                    (* (reg8 *) Encoder_L__CTL)
/* Drive Modes */
#define Encoder_L_DM0                    (* (reg8 *) Encoder_L__DM0) 
#define Encoder_L_DM1                    (* (reg8 *) Encoder_L__DM1)
#define Encoder_L_DM2                    (* (reg8 *) Encoder_L__DM2) 
/* Input Buffer Disable Override */
#define Encoder_L_INP_DIS                (* (reg8 *) Encoder_L__INP_DIS)
/* LCD Common or Segment Drive */
#define Encoder_L_LCD_COM_SEG            (* (reg8 *) Encoder_L__LCD_COM_SEG)
/* Enable Segment LCD */
#define Encoder_L_LCD_EN                 (* (reg8 *) Encoder_L__LCD_EN)
/* Slew Rate Control */
#define Encoder_L_SLW                    (* (reg8 *) Encoder_L__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Encoder_L_PRTDSI__CAPS_SEL       (* (reg8 *) Encoder_L__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Encoder_L_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Encoder_L__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Encoder_L_PRTDSI__OE_SEL0        (* (reg8 *) Encoder_L__PRTDSI__OE_SEL0) 
#define Encoder_L_PRTDSI__OE_SEL1        (* (reg8 *) Encoder_L__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Encoder_L_PRTDSI__OUT_SEL0       (* (reg8 *) Encoder_L__PRTDSI__OUT_SEL0) 
#define Encoder_L_PRTDSI__OUT_SEL1       (* (reg8 *) Encoder_L__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Encoder_L_PRTDSI__SYNC_OUT       (* (reg8 *) Encoder_L__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Encoder_L__SIO_CFG)
    #define Encoder_L_SIO_HYST_EN        (* (reg8 *) Encoder_L__SIO_HYST_EN)
    #define Encoder_L_SIO_REG_HIFREQ     (* (reg8 *) Encoder_L__SIO_REG_HIFREQ)
    #define Encoder_L_SIO_CFG            (* (reg8 *) Encoder_L__SIO_CFG)
    #define Encoder_L_SIO_DIFF           (* (reg8 *) Encoder_L__SIO_DIFF)
#endif /* (Encoder_L__SIO_CFG) */

/* Interrupt Registers */
#if defined(Encoder_L__INTSTAT)
    #define Encoder_L_INTSTAT            (* (reg8 *) Encoder_L__INTSTAT)
    #define Encoder_L_SNAP               (* (reg8 *) Encoder_L__SNAP)
    
	#define Encoder_L_0_INTTYPE_REG 		(* (reg8 *) Encoder_L__0__INTTYPE)
#endif /* (Encoder_L__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Encoder_L_H */


/* [] END OF FILE */
