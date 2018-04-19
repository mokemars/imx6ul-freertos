/*****************************************************************************
 * (c) Copyright 2009 Actel Corporation.  All rights reserved.
 *
 * Author : Actel Application Team
 * Rev    : 1.0.0.0
 * Description: Device driver for the on-board OLED for SmartFusion KITS
 *  API of sample basic driver for OLED display found on Actel SmartFusion
 *  development boards.
 *  This code is intended as an example of using the SmartFusion I2C driver.
 *
 */
#ifndef OLED_H_
#define OLED_H_

#include "drivers/mss_i2c/mss_i2c.h"

#define OLED_HORIZ_SCROLL_ON    	 0x01
#define OLED_HORIZ_SCROLL_OFF   	 0x00

#define OLED_LEFT_HORIZ_SCROLL       0x01
#define OLED_RIGHT_HORIZ_SCROLL      0x00

#define OLED_VERT_SCROLL_ON    		 0x01
#define OLED_VERT_SCROLL_OFF   	 	 0x00

#define OLED_HORIZ_SCROLL_STEP		 0x00
#define OLED_CONTRAST_VAL      		 0xFF
#define OLED_START_PAGE         	 0x00
#define OLED_HORIZ_SCROLL_TINVL 	 0x00
#define OLED_END_PAGE           	 0x04

/*-------------------------------------------------------------------------*/
/*  The oled_no_of_line type represents number of lines to be written on OLED.
    FIRST LINE  : The OLED cursor is set to line number 1 and only 1 line is
                  printed on OLED
    SECOND_LINE : The OLED cursor is set to line number 2 and only 1 line is
                  printed on OLED
    BOTH_LINES  : The OLED cursor is set to line number 1 and line 1 and line 2
                  are printed on OLED
    ALL_LINES   : The OLED cursor is set to Page 0, and Page 0 to Page 4 are
				  printed on OLED (for clearing all the pages)
*/
typedef enum {
    FIRST_LINE = 1,
    SECOND_LINE,
    BOTH_LINES,
    ALL_LINES
} oled_no_of_line;

/*-------------------------------------------------------------------------*//**
  oled_data
  ------------------------------------------------------------------------------
  There is one instance of this structure for OLED data. Instances of this
  structure is used to provide the data to OLED. A pointer to an instance of
  the oled_data structure is passed as the parameter OLED driver functions.
 */
struct oled_data
{
    /* Represents line number, where String 1 has to be printed */
    uint8_t line1;
    /* Represents character offset within the line1, from where String 1 has to be
     * printed */
    uint8_t char_offset1;
    /* Represents line number, where String 2 has to be printed */
    uint8_t line2;
    /* Represents character offset within the line2, from where String 2 has to be
     * printed */
    uint8_t char_offset2;
    /* String 1 holds the data to be displayed on line 1 of OLED, It has to be
     * less that 49 characters*/
    char    *string1;
    /* String 1 holds the data to be displayed on line 2 of OLED, It has to be
     * less that 49 characters*/
    char    *string2;
    /* Holds the contrast value to be set for String 1 and String 2 */
    uint8_t contrast_val;
    /* Represents ON or OFF for continuous horizontal scrolling  */
    uint8_t on_off;
	/* Represents horizontal scrolling direction (LEFT/RIGHT) */
	uint8_t horiz_scroll_direction;
	/* Represents ON or OFF for vertical scrolling */
	uint8_t vertical_on_off;
	/* Represents  vertical scrolling offset for vertical scrolling */
	unsigned char vert_scroll_offset;
    /* Represents  number of columns scrolls per step for horizontal scroll*/
    unsigned char column_scrool_per_step;
    /* Represents  start page for horizontal scroll*/
    unsigned char start_page;
    /* Represents  time interval for horizontal scroll*/
    unsigned char time_intrval_btw_scroll_step;
    /* Represents  end page for horizontal scroll*/
    unsigned char end_page;

};
/***************************************************************************//**
  The following defines can be used as parameter to the OLED_set_cursor()
  function.
 */

/***************************************************************************//**
  The OLED_init function initializes the OLED display.
 */
void OLED_init( void );

/***************************************************************************//**
  This function(OLED_clear_display) clears the content of the display RAM
  based on the LINES input.
  ------------------------------------------------------------------------------
  @param oled_no_of_line:
      The oled_no_of_line parameter enum that holds Number of lines.
      If FIRST_LINE is passed as parameter to this function then, this functions
      clears only First line that is 0
      If SECOND_LINE is passed as parameter to this function then, this functions
      clears only Second line that is 1
      If BOTH_LINE is passed as parameter to this function then, this functions
      clears entire OLED display.
      If ALL_LINES is passed as parameter to this function then, this functions
      clears entire display RAM (From Page 0 to Page 4.
 */
void OLED_clear_display( oled_no_of_line LINES );

/*-------------------------------------------------------------------------*//**
  OLED Write data.
  ------------------------------------------------------------------------------
  This function (OLED_write_data ) writes the data to OLED based on the
  parameters passed to this function.
  ------------------------------------------------------------------------------
  @param data:
      The data parameter is a pointer to an oled_data structure, that holds
      different fields of data to be required for OLED (see the oled_data structure
      definition).

  @param oled_no_of_line:
      The oled_no_of_line parameter enum that holds Number of lines.
      If FIRST_LINE is passed as parameter to this function then, this functions
      wtites string 1 at FIRST LINE
      If SECOND_LINE is passed as parameter to this function then, this functions
      wtites string 1 at SECOND LINE
      If BOTH_LINE is passed as parameter to this function then, this functions
      wtites string 1 and string 2 at FIRST LINE and SECOND LINE respectively.
Example:
  @code
#include "drivers/mss_watchdog/mss_watchdog.h"
#include "oled.h"
#define FIRST_CHARACTER 0

int main()
{

    char *string1="SmartFusion";
    char *string2="INNOVATIVE ";
    struct oled_data write_data;

    write_data.line1 = FIRST_LINE;
    write_data.char_offset1 = FIRST_CHARACTER;
    write_data.string1 = string1;
    write_data.line2 = SECOND_LINE;
    write_data.char_offset2 = FIRST_CHARACTER;
    write_data.string2 = string2;
    write_data.contrast_val = 0x01;

    MSS_WD_disable();
    OLED_init();
    OLED_write_data(&write_data,BOTH_LINES);
    return 0;
}
  @endcode
 */

void OLED_write_data(struct oled_data * data, oled_no_of_line flag);

/*-------------------------------------------------------------------------*//**
  OLED Horizontal scrolling.
  ------------------------------------------------------------------------------
  This function (OLED_horizontal_scroll ) enables the Continuous Horizontal scrolling.
  ------------------------------------------------------------------------------
  @param data:
      The horiz_scroll parameter is a pointer to an oled_data structure, that holds
      different fields of data to be required for OLED (see the oled_data structure
      definition).
Example:
  @code

int main()
{

    char *string1="SmartFusion";
    char *string2="INNOVATIVE     ";
    struct oled_data write_data;

    write_data.line1 = FIRST_LINE;
    write_data.char_offset1 = FIRST_CHARACTER;
    write_data.string1 = string1;
    write_data.line2 = SECOND_LINE;
    write_data.char_offset2 = FIRST_CHARACTER;
    write_data.string2 = string2;

    write_data.contrast_val 				= OLED_CONTRAST_VAL;
	write_data.contrast_val                 = OLED_CONTRAST_VAL;
	write_data.on_off                       = OLED_HORIZ_SCROLL_ON;
	write_data.horiz_scroll_direction		= OLED_RIGHT_HORIZ_SCROLL;
	write_data.vertical_on_off				= OLED_VERT_SCROLL_OFF;
	write_data.column_scrool_per_step       = OLED_HORIZ_SCROLL_STEP;
	write_data.start_page                   = OLED_START_PAGE;
	write_data.time_intrval_btw_scroll_step = OLED_HORIZ_SCROLL_TINVL;
	write_data.end_page                     = OLED_END_PAGE;

    MSS_WD_disable();
    OLED_init();
    OLED_write_data(&write_data,BOTH_LINES);
    OLED_horizontal_scroll(&write_data);
    return 0;
}
  @endcode
 */
void OLED_horizontal_scroll(struct oled_data * horiz_scroll);

/***************************************************************************//**
  This function(OLED_contrast) sets the contrast to the data displayed on the
  OLED.
  ------------------------------------------------------------------------------
  @param color_contrast:
  The color_contrast parameter that holds contrast value.
  The color_contrast values should be in the range of 1 to 256.
 */
void OLED_contrast(uint8_t color_contrast);

/*-------------------------------------------------------------------------*//**
  OLED Continuous Vertical and Horizontal scrolling.
  ------------------------------------------------------------------------------
  This function (OLED_cont_vert_and_horiz_scroll ) enables the vertical and
  continuous Horizontal scrolling.
  ------------------------------------------------------------------------------
  @param data:
      The horiz_scroll parameter is a pointer to an oled_data structure, that holds
      different fields of data to be required for OLED (see the oled_data structure
      definition).
Example:
  @code

int main()
{

    char *string1="SmartFusion";
    char *string2="INNOVATIVE     ";
    struct oled_data write_data;

    write_data.line1 = FIRST_LINE;
    write_data.char_offset1 = FIRST_CHARACTER;
    write_data.string1 = string1;
    write_data.line2 = SECOND_LINE;
    write_data.char_offset2 = FIRST_CHARACTER;
    write_data.string2 = string2;

	write_data.contrast_val                 = OLED_CONTRAST_VAL;
	write_data.vertical_on_off				= OLED_VERT_SCROLL_ON;
	write_data.horiz_scroll_direction		= OLED_RIGHT_HORIZ_SCROLL;
	write_data.column_scrool_per_step       = OLED_HORIZ_SCROLL_STEP;
	write_data.start_page                   = OLED_START_PAGE;
	write_data.time_intrval_btw_scroll_step = OLED_HORIZ_SCROLL_TINVL;
	write_data.end_page                     = OLED_END_PAGE;
	write_data.vert_scroll_offset			= 0x01;

    MSS_WD_disable();
    OLED_init();
    OLED_write_data(&write_data, BOTH_LINES);
    OLED_cont_vert_and_horiz_scroll(&write_data);
    return 0;
}
  @endcode
 */
void OLED_cont_vert_and_horiz_scroll(struct oled_data * horiz_scroll);

void OLED_set_cursor(uint8_t line, uint8_t char_offset);
void OLED_write_string(const char *string);
void OLED_write_char(const uint8_t data_char);

#endif
