/*******************************************************************************
* File Name: DIP_2.h  
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

#if !defined(CY_PINS_DIP_2_H) /* Pins DIP_2_H */
#define CY_PINS_DIP_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DIP_2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 DIP_2__PORT == 15 && ((DIP_2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    DIP_2_Write(uint8 value);
void    DIP_2_SetDriveMode(uint8 mode);
uint8   DIP_2_ReadDataReg(void);
uint8   DIP_2_Read(void);
void    DIP_2_SetInterruptMode(uint16 position, uint16 mode);
uint8   DIP_2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the DIP_2_SetDriveMode() function.
     *  @{
     */
        #define DIP_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define DIP_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define DIP_2_DM_RES_UP          PIN_DM_RES_UP
        #define DIP_2_DM_RES_DWN         PIN_DM_RES_DWN
        #define DIP_2_DM_OD_LO           PIN_DM_OD_LO
        #define DIP_2_DM_OD_HI           PIN_DM_OD_HI
        #define DIP_2_DM_STRONG          PIN_DM_STRONG
        #define DIP_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define DIP_2_MASK               DIP_2__MASK
#define DIP_2_SHIFT              DIP_2__SHIFT
#define DIP_2_WIDTH              1u

/* Interrupt constants */
#if defined(DIP_2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DIP_2_SetInterruptMode() function.
     *  @{
     */
        #define DIP_2_INTR_NONE      (uint16)(0x0000u)
        #define DIP_2_INTR_RISING    (uint16)(0x0001u)
        #define DIP_2_INTR_FALLING   (uint16)(0x0002u)
        #define DIP_2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define DIP_2_INTR_MASK      (0x01u) 
#endif /* (DIP_2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DIP_2_PS                     (* (reg8 *) DIP_2__PS)
/* Data Register */
#define DIP_2_DR                     (* (reg8 *) DIP_2__DR)
/* Port Number */
#define DIP_2_PRT_NUM                (* (reg8 *) DIP_2__PRT) 
/* Connect to Analog Globals */                                                  
#define DIP_2_AG                     (* (reg8 *) DIP_2__AG)                       
/* Analog MUX bux enable */
#define DIP_2_AMUX                   (* (reg8 *) DIP_2__AMUX) 
/* Bidirectional Enable */                                                        
#define DIP_2_BIE                    (* (reg8 *) DIP_2__BIE)
/* Bit-mask for Aliased Register Access */
#define DIP_2_BIT_MASK               (* (reg8 *) DIP_2__BIT_MASK)
/* Bypass Enable */
#define DIP_2_BYP                    (* (reg8 *) DIP_2__BYP)
/* Port wide control signals */                                                   
#define DIP_2_CTL                    (* (reg8 *) DIP_2__CTL)
/* Drive Modes */
#define DIP_2_DM0                    (* (reg8 *) DIP_2__DM0) 
#define DIP_2_DM1                    (* (reg8 *) DIP_2__DM1)
#define DIP_2_DM2                    (* (reg8 *) DIP_2__DM2) 
/* Input Buffer Disable Override */
#define DIP_2_INP_DIS                (* (reg8 *) DIP_2__INP_DIS)
/* LCD Common or Segment Drive */
#define DIP_2_LCD_COM_SEG            (* (reg8 *) DIP_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define DIP_2_LCD_EN                 (* (reg8 *) DIP_2__LCD_EN)
/* Slew Rate Control */
#define DIP_2_SLW                    (* (reg8 *) DIP_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DIP_2_PRTDSI__CAPS_SEL       (* (reg8 *) DIP_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DIP_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DIP_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DIP_2_PRTDSI__OE_SEL0        (* (reg8 *) DIP_2__PRTDSI__OE_SEL0) 
#define DIP_2_PRTDSI__OE_SEL1        (* (reg8 *) DIP_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DIP_2_PRTDSI__OUT_SEL0       (* (reg8 *) DIP_2__PRTDSI__OUT_SEL0) 
#define DIP_2_PRTDSI__OUT_SEL1       (* (reg8 *) DIP_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DIP_2_PRTDSI__SYNC_OUT       (* (reg8 *) DIP_2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(DIP_2__SIO_CFG)
    #define DIP_2_SIO_HYST_EN        (* (reg8 *) DIP_2__SIO_HYST_EN)
    #define DIP_2_SIO_REG_HIFREQ     (* (reg8 *) DIP_2__SIO_REG_HIFREQ)
    #define DIP_2_SIO_CFG            (* (reg8 *) DIP_2__SIO_CFG)
    #define DIP_2_SIO_DIFF           (* (reg8 *) DIP_2__SIO_DIFF)
#endif /* (DIP_2__SIO_CFG) */

/* Interrupt Registers */
#if defined(DIP_2__INTSTAT)
    #define DIP_2_INTSTAT            (* (reg8 *) DIP_2__INTSTAT)
    #define DIP_2_SNAP               (* (reg8 *) DIP_2__SNAP)
    
	#define DIP_2_0_INTTYPE_REG 		(* (reg8 *) DIP_2__0__INTTYPE)
#endif /* (DIP_2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_DIP_2_H */


/* [] END OF FILE */
