/*******************************************************************************
 Universal Driver console input

  Summary: Handles console cmd input.

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
#include <ctype.h>

#include "wf_universal_driver.h"
#include "wf_global_includes.h"


#if defined(WF_USE_CONSOLE)

#include "wf_console_color.h"
#include "wf_console.h"
#include "wf_console_cmds.h"

#define MAX_RX_BYTES            16
#define MAX_CMD_LINE_LENGTH     60
#define MAX_ESC_SEQUENCE_LENGTH 6

#define PROMPT_STR              "> "
#define PROMPT_STR_LENGTH       sizeof(PROMPT_STR)

#if (WF_CONSOLE_HISTORY_COUNT > 0)
#define PREV_CMD        0
#define NEXT_CMD        1
#endif

// states for escape sequence
enum
{
    WAIT_FOR_LEFT_BRACKET                   = 0,
    WAIT_FOR_LEFT_BRACKET_PLUS_ONE          = 1,
    WAIT_FOR_LEFT_BRACKET_PLUS_TWO          = 2,
    WAIT_FOR_LEFT_BRACKET_PLUS_THREE        = 3,
    WAIT_FOR_LEFT_BRACKET_PLUS_FOUR         = 4
};

//---------------
// Supported keys
//---------------
#define CTRL_C          0x03
#define BACKSPACE_KEY   0x08
#define TAB_KEY         0x09
#define ENTER_KEY       0x0d
#define DELETE_KEY      0x7f
#define CTRL_R          0x12

// three-character esc sequences
const uint8_t UP_ARROW_KEY[]    = {ESC_KEY, '[', 'A'};
const uint8_t DOWN_ARROW_KEY[]  = {ESC_KEY, '[', 'B'};
const uint8_t RIGHT_ARROW_KEY[] = {ESC_KEY, '[', 'C'};
const uint8_t LEFT_ARROW_KEY[]  = {ESC_KEY, '[', 'D'};
#if 0
const uint8_t SHIFT_TAB[]       = {ESC_KEY, '[', 'Z'};
#endif

// four-character esc sequences
const uint8_t HOME_KEY[]        = {ESC_KEY, '[', '1', '~'};
const uint8_t INSERT_KEY[]      = {ESC_KEY, '[', '2', '~'};
const uint8_t DEL_KEY[]         = {ESC_KEY, '[', '3', '~'};
const uint8_t END_KEY[]         = {ESC_KEY, '[', '4', '~'};
#if 0
const uint8_t PAGE_UP_KEY[]     = {ESC_KEY, '[', '5', '~'};
const uint8_t PAGE_DOWN_KEY[]   = {ESC_KEY, '[', '6', '~'};
#endif

// five-character esc sequences
const uint8_t F1_KEY[]          = {ESC_KEY, '[', '1', '1', '~'};
#if 0
const uint8_t F2_KEY[]          = {ESC_KEY, '[', '1', '2', '~'};
const uint8_t F3_KEY[]          = {ESC_KEY, '[', '1', '3', '~'};
const uint8_t F4_KEY[]          = {ESC_KEY, '[', '1', '4', '~'};
const uint8_t F5_KEY[]          = {ESC_KEY, '[', '1', '5', '~'};
const uint8_t F6_KEY[]          = {ESC_KEY, '[', '1', '7', '~'};
const uint8_t F7_KEY[]          = {ESC_KEY, '[', '1', '8', '~'};
const uint8_t F8_KEY[]          = {ESC_KEY, '[', '1', '9', '~'};
const uint8_t F9_KEY[]          = {ESC_KEY, '[', '2', '0', '~'};
const uint8_t F10_KEY[]         = {ESC_KEY, '[', '2', '1', '~'};
const uint8_t F11_KEY[]         = {ESC_KEY, '[', '2', '3', '~'};
const uint8_t F12_KEY[]         = {ESC_KEY, '[', '2', '4', '~'};
#endif

//const uint8_t CURSOR_FORWARD[]                      = {ESC_KEY, '[', '1', 'C', 0x00};
//const uint8_t CURSOR_BACKWARD[]                     = {ESC_KEY, '[', '1', 'D', 0x00};
const uint8_t CURSOR_HOME[]                         = {ESC_KEY, '[', 'f', 0x00};
const uint8_t ERASE_FROM_CURSOR_TO_END_OF_LINE[]    = {ESC_KEY, '[', 'K', 0x00};
#if 0
const uint8_t ERASE_ENTIRE_LINE[]                   = {ESC_KEY, '[', '2', 'K', 0x00};
#endif
const uint8_t UNDERLINE_MODE[]                      = {ESC_KEY, '[', '4', 'm', 0x00};
const uint8_t NORMAL_MODE[]                         = {ESC_KEY, '[', 'm', 0x00};
const uint8_t INSERT_MODE[]                         = {ESC_KEY, '[', '4', 'h', 0x00};
const uint8_t REPLACE_MODE[]                        = {ESC_KEY, '[', '4', 'l', 0x00};
const uint8_t DELETE_CHAR_AT_CURSOR[]               = {ESC_KEY, '[', 'P', 0x00};

const uint8_t CURSOR_INVISIBLE[]                    = {ESC_KEY, '[', '?', '2', '5', 'l', 0};
const uint8_t CURSOR_VISIBLE[]                      = {ESC_KEY, '[', '?', '2', '5', 'h', 0};

const uint8_t CURSOR_BLINK_BLOCK[]                  = {ESC_KEY, '[', '0', ' ', 'q', 0};
const uint8_t CURSOR_BLINK_VERTICAL[]               = {ESC_KEY, '[', '5', ' ', 'q', 0};
#if 0
const uint8_t CURSOR_STEADY_BLOCK[]                 = {ESC_KEY, '[', '2', ' ', 'q', 0};
const uint8_t CURSOR_BLINK_UNDERLINE[]              = {ESC_KEY, '[', '3', ' ', 'q', 0};
const uint8_t CURSOR_STEADY_UNDERLINE[]             = {ESC_KEY, '[', '4', ' ', 'q', 0};
const uint8_t CURSOR_STEADY_VERTICAL[]              = {ESC_KEY, '[', '6', ' ', 'q', 0};
#endif

const uint8_t ERASE_SCREEN[]                        = {ESC_KEY, '[', '2', 'J', 0};

#if (WF_CONSOLE_HISTORY_COUNT > 0)
typedef struct
{
    char     buf[WF_CONSOLE_HISTORY_COUNT][MAX_CMD_LINE_LENGTH + 1];  // N lines of history
    uint8_t  index;
    bool     seeded;
    int8_t   recallIndex;
} t_cmdHistory;
#endif

typedef struct consoleRxQueueStruct
{
   uint8_t  writeIndex;
   uint8_t  readIndex;
   char     bytes[MAX_RX_BYTES + 1]; // one unused position
} t_consoleRxQueue;

//==============================================================================
//                                  MACROS
//==============================================================================
#define IncrementReadIndex()                            \
    if (g_consoleRxQueue.readIndex == MAX_RX_BYTES)     \
    {                                                   \
        g_consoleRxQueue.readIndex = 0;                 \
    }                                                   \
    else                                                \
    {                                                   \
        ++g_consoleRxQueue.readIndex;                   \
    }

#define IncrementWriteIndex()                           \
    if (g_consoleRxQueue.writeIndex == MAX_RX_BYTES)    \
    {                                                   \
        g_consoleRxQueue.writeIndex = 0;                \
    }                                                   \
    else                                                \
    {                                                   \
        ++g_consoleRxQueue.writeIndex;                  \
    }



//==============================================================================
//                                  LOCAL GLOBALS
//==============================================================================
static t_consoleRxQueue g_consoleRxQueue;
static char             g_cmdLine[MAX_CMD_LINE_LENGTH + 1];
static uint8_t          g_cursorIndex;
static bool             g_escSequenceInProgress;
static uint8_t          g_escState;
static char             g_escSequence[MAX_ESC_SEQUENCE_LENGTH];
static bool             g_insertMode;

#if (WF_CONSOLE_HISTORY_COUNT > 0)
static t_cmdHistory     g_cmdHistory;
static bool             g_bumpFlag = false;
#endif

//==============================================================================
//                                  LOCAL FUNCTION PROTOTYPES
//==============================================================================
static bool     isConsoleQFull(void);
static bool     isConsoleQEmpty(void);
static char     ConsoleByteDequeue(void);
static void     CmdLineClear(void);
static void     Enter(void);
static void     ProcessCtrlC(void);
static void     ProcessRxCharacter(uint8_t c);
static bool     isEscSequenceInProgress(void);
static void     ProcessAsciiChar(char byte);
static void     ProcessEscapeSequence(char rxByte);
static void     Backspace(void);
static void     Delete(void);
static void     ProcessArrow(char rxByte);
static void     ProcessFourCharacterEscSequence(char *p_escSeq);
static void     ProcessFiveCharacterEscSequence(char *p_escSeq);
static void     CursorForward(uint8_t count);
static void     CursorBackward(uint8_t count);
static void     DeleteCharAtCursor(void);
static void     ToggleInsertMode(void);
static void     InsertMode(void);
static void     ReplaceMode(void);


#if (WF_CONSOLE_HISTORY_COUNT > 0)
static void EraseLine(void);
static void InitHistory(void);
static void DisplayHistoryEntry(uint8_t action);
static void AddCmdToHistory(char *p_cmd);
#endif

void ClearScreen(void)
{
    WF_PRINTF("%s", ERASE_SCREEN);
    OutputPrompt();
}

void ConsoleQInit(void)
{
    InitConsoleCmd();

    // start with empty character queue
    memset(&g_consoleRxQueue, 0x00, sizeof(g_consoleRxQueue));
    g_consoleRxQueue.readIndex  = 0;
    g_consoleRxQueue.writeIndex = 0;

    // default to character insert mode
    InsertMode();

    // reset command line buffer to empty
    CmdLineClear();

#if (WF_CONSOLE_HISTORY_COUNT > 0)
    InitHistory();
#endif

#if !defined(WF_USE_DEBUG_OUPUT)
    // if only using console then output prompt to show some activity on screen
    OutputPrompt();
#endif
}

// increment write index, then write
void ConsoleByteEnqueue(char byte)
{
    // if console queue is full than throw away the character
    if (isConsoleQFull())
    {
        WF_PRINTF("Console Queue full!\r\n");
        return;
    }
    // write data to queue
    g_consoleRxQueue.bytes[g_consoleRxQueue.writeIndex] = byte;
    IncrementWriteIndex();
}


// removes a character from queue filled in by UART Rx interrupt
static char ConsoleByteDequeue(void)
{
    uint8_t byte;

    // console queue should never be empty
    if (isConsoleQEmpty())
    {
        // TBD: error event
        WF_PRINTF("Console Queue Empty!\r\n");
        return ESC_KEY;
    }

    // read byte out of queue
    byte = g_consoleRxQueue.bytes[g_consoleRxQueue.readIndex];
    IncrementReadIndex();

    return byte;
}

static bool isConsoleQEmpty(void)
{
    return (g_consoleRxQueue.readIndex == g_consoleRxQueue.writeIndex);
}

// queue full when writeIndex + 1 = readIndex
static bool isConsoleQFull(void)
{
    uint8_t readIndex  = g_consoleRxQueue.readIndex;
    uint8_t writeIndex = g_consoleRxQueue.writeIndex;

    // check wrap case first
    if ((writeIndex == MAX_RX_BYTES) && (readIndex == 0))
    {
        return true;
    }
    // else do regular check
    else if ( (writeIndex + 1) == readIndex )
    {
        return true;
    }

    return false;
}

// called by WF_Task()
void ConsoleStateMachine(void)
{
    char rxByte;

    // while Rx bytes to process
    while (!isConsoleQEmpty())
    {
        // read next byte
        rxByte = ConsoleByteDequeue();

        // if ESC sequence in progress
        if (isEscSequenceInProgress())
        {
            ProcessEscapeSequence(rxByte);
        }
        // else ESC sequence not in progress
        else
        {
            ProcessRxCharacter(rxByte);
        }
    }

    // if user has typed scan command and scan results ready to be displayed
    if (isScanResultsDisplayEnabled())
    {
        DisplayScanResults();  // display the next scan result
    }
}

static void ProcessEscapeSequence(char rxByte)
{
    switch (g_escState)
    {
        case WAIT_FOR_LEFT_BRACKET:
        //--------------------------------------------
            // if we got the '['
            if (rxByte == '[')
            {
                g_escSequence[1] = '[';
                g_escState = WAIT_FOR_LEFT_BRACKET_PLUS_ONE;
            }
            // else did not get expected '['
            else
            {
                // if we got anything other than another ESC key then we're done, user pressed ESC key
                if (rxByte != ESC_KEY)
                {
                    g_escSequenceInProgress = false;

                    // add character to command line
                    ProcessRxCharacter(rxByte);
                }
                // else got another ESC key
                else
                {
                    // stay in this state
                }
            }
            break;

        case WAIT_FOR_LEFT_BRACKET_PLUS_ONE:
        //--------------------------------------------
            g_escSequence[2] = rxByte;
            if ((rxByte == 'A') || (rxByte == 'B') || (rxByte == 'C') || (rxByte == 'D'))
            {
                ProcessArrow(rxByte);
            }
            // else if <SHIFT><TAB> pressed
            else if (rxByte == 'Z')
            {
               g_escSequenceInProgress = false;  // ignore keypress
            }
            else if ((rxByte >= '0') || (rxByte <= '9'))
            {
                g_escState = WAIT_FOR_LEFT_BRACKET_PLUS_TWO;
            }
            else
            {
                WF_PRINTF("Unknown plus one\r\n");
            }
            break;

        case WAIT_FOR_LEFT_BRACKET_PLUS_TWO:
        //--------------------------------------------
            g_escSequence[3] = rxByte;
            if (rxByte == '~')
            {
                ProcessFourCharacterEscSequence(g_escSequence);
            }
            else
            {
                g_escState = WAIT_FOR_LEFT_BRACKET_PLUS_THREE;
            }
            break;

        case WAIT_FOR_LEFT_BRACKET_PLUS_THREE:
        //--------------------------------------------
            g_escSequence[4] = rxByte;
            if (rxByte == '~')
            {
                ProcessFiveCharacterEscSequence(g_escSequence);
            }
            else
            {
                WF_PRINTF("Unknown six char\r\n");
            }
            break;
    }
}


static void ProcessRxCharacter(uint8_t rxByte)
{
    switch (rxByte)
    {
        case ENTER_KEY:
            Enter();
            break;

        case BACKSPACE_KEY:
            Backspace();
            break;

        case TAB_KEY:
            // ignore
            break;

        case DELETE_KEY:
            Delete();
            break;

        case CTRL_R:
            ToggleInsertMode();
            break;

        case CTRL_C:
            ProcessCtrlC();
            break;

        case ESC_KEY:
            g_escSequenceInProgress = true;
            memset(g_escSequence, 0x00, sizeof(g_escSequence));
            g_escSequence[0] = ESC_KEY;
            g_escState = WAIT_FOR_LEFT_BRACKET;
            break;

        default:
            if (isprint(rxByte))
            {
                ProcessAsciiChar(rxByte);
            }
            break;

    }

}

static void InsertMode(void)
{
    WF_PRINTF("%s", INSERT_MODE);  // put the terminal in insert mode
    WF_PRINTF("%s", CURSOR_BLINK_VERTICAL);
    g_insertMode = true;
}

static void ReplaceMode(void)
{
    WF_PRINTF("%s", REPLACE_MODE);
    WF_PRINTF("%s", CURSOR_BLINK_BLOCK);
    g_insertMode = false;
}

static void ToggleInsertMode(void)
{
    if (g_insertMode == true)
    {
        ReplaceMode();
    }
    else
    {
        InsertMode();
    }
}

static void ProcessFourCharacterEscSequence(char *p_escSeq)
{
    if (memcmp(p_escSeq, HOME_KEY, sizeof(HOME_KEY)) == 0)
    {
        CursorBackward(g_cursorIndex);
    }
    else if (memcmp(p_escSeq, INSERT_KEY, sizeof(INSERT_KEY)) == 0)
    {
        WF_PRINTF("Insert\r\n");
    }
    else if (memcmp(p_escSeq, END_KEY, sizeof(END_KEY)) == 0)
    {
        CursorForward(strlen(g_cmdLine) - g_cursorIndex);
    }
    else if (memcmp(p_escSeq, DEL_KEY, sizeof(DEL_KEY)) == 0)
    {
        Delete();
    }


    g_escSequenceInProgress = false;
}

static void ProcessFiveCharacterEscSequence(char *p_escSeq)
{
    if (memcmp(p_escSeq, F1_KEY, sizeof(F1_KEY)) == 0)
    {
        ProcessConsoleCmd("help");
        OutputPrompt();
    }

    g_escSequenceInProgress = false;
}

static void CursorForward(uint8_t count)
{
    char buf[10];
    uint8_t len = strlen(g_cmdLine);
    uint8_t endIndex = g_cursorIndex + count;

    // if cursor at end of line, or line is empty or input count is 0
    if ((endIndex > len) || (len == 0) || (count == 0))
    {
        return;
    }

    buf[0] = ESC_KEY;
    buf[1] = '[';

    sprintf(&buf[2], "%dC", count);
    WF_PRINTF("%s", buf);
    g_cursorIndex = endIndex;
}

static void CursorBackward(uint8_t count)
{
    char buf[10];

    // if cursor at start of line or input count is 0
    if ((g_cursorIndex == 0) || (count == 0))
    {
        return;
    }

    buf[0] = ESC_KEY;
    buf[1] = '[';
    sprintf(&buf[2], "%dD", count);
    WF_PRINTF("%s", buf);
    g_cursorIndex -= count;
}

static void ProcessArrow(char rxByte)
{
    // if up arrow
    if (rxByte == 'A')
    {
#if (WF_CONSOLE_HISTORY_COUNT > 0)
        DisplayHistoryEntry(PREV_CMD);
#endif
    }
    // else if down arrow
    else if (rxByte == 'B')
    {
#if (WF_CONSOLE_HISTORY_COUNT > 0)
        DisplayHistoryEntry(NEXT_CMD);
#endif
    }
    // else if right arrow
    else if (rxByte == 'C')
    {
        CursorForward(1);
    }
    // else left arrow
    else
    {
        CursorBackward(1);
    }

    g_escSequenceInProgress = false;
}


static void DeleteCharAtCursor(void)
{
    int i;
    int len = strlen(g_cmdLine);

    // Update terminal
    WF_PRINTF("%s", DELETE_CHAR_AT_CURSOR);
    
    // Update cmd buffer by removing character at cursor index
    for (i = g_cursorIndex; i < len - 1; i++)
    {
        g_cmdLine[i] = g_cmdLine[i + 1];
    }

    if (i < len)
    {
        g_cmdLine[i] = '\0';
    }
}

// deletes character to left of cursor and moves the cursor to that position
static void Backspace(void)
{
    if (g_cursorIndex != 0)
    {
        CursorBackward(1);
        DeleteCharAtCursor();
    }
}

#if (WF_CONSOLE_HISTORY_COUNT > 0)
// erase entire line, put cursor at beginning of line
static void EraseLine(void)
{
    // move cursor to beginning of line (leave prompt in place)
    CursorBackward(g_cursorIndex);

    // erase line from cursor to end of line
    WF_PRINTF("%s", ERASE_FROM_CURSOR_TO_END_OF_LINE);
}
#endif // WF_CONSOLE_HISTORY_COUNT > 0

static void Delete(void)
{
    // update screen
    DeleteCharAtCursor();
}

static void ProcessAsciiChar(char rxByte)
{
    uint8_t len = strlen(g_cmdLine);

    if (len == MAX_CMD_LINE_LENGTH)
    {
        return;
    }

    // echo back to console
    WF_PRINTF("%c", rxByte);

    // if inserting character at cursor
    if (g_insertMode)
    {
        // move characters at cursor one character to right
        memmove(&g_cmdLine[g_cursorIndex + 1], &g_cmdLine[g_cursorIndex], strlen(&g_cmdLine[g_cursorIndex]));

        // insert new character into slot vacated by move to right
        g_cmdLine[g_cursorIndex++] = rxByte;

        // terminate string
        g_cmdLine[len + 1] = '\0';
    }
    // else replace character at cursor
    else
    {
        g_cmdLine[g_cursorIndex++] = rxByte;
    }

}

static bool isEscSequenceInProgress(void)
{
    return g_escSequenceInProgress;
}

// clear command
static void ProcessCtrlC(void)
{
    CmdLineClear();
    OutputPrompt();
}

static void Enter(void)
{
    // If at least one character in the command line
    if (!strlen(g_cmdLine) == 0)
    {
        WF_PRINTF("\r\n");

#if (WF_CONSOLE_HISTORY_COUNT > 0)

        // add cmd to history buffer if it is not identical to the current command in the history buffer.  This
        // avoids filling up history buffer with commands that are recalled and executed
        if (strcmp(g_cmdLine, g_cmdHistory.buf[g_cmdHistory.recallIndex]) != 0)
        {
            // store command in history buffer
            AddCmdToHistory(g_cmdLine);
        }
        // cmd line is same as current history index
        else
        {
            // bump history index so if up arrow hit again same command will come up
            ++g_cmdHistory.recallIndex;
            g_bumpFlag = true; // will be used if up arrow key is hit to keep logical sequence

        }
#endif

        // this function will alter g_cmdLine, so need history code above this
        ProcessConsoleCmd(g_cmdLine);

    }
    CmdLineClear();
    OutputPrompt();
}

static void CmdLineClear(void)
{
    g_cursorIndex          = 0;
    g_escSequenceInProgress = false;
    memset(g_cmdLine, 0x00, sizeof(g_cmdLine));
}

void OutputPrompt(void)
{
    WF_PRINTF("\r\n%s", PROMPT_STR);
}

#if (WF_CONSOLE_HISTORY_COUNT > 0)
static void InitHistory(void)
{
    g_cmdHistory.index       = 0;
    g_cmdHistory.seeded      = false;
    g_cmdHistory.recallIndex = 0;
    memset(g_cmdHistory.buf, 0x00, sizeof(g_cmdHistory.buf));
}

static void AddCmdToHistory(char *p_cmd)
{
    // copy new command to buffer
    strcpy(g_cmdHistory.buf[g_cmdHistory.index], p_cmd);

    // bump index to next line in history buffer
    g_cmdHistory.index = (g_cmdHistory.index + 1) % WF_CONSOLE_HISTORY_COUNT;

    // put recall index one command in advance of command just added
    g_cmdHistory.recallIndex = g_cmdHistory.index;

    g_cmdHistory.seeded = true;  // at least one entry in history buffer
}

static void DisplayHistoryEntry(uint8_t action)
{

    bool foundEntry = false;

    // if nothing in history buffer
    if (g_cmdHistory.seeded == false)
    {
      return;
    }

    if (action == PREV_CMD)
    {
        g_bumpFlag = false;
        
        --g_cmdHistory.recallIndex;
        if (g_cmdHistory.recallIndex < 0)
        {
            g_cmdHistory.recallIndex = WF_CONSOLE_HISTORY_COUNT - 1;
        }

        /* search until found a history entry or searched all entries */
        while (foundEntry == false)
        {
            /* if found a history entry */
            if (g_cmdHistory.buf[g_cmdHistory.recallIndex][0] != 0)
            {
                foundEntry = true;
            }
            else
            {
                --g_cmdHistory.recallIndex;
                if (g_cmdHistory.recallIndex < 0)
                {
                    g_cmdHistory.recallIndex = WF_CONSOLE_HISTORY_COUNT  - 1;
                }
            }
        }
    }
    else /* action == NEXT_CMD */
    {
        if (!g_bumpFlag)
        {
            g_cmdHistory.recallIndex = (g_cmdHistory.recallIndex + 1) % WF_CONSOLE_HISTORY_COUNT;
        }
        else
        {
            g_bumpFlag = false;
        }

        /* search until found a history entry or searched all entries */
        while (foundEntry == false)
        {
            /* if found a history entry */
            if (g_cmdHistory.buf[g_cmdHistory.recallIndex][0] != 0)
            {
                foundEntry = true;
            }
            else
            {
                g_cmdHistory.recallIndex = (g_cmdHistory.recallIndex + 1) % WF_CONSOLE_HISTORY_COUNT;
            }
        }
    }

    if (foundEntry)
    {
        // erase line on screen and output prompt
        EraseLine();          // leaves cursor index at 0 */

        // output command from history buffer
        WF_PRINTF("%s", g_cmdHistory.buf[g_cmdHistory.recallIndex]);

        // copy history command to console buffer (so they match)
        strcpy(g_cmdLine, g_cmdHistory.buf[g_cmdHistory.recallIndex]);

        // update cursor index to end of line
        g_cursorIndex = strlen(g_cmdHistory.buf[g_cmdHistory.recallIndex]);
    }
}


#endif // WF_CONSOLE_HISTORY_COUNT > 0

#endif // WF_USE_CONSOLE
