/*******************************************************************************
* File Name: PWM_RESET_PM.c
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

#include "PWM_RESET.h"

/* Check for removal by optimization */
#if !defined(PWM_RESET_Sync_ctrl_reg__REMOVED)

static PWM_RESET_BACKUP_STRUCT  PWM_RESET_backup = {0u};

    
/*******************************************************************************
* Function Name: PWM_RESET_SaveConfig
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
void PWM_RESET_SaveConfig(void) 
{
    PWM_RESET_backup.controlState = PWM_RESET_Control;
}


/*******************************************************************************
* Function Name: PWM_RESET_RestoreConfig
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
void PWM_RESET_RestoreConfig(void) 
{
     PWM_RESET_Control = PWM_RESET_backup.controlState;
}


/*******************************************************************************
* Function Name: PWM_RESET_Sleep
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
void PWM_RESET_Sleep(void) 
{
    PWM_RESET_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_RESET_Wakeup
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
void PWM_RESET_Wakeup(void)  
{
    PWM_RESET_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
