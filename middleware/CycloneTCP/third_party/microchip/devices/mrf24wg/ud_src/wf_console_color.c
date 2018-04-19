/*******************************************************************************
 VT100 codes used for Universal Driver Demos

  Summary: VT100 Codes

  Description: Same as summary.
*******************************************************************************/

/* MRF24WG0M Universal Driver
*
* Copyright (c) 2012-2014, Microchip Technology, Inc. <www.microchip.com>
* Contact Microchip for the latest version.
*
* This program is free software; distributed under the terms of BSD
* license:
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1.    Redistributions of source code must retain the above copyright notice, this
*        list of conditions and the following disclaimer.
* 2.    Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
* 3.    Neither the name(s) of the above-listed copyright holder(s) nor the names
*        of its contributors may be used to endorse or promote products derived
*        from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdint.h>

#include "wf_console_color.h"

#include "wf_customize.h"
#include "printf.h"

#if defined(WF_USE_DEBUG_OUTPUT) || defined(WF_USE_CONSOLE)



#include "wf_console.h"

#if defined(WF_CONSOLE_USE_COLOR)

//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
// VT100 foreground colors
const char Black[] = {ESC_KEY, '[', '3', '0', 'm', '\0'};     // foreground black
const char Red[]   = {ESC_KEY, '[', '3', '1', 'm', '\0'};     // foreground red
const char Green[] = {ESC_KEY, '[', '3', '2', 'm', '\0'};     // foreground green
#if 0
const char Cyan[]  = {ESC_KEY, '[', '3', '6', 'm', '\0'};     // foreground green
const char Blue[]  = {ESC_KEY, '[', '3', '4', 'm', '\0'};     // foreground blue
#endif

// VT100 bold, dim
#if 0
const char Normal[]  = {ESC_KEY, '[', '0', 'm', '\0'};          // normal text
const char Bold[]    = {ESC_KEY, '[', '1', 'm', '\0'};          // bold text
const char Dim[]     = {ESC_KEY, '[', '2', 'm', '\0'};          // dim text
#endif


// sets VT100 terminal foreground color
void SetForeground(uint8_t color)
{
    if (color == BLACK)
    {
        WF_PRINTF("%s", Black);
    }
    else if (color == GREEN)
    {
        WF_PRINTF("%s", Green);
    }
    else  if (color == RED)
    {
        WF_PRINTF("%s", Red);
    }
}
#endif // WF_COLOR_OUTPUT



#endif // WF_USE_DEBUG_OUTPUT or WF_USE_CONSOLE
