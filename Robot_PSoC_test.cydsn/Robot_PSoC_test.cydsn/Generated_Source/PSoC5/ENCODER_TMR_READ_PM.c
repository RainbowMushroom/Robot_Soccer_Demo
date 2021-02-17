/*******************************************************************************
* File Name: ENCODER_TMR_READ_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "ENCODER_TMR_READ.h"

/* Check for removal by optimization */
#if !defined(ENCODER_TMR_READ_Sync_ctrl_reg__REMOVED)

static ENCODER_TMR_READ_BACKUP_STRUCT  ENCODER_TMR_READ_backup = {0u};

    
/*******************************************************************************
* Function Name: ENCODER_TMR_READ_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ENCODER_TMR_READ_SaveConfig(void) 
{
    ENCODER_TMR_READ_backup.controlState = ENCODER_TMR_READ_Control;
}


/*******************************************************************************
* Function Name: ENCODER_TMR_READ_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void ENCODER_TMR_READ_RestoreConfig(void) 
{
     ENCODER_TMR_READ_Control = ENCODER_TMR_READ_backup.controlState;
}


/*******************************************************************************
* Function Name: ENCODER_TMR_READ_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ENCODER_TMR_READ_Sleep(void) 
{
    ENCODER_TMR_READ_SaveConfig();
}


/*******************************************************************************
* Function Name: ENCODER_TMR_READ_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ENCODER_TMR_READ_Wakeup(void)  
{
    ENCODER_TMR_READ_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
