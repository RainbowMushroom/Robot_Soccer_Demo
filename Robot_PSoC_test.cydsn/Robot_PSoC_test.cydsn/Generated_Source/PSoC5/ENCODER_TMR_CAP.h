/*******************************************************************************
* File Name: ENCODER_TMR_CAP.h  
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

#if !defined(CY_CONTROL_REG_ENCODER_TMR_CAP_H) /* CY_CONTROL_REG_ENCODER_TMR_CAP_H */
#define CY_CONTROL_REG_ENCODER_TMR_CAP_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} ENCODER_TMR_CAP_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    ENCODER_TMR_CAP_Write(uint8 control) ;
uint8   ENCODER_TMR_CAP_Read(void) ;

void ENCODER_TMR_CAP_SaveConfig(void) ;
void ENCODER_TMR_CAP_RestoreConfig(void) ;
void ENCODER_TMR_CAP_Sleep(void) ; 
void ENCODER_TMR_CAP_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define ENCODER_TMR_CAP_Control        (* (reg8 *) ENCODER_TMR_CAP_Sync_ctrl_reg__CONTROL_REG )
#define ENCODER_TMR_CAP_Control_PTR    (  (reg8 *) ENCODER_TMR_CAP_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_ENCODER_TMR_CAP_H */


/* [] END OF FILE */
