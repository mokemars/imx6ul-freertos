/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : switch.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines interrupt service routine functions for the switch
*                  interrupts. De-bounces switch and sets a key press flag. 
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Switch handler function definitions */
#include "switch.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Switch flag global variable */
volatile uint8_t gSwitchFlag = 0x00u;
/* Switch standby ready global variable */
volatile bool gSwitchStandbyReady = true;
/* Detected switch faults counter variable */
volatile uint16_t gSwitchFaultsDetected = 0;
/* Switch press callback pointer declaration */
void (*gSwitchPressCallbackFunc)(void) = NULL;
/* Switch release callback pointer declaration */
void (*gSwitchReleaseCallbackFunc)(void) = NULL;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Start debounce timer function prototype */ 
void StartDebounceTimer(uint16_t);
/* Switch debounce timer callback function prototype */
void SwitchDebounceCB(void);
/* Switch 1 press callback function prototype */
static void SW1PressCB(void);
/* Switch 2 press callback function prototype */
static void SW2PressCB(void);
/* Switch 3 press callback function prototype */
static void SW3PressCB(void);

/*******************************************************************************
* Outline       : InitialiseSwitchInterrupts
* Description   : Configures the IRQ pins connected to switches SW1-SW3 to
*                  detect switch presses, and invoke an ISR.
* Argument      : none
* Return value  : none
*******************************************************************************/
void InitialiseSwitchInterrupts(void)
{        
    /* Reset the switch flag */
    gSwitchFlag = 0x00u;
    
    /* Reset the switch standby ready flag */
    gSwitchStandbyReady = 0x1u;
    
    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00u;
    
    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40u;

    /* Set IRQ10 on Port0 as an input pin */
    MPC.P02PFS.BIT.ISEL = 0x1u;
    /* Select the pin function for the lower 5 bits */
       MPC.P02PFS.BIT.PSEL = 0x0u;
    
    /* Set IRQ8 on Port4 as an input pin */
    MPC.P00PFS.BIT.ISEL = 0x1u;
    /* Select the pin function for the lower 5 bits */
    MPC.P00PFS.BIT.PSEL = 0x0u;
   
    /* Set IRQ15 on Port0 as an input pin */
    MPC.P07PFS.BIT.ISEL = 0x1u;
    /* Select the pin function for the lower 5 bits */
    MPC.P07PFS.BIT.PSEL = 0x0u;
    
    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80u;
    
    /* Disable digital pin filtering for all IRQ pins */
    ICU.IRQFLTE1.BYTE = 0x0u;
    
    /* Configure switch SW1 (IRQ10) */
    /* Clear SW1 interrupt requests */
    IEN(ICU, IRQ10) = 0x0u;    
    /* Set P3_2 (SW2) as an input */
    PORT0.PDR.BIT.B2 = 0x0u;
    /* Set detection direction as falling edge */
    ICU.IRQCR[10].BIT.IRQMD = 0x1u;    
    /* Clear IR flag */
    IR(ICU, IRQ10) = 0x0u;                      
    /* Set interrupt priority level to 7 */
    IPR(ICU, IRQ10) = SWITCH_IPL;    
    /* Enable SW1 interrupt requests */
    IEN(ICU, IRQ10) = 0x1u;            
                              
    /* Configure IRQ8 interrupt (connected to SW2) */
    /* Clear SW2 interrupt requests */
    IEN(ICU, IRQ8) = 0x0u;    
    /* Set P3_2 (SW2) as an input */
    PORT4.PDR.BIT.B4 = 0x0u;
    /* Set detection direction as falling edge */
    ICU.IRQCR[8].BIT.IRQMD = 0x1u;    
    /* Clear IR flag */
    IR(ICU, IRQ8) = 0x0u;                      
    /* Set interrupt priority level to 7 */
    IPR(ICU, IRQ8) = SWITCH_IPL;    
    /* Enable SW2 interrupt requests */
    IEN(ICU, IRQ8) = 0x1u;                          
                            
    /* Configure IRQ15 interrupt (connected to SW3) */
    /* Clear SW3 interrupt requests */
    IEN(ICU, IRQ15) = 0x0u;    
    /* Set P3_2 (SW3) as an input */
    PORT0.PDR.BIT.B7 = 0x0u;
    /* Set detection direction as falling edge */
    ICU.IRQCR[15].BIT.IRQMD = 0x1u;    
    /* Clear IR flag */
    IR(ICU, IRQ15) = 0x0u;
    /* Set interrupt priority level to 7 */
    IPR(ICU, IRQ15) = SWITCH_IPL;    
    /* Enable SW3 interrupt requests */
    IEN(ICU, IRQ15) = 0x1u;
}
/******************************************************************************
* End of InitialiseSwitchInterrupts
******************************************************************************/

/******************************************************************************
* Outline       : SetSwitchPressCallback
* Description   : Takes a pointer to a function, and sets it as the callback
*                  function for the switch interrupts. The passed function is
*                  executed every time any switch is pressed down.
* Argument      : pointer to callback function (set to NULL to disable)
* Return value  : none
******************************************************************************/
void SetSwitchPressCallback(void(*callback)(void))
{
    /* Store the callback function pointer into the global variable */
    gSwitchPressCallbackFunc = callback;
}
/******************************************************************************
* End of SetSwitchPressCallback
******************************************************************************/

/******************************************************************************
* Outline       : SetSwitchReleaseCallback
* Description   : Takes a pointer to a function, and sets it as the callback
*                  function for the switch interrupts. The passed function is
*                  executed every time any switch is released.
* Argument      : pointer to callback function (set to NULL to disable)
* Return value  : none
******************************************************************************/
void SetSwitchReleaseCallback(void(*callback)(void))
{
    /* Store the callback function pointer into the global variable */
    gSwitchReleaseCallbackFunc = callback;
}
/******************************************************************************
* End of SetSwitchReleaseCallback
******************************************************************************/

/******************************************************************************
* Outline       : ControlSwitchInterrupts
* Description   : Enables or disables the switch IRQ interrupts, based on the
*                  input variable, control.
* Argument      : uint8_t - 0x1  : Enables switch interrupts.
*                            0x0  : Disables switch interrupts.
* Return value  : none
******************************************************************************/
void ControlSwitchInterrupts(uint8_t control)
{    
    /* Check if control input is 0x1 */
    if(control)
    {
        /* Enable SW1 interrupt requests */
        IEN(ICU, IRQ10) = 0x1u;
    
        /* Enable SW2 interrupt requests */
        IEN(ICU, IRQ8) = 0x1u;
    
        /* Enable SW3 interrupt requests */
        IEN(ICU, IRQ15) = 0x1u;
    }
    /* Control input is 0x0 */
    else
    {    
        /* Disable SW1 interrupts */
        IEN(ICU, IRQ10) = 0x0u;
        
        /* Disable SW2 interrupts */
        IEN(ICU, IRQ8) = 0x0u;
    
        /* Disable SW3 interrupts */
        IEN(ICU, IRQ15) = 0x0u;
    }
}    
/******************************************************************************
* End of function ControlSwitchInterrupts
******************************************************************************/    
    
/******************************************************************************
* Outline       : Excep_IRQ10
* Description   : Switch 1 callback ISR function. The function disables switch
*                  interrupts, then uses the debounce timer to re-enable them
*                  after the debounce period finishes.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ10
__interrupt void Excep_IRQ10(void)
{        
    /* Disable switch 1 interrupts */
    IEN(ICU, IRQ10) = 0x0u;
    
    /* Set standby ready flag as false */
    gSwitchStandbyReady = false;
                        
    /* Check if interrupt was generated by falling edge */
    if(0x1 == ICU.IRQCR[10u].BIT.IRQMD)
    {
        /* Start debounce timer */
        StartDebounceTimer(DEBOUNCE_SHORT);        
        
        /* Set detection direction as rising edge */
        ICU.IRQCR[10u].BIT.IRQMD = 0x2u;
                
        /* Set global switch flag to indicate SW1 is held down */
        gSwitchFlag |= SWITCHHOLD_1;
    }
    else
    {    
        /* Start debounce timer */
        StartDebounceTimer(DEBOUNCE_LONG);        
            
        /* Set detection direction to falling edge */
        ICU.IRQCR[10u].BIT.IRQMD = 0x1u;
        
        /* Clear SW1 held-down flag bit in switch flag */
        gSwitchFlag &= (uint8_t)(0xF0u | ~SWITCHHOLD_1);
        
        /* Set global switch flag to indicate SW1 press complete */
        gSwitchFlag |= SWITCHPRESS_1;
    
        /* Check if switch release callback function is not NULL */
        if(gSwitchReleaseCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchReleaseCallbackFunc();
        }
    }
}
/******************************************************************************
* End of function SW1_InterruptCB
******************************************************************************/    

/******************************************************************************
* Outline       : Excep_IRQ8
* Description   : Switch 2 callback ISR function. The function disables switch
*                  interrupts, then uses the debounce timer to re-enable them
*                  after the debounce period finishes.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ8
__interrupt void Excep_IRQ8(void)
{        
    /* Disable switch 2 interrupts */
    IEN(ICU, IRQ12) = 0x0u;
    
    /* Set standby ready flag as false */
    gSwitchStandbyReady = false;
        
    /* Check if interrupt triggered from falling edge */
    if(0x1 == ICU.IRQCR[8].BIT.IRQMD)
    {
        /* Start debounce timer */
        StartDebounceTimer(DEBOUNCE_SHORT);
        
        /* Set detection direction as rising edge */
        ICU.IRQCR[8].BIT.IRQMD = 0x2u;
        
        /* Set global switch flag to indicate SW2 is held down */
        gSwitchFlag |= SWITCHHOLD_2;
    }
    else
    {        
        /* Start debounce timer */
        StartDebounceTimer(DEBOUNCE_LONG);
        
        /* Set detection direction to falling edge */
        ICU.IRQCR[8].BIT.IRQMD = 0x1u;
        
        /* Clear SW2 held-down flag bit in switch flag */
        gSwitchFlag &= (uint8_t)(0xF0u | ~SWITCHHOLD_2);
        
        /* Set global switch flag to indicate SW2 press complete */
        gSwitchFlag |= SWITCHPRESS_2;
    
        /* Check if switch release callback function is not NULL */
        if(gSwitchReleaseCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchReleaseCallbackFunc();
        }
    }
}
/******************************************************************************
* End of function SW2_InterruptCB
******************************************************************************/    

/******************************************************************************
* Outline       : Excep_IRQ15
* Description   : Switch 3 callback ISR function. The function disables switch
*                  interrupts, then uses the debounce timer to re-enable them
*                  after the debounce period finishes.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ15
__interrupt void Excep_IRQ15(void)
{
    /* Disable switch 3 interrupts */
    IEN(ICU, IRQ15) = 0x0u;    
    
    /* Set standby ready flag as false */
    gSwitchStandbyReady = false;
                
    /* Check if detection direction is set to falling edge */
    if(0x1 == ICU.IRQCR[15].BIT.IRQMD)
    {
        /* Start debounce timer */
        StartDebounceTimer(DEBOUNCE_SHORT);
        
        /* Set detection direction as rising edge */
        ICU.IRQCR[15].BIT.IRQMD = 0x2u;
        
        /* Set global switch flag to indicate SW3 is held down */
        gSwitchFlag |= SWITCHHOLD_3;
    }
    else
    {
        /* Start debounce timer */
        StartDebounceTimer(DEBOUNCE_LONG);        
        
        /* Set detection direction to falling edge */
        ICU.IRQCR[15].BIT.IRQMD = 0x1u;
        
        /* Clear SW3 held-down flag bit in switch flag */
        gSwitchFlag &= (uint8_t)(0xF0u | ~SWITCHHOLD_3);
        
        /* Set global switch flag to indicate SW1 press complete */
        gSwitchFlag |= SWITCHPRESS_3;
    
        /* Check if switch release callback function is not NULL */
        if(gSwitchReleaseCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchReleaseCallbackFunc();
        }
    }
}
/******************************************************************************
* End of function SW3_InterruptCB
******************************************************************************/    

/******************************************************************************
* Outline       : SwitchDebounceCB
* Description   : Switch debounce timer callback function. Function is executed
*                  by the CMT channel ISR. Function re-enables all switch all
*                  switch interrupts; then verifies a correct switch press was
*                  detected, and calls the user switch press callback function.
* Argument      : none
* Return value  : none
******************************************************************************/
void SwitchDebounceCB(void)
{    
    /* Clear IRQ10 interrupt flag */
    IR(ICU, IRQ10) = 0x0u;
    
    /* Re-enable switch 1 interrupts */
    IEN(ICU, IRQ10) = 0x1u;

    /* Clear IRQ8 interrupt flag */
    IR(ICU, IRQ8) = 0x0u;

    /* Re-enable switch 2 interrupts */
    IEN(ICU, IRQ8) = 0x1;

    /* Clear IRQ15 interrupt flag */
    IR(ICU, IRQ15) = 0x0u;

    /* Re-enable switch 3 interrupts */
    IEN(ICU, IRQ15) = 0x1u;
    
    /* Check if switch 1 hold flag is set */ 
    if((gSwitchFlag & 0x0Fu) & SWITCHHOLD_1)
    {
        /* Execute switch press callback function */
        SW1PressCB();
    }
    
    /* Check if switch 2 hold flag is set */     
    if((gSwitchFlag & 0x0F) & SWITCHHOLD_2)
    {
        /* Execute switch press callback function */
        SW2PressCB();
    }
    
    /* Check if switch 3 hold flag is set */ 
    if((gSwitchFlag & 0x0Fu) & SWITCHHOLD_3)
    {
        /* Execute switch press callback function */
        SW3PressCB();
    }
    
    /* Check if any switches are held down */
    if(0x00u == (gSwitchFlag & 0x0Fu))
    {        
        /* Set standby ready flag to true */
        gSwitchStandbyReady = true;
    }
        
}
/******************************************************************************
* End of function SwitchDebounceCB
******************************************************************************/

/******************************************************************************
* Outline       : SW1PressCB
* Description   : Switch press callback function (internal). The Function
*                 verifies that the switch pin level is low before executing
*                 the user switch press callback function. If the pin level is
*                 high, the switch press duration was too short and the function
*                 increments the gSwitchFaultsDetected counter.
* Argument      : none
* Return value  : none
******************************************************************************/
static void SW1PressCB(void)
{
  /* Check if switch 1 pin level is high (switch press duration
           less than debounce, invalid switch press) */
    if(PORT0.PIDR.BIT.B2)
    {
        /* Reset detection direction to falling edge */
        ICU.IRQCR[10].BIT.IRQMD = 0x1;
        
        /* Clear switch press flag */
        gSwitchFlag &= (uint8_t)(0xF0u | ~SWITCHHOLD_1);
            
        /* Increment detected switch faults counter */
        gSwitchFaultsDetected++;
    }
    /* Switch 1 pin level is low (valid switch press) */
    else
    {
        /* Check if switch press callback function is not NULL */
        if(gSwitchPressCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchPressCallbackFunc();
        }
    }
}
/******************************************************************************
* End of function SW1PressCB
******************************************************************************/

/******************************************************************************
* Outline       : SW2PressCB
* Description   : Switch press callback function (internal). The Function
*                 verifies that the switch pin level is low before executing
*                 the user switch press callback function. If the pin level is
*                 high, the switch press duration was too short and the function
*                 increments the gSwitchFaultsDetected counter.
* Argument      : none
* Return value  : none
******************************************************************************/
static void SW2PressCB(void)
{
   /* Check if switch 2 pin level is high (switch press duration
       less than debounce, invalid switch press) */
    if(PORT0.PIDR.BIT.B0)
    {
        /* Reset detection direction to falling edge */
        ICU.IRQCR[8].BIT.IRQMD = 0x1;
            
        /* Clear switch press flag */
        gSwitchFlag &= (uint8_t)(0xF0u | ~SWITCHHOLD_2);
            
        /* Increment detected switch faults counter */
        gSwitchFaultsDetected++;
    }
    /* Switch 2 pin level is low (valid switch press) */
    else
    {
        /* Check if switch press callback function is not NULL */
        if(gSwitchPressCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchPressCallbackFunc();
        }
    }
}
/******************************************************************************
* End of function SW2PressCB
******************************************************************************/

/******************************************************************************
* Outline       : SW3PressCB
* Description   : Switch press callback function (internal). The Function
*                 verifies that the switch pin level is low before executing
*                 the user switch press callback function. If the pin level is
*                 high, the switch press duration was too short and the function
*                 increments the gSwitchFaultsDetected counter.
* Argument      : none
* Return value  : none
******************************************************************************/
static void SW3PressCB(void)
{
    /* Check if switch 3 pin level is high (switch press duration
           less than debounce, invalid switch press) */
    if(PORT0.PIDR.BIT.B7)
    {
        /* Reset detection direction to falling edge */
        ICU.IRQCR[15].BIT.IRQMD = 0x1;
        
        /* Clear switch press flag */
        gSwitchFlag &= (uint8_t)(0xF0u | ~SWITCHHOLD_3);
            
        /* Increment detected switch faults counter */
        gSwitchFaultsDetected++;
    }
    /* Switch 3 pin level is low (valid switch press) */
    else
    {
        /* Check if switch press callback function is not NULL */
        if(gSwitchPressCallbackFunc)
        {
            /* Execute user callback function */
            gSwitchPressCallbackFunc();
        }
    }
}
/******************************************************************************
* End of function SW3PressCB
******************************************************************************/

/******************************************************************************
* Outline       : StartDebounceTimer
* Description   : Function initialises the CMT timer (the first time the
*                  function is called), and starts the CMT timer to perform
*                  generate the switch debounce interrupt.
* Argument      : uint16_t compare_match    : compare match value to trigger
*                                              *interrupt at.
* Return value  : none
******************************************************************************/
void StartDebounceTimer(uint16_t compare_match)
{
    /* Declare local static variable to track if the CMT timer has been 
        initialised yet */
    static bool timer_initialised = false;
    
    /* Check if the CMT timer is not initialised (first time function has been
       called). */
    if(!timer_initialised)
    {    
        /* Disable register protection */
        SYSTEM.PRCR.WORD = 0xA50B;    
    
        /* Enable the CMT0 module */
        MSTP_CMT0 = 0;
    
        /* Re-enable register protection */
        SYSTEM.PRCR.BIT.PRKEY = 0xA5u;
        SYSTEM.PRCR.WORD &= 0xFF00u;    
    
        /* Set CMT0 clock source as PLCK/512 */
        CMT0.CMCR.BIT.CKS = 0x2;
     
        /* Enable compare match interrupt */
        CMT0.CMCR.BIT.CMIE = 1;
    
        /* Enable CMT0 interrupt request */
        IEN(CMT0, CMI0) = 1;
    
        /* Set interrupt priority to 12 */
        IPR(CMT0, CMI0) = 12u;
        
        /* Set the timer inititalised flag */
        timer_initialised = true;
    }
    
    /* Set compare match to to generate debounce period */
    CMT0.CMCOR = compare_match;
    
    /* Reset count to zero */
    CMT0.CMCNT = 0x0000;
        
    /* Start timer */
    CMT.CMSTR0.BIT.STR0 = 1;
}
/******************************************************************************
* End of function StartDebounceTimer
******************************************************************************/

/******************************************************************************
* Outline       : Excep_CMTU0_CMT0
* Description   : Interupt service routine for the CMT channel 0 compare
*                  interrupt. Fun
* Argument      : uint16_t compare_match    : compare match value to trigger
*                                              *interrupt at.
* Return value  : none
******************************************************************************/
#pragma vector=VECT_CMT0_CMI0
__interrupt void Excep_CMT0_CMI0(void)
{
    /* Stop the CMT0 timer running */
    CMT.CMSTR0.BIT.STR0 = 0;
    
    /* Call the switch debounce callback function */
    SwitchDebounceCB();
}
/******************************************************************************
* End of function Excep_CMTU0_CMT0
******************************************************************************/
