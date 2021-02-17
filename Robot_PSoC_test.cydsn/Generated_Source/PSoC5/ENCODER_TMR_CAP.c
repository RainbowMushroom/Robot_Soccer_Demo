/*******************************************************************************
* File Name: ENCODER_TMR_CAP.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware control of a Control Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "ENCODER_TMR_CAP.h"

/* Check for removal by optimization */
#if !defined(ENCODER_TMR_CAP_Sync_ctrl_reg__REMOVED)

    
/*******************************************************************************
* Function Name: ENCODER_TMR_CAP_Write
********************************************************************************
*
* Summary:
*  Write a byte to the Control Register.
*
* Parameters:
*  control:  The value to be assigned to the Control Register.
*
* Return:
*  None.
*
*******************************************************************************/
void ENCODER_TMR_CAP_Write(uint8 control) 
{
    ENCODER_TMR_CAP_Control = control;
}


/*******************************************************************************
* Function Name: ENCODER_TMR_CAP_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Control Register.
*
* Parameters:
*  None.
*
* Return:
*  Returns the current value in the Control Register.
*
*******************************************************************************/
uint8 ENCODER_TMR_CAP_Read(void) 
{
    return ENCODER_TMR_CAP_Control;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
