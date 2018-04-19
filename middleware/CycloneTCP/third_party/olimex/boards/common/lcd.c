//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   #include "sam7_ex256.h"
//LPC1766-STK evaluation board?
#elif defined(USE_LPC1766_STK)
   #include "lpc1766_stk.h"
//LPC2378-STK evaluation board?
#elif defined(USE_LPC2378_STK)
   #include "lpc2378_stk.h"
//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   #include "stm32_p407.h"
//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   #include "91x_lib.h"
#endif

//Dependencies
#include "lcd.h"

//#include "bmp.h"
//#include "bits.h"

#if 0

AT91PS_PIO    l_pPioA   = AT91C_BASE_PIOA;
AT91PS_PIO    l_pPioB   = AT91C_BASE_PIOB;
AT91PS_SPI    l_pSpi    = AT91C_BASE_SPI0;
AT91PS_PMC    l_pPMC    = AT91C_BASE_PMC;
AT91PS_PDC    l_pPDC    = AT91C_BASE_PDC_SPI0;


#define SPI_SR_TXEMPTY

#define LCD_RESET_LOW     l_pPioA->PIO_CODR   = BIT2
#define LCD_RESET_HIGH    l_pPioA->PIO_SODR   = BIT2

void Delaya (unsigned long a) { while (--a!=0); }

void Delay_ (unsigned long a) {

  volatile unsigned long d;
  d=a;
  while (--d!=0);
}

unsigned int i,j;

void displayInit(void) {

  // Pin for backlight
  //l_pPioB->PIO_CODR   = BIT20;    // Set PB20 to LOW
  l_pPioB->PIO_SODR   = BIT20;    // Set PB20 to HIGH
  l_pPioB->PIO_OER    = BIT20;    // Configure PB20 as output

  // Reset pin
  l_pPioA->PIO_SODR   = BIT2;     // Set PA2 to HIGH
  l_pPioA->PIO_OER    = BIT2;     // Configure PA2 as output

  // CS pin
  //l_pPioA->PIO_SODR   = BIT12;     // Set PA2 to HIGH
  //l_pPioA->PIO_OER    = BIT12;     // Configure PA2 as output


  // Init SPI0
  //set functionality to pins:
  //port0.12 -> NPCS0
  //port0.16 -> MISO
  //port0.17 -> MOSI
  //port0.18 -> SPCK
  l_pPioA->PIO_PDR = BIT12 | BIT16 | BIT17 | BIT18 | BIT13;
  l_pPioA->PIO_ASR = BIT12 | BIT16 | BIT17 | BIT18 | BIT13;
  l_pPioA->PIO_BSR = 0;


  //enable the clock of SPI
  l_pPMC->PMC_PCER = 1 << AT91C_ID_SPI0;
  //l_pPMC->PMC_PCER = 1 << 5;

  // Fixed mode
  l_pSpi->SPI_CR      = 0x80;               //SPI Enable, Sowtware reset
  l_pSpi->SPI_CR      = 0x01;               //SPI Enable


  //l_pSpi->SPI_MR      = 0xE0099;           //Master mode, fixed select, disable decoder, FDIV=1 (NxMCK), PCS=1110, loopback
  //l_pSpi->SPI_MR      = 0xE0019;            //Master mode, fixed select, disable decoder, FDIV=1 (NxMCK), PCS=1110,
  //l_pSpi->SPI_MR      = 0xE0011;            //Master mode, fixed select, disable decoder, FDIV=0 (MCK), PCS=1110
  l_pSpi->SPI_MR      = 0x100E0011;            //Master mode, fixed select, disable decoder, FDIV=1 (MCK), PCS=1110

  //l_pSpi->SPI_CSR[0]  = 0x4A02;             //8bit, CPOL=0, ClockPhase=1, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x4A13;             //9bit, CPOL=1, ClockPhase=1, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x4A12;             //9bit, CPOL=0, ClockPhase=1, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x4A11;             //9bit, CPOL=1, ClockPhase=0, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x01011F11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*31 = 48kHz
  // work l_pSpi->SPI_CSR[0]  = 0x01010F11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*15 = 96kHz
  l_pSpi->SPI_CSR[0]  = 0x01010C11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*12 = 125kHz
  l_pSpi->SPI_CSR[1]  = 0x01010502;

}


void WriteSpiCommand(unsigned int data){

  data = (data & ~0x0100);

  // Wait for the transfer to complete
  while((l_pSpi->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

  l_pSpi->SPI_TDR = data;
}

void WriteSpiData(unsigned int data){

  data = (data | 0x0100);

  // Wait for the transfer to complete
  while((l_pSpi->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

  l_pSpi->SPI_TDR = data;
}


void displaySetBacklight(bool state) {

  if(state)
    l_pPioB->PIO_SODR   = BIT20;    // Set PB20 to HIGH
  else
    l_pPioB->PIO_CODR   = BIT20;    // Set PB20 to LOW

}

void displaySetContrast(unsigned char contrast) {

#ifdef GE12
  WriteSpiCommand(CONTRAST);
  WriteSpiData(0x20+contrast);    // contrast
#else
  WriteSpiCommand(VOLCTR);
  WriteSpiData(32+contrast); // contrast
  WriteSpiData(3); // contrast

#endif

}

void LCDSettings(void) {

#ifdef GE12

  // Hardware reset
  //LCD_RESET_LOW;
  //Delaya(2000);
  //LCD_RESET_HIGH;
  //Delaya(2000);

  ///// Initialization start /////

  // 2. Software Reset
  WriteSpiCommand(SOFTRST);
  Delaya(2000);

  // 3. Initial escape
  WriteSpiCommand(INITESC);
  Delaya(2000);

  ///// Initialization end /////

  ///// Display setting 1 /////

  // 1. Refresh set
  WriteSpiCommand(REFSET);
  WriteSpiData(0);

  // 2. Display control - 7 parameters
   /*
   WriteSpiCommand(DISPCTRL);
   WriteSpiData(128);     // Set the lenght of one selection term
   WriteSpiData(128);     // Set N inversion -> no N inversion
   WriteSpiData(129);     // Set frame frequence and bias rate -> 2 devision of frequency and 1/8 bias, 1/67 duty, 96x67 size
   //WriteByte(134, DATA);     // Set frame frequence and bias rate -> 2 devision of frequency and 1/8 bias, 1/67 duty, 96x67 size
   //WriteByte(0x8D, DATA);     // Set frame frequence and bias rate -> 2 devision of frequency and 1/8 bias, 1/67 duty, 96x67 size
   //WriteByte(132, DATA);     // Set frame frequence and bias rate -> 2 devision of frequency and 1/8 bias, 1/67 duty, 96x67 size
   WriteSpiData(84);      // Set duty parameter
   WriteSpiData(69);      // Set duty parameter
   WriteSpiData(82);      // Set duty parameter
   WriteSpiData(67);      // Set duty parameter
   */


  WriteSpiCommand(DISPCTRL);
  WriteSpiData(128);     // Set the lenght of one selection term
  WriteSpiData(128);     // Set N inversion -> no N inversion
  WriteSpiData(134);     // Set frame frequence and bias rate -> 2 devision of frequency and 1/8 bias, 1/67 duty, 96x67 size
  WriteSpiData(84);      // Set duty parameter
  WriteSpiData(69);      // Set duty parameter
  WriteSpiData(82);      // Set duty parameter
  WriteSpiData(67);      // Set duty parameter


  // 3.1 Grey scale 0 position set - 15 parameters
  WriteSpiCommand(GRAYSCALE0);
  WriteSpiData(1);     // GCP1 - gray lavel to be output when the RAM data is "0001"
  WriteSpiData(2);     // GCP2 - gray lavel to be output when the RAM data is "0010"
  WriteSpiData(4);     // GCP3 - gray lavel to be output when the RAM data is "0011"
  WriteSpiData(8);     // GCP4 - gray lavel to be output when the RAM data is "0100"
  WriteSpiData(16);    // GCP5 - gray lavel to be output when the RAM data is "0101"
  WriteSpiData(30);    // GCP6 - gray lavel to be output when the RAM data is "0110"
  WriteSpiData(40);    // GCP7 - gray lavel to be output when the RAM data is "0111"
  WriteSpiData(50);    // GCP8 - gray lavel to be output when the RAM data is "1000"
  WriteSpiData(60);    // GCP9 - gray lavel to be output when the RAM data is "1001"
  WriteSpiData(70);    // GCP10 - gray lavel to be output when the RAM data is "1010"
  WriteSpiData(80);    // GCP11 - gray lavel to be output when the RAM data is "1011"
  WriteSpiData(90);    // GCP12 - gray lavel to be output when the RAM data is "1100"
  WriteSpiData(100);   // GCP13 - gray lavel to be output when the RAM data is "1101"
  WriteSpiData(110);   // GCP14 - gray lavel to be output when the RAM data is "1110"
  WriteSpiData(127);   // GCP15 - gray lavel to be output when the RAM data is "1111"

  // 4. Gamma curve set - select gray scale - GRAYSCALE 0 or GREYSCALE 1
  WriteSpiCommand(GAMMA);
  WriteSpiData(1);     // Select grey scale 0


  // 5. Command driver output
  WriteSpiCommand(COMMONDRV);
  WriteSpiData(0);     // Set COM1-COM41 side come first, normal mod


  // 6. Set Normal mode (my)
  WriteSpiCommand(NORMALMODE);

  // 7. Inversion off
  // WriteSpiCommand(INVERSIONOFF);

  // 8. Column address set
  WriteSpiCommand(COLADDRSET);
  WriteSpiData(0);
  WriteSpiData(131);

  // 9. Page address set
  WriteSpiCommand(PAGEADDRSET);
  WriteSpiData(0);
  WriteSpiData(131);

  // 10. Memory access controler
  WriteSpiCommand(ACCESSCTRL);
  WriteSpiData(0x40);     // horizontal
  //WriteSpiData(0x20);   // vertical

  ///// Display setting 1 end  /////


  ///// Power supply  ///////

  // 1. Power control
  WriteSpiCommand(PWRCTRL);
  WriteSpiData(4);     // Internal resistance, V1OUT -> high power mode, oscilator devision rate

  // 2. Sleep out
  WriteSpiCommand(SLEEPOUT);


  // 3. Voltage control - voltage control and write contrast define LCD electronic volume
  WriteSpiCommand(VOLTCTRL);
  //WriteSpiData(0x7f);    //  full voltage control
  //WriteSpiData(0x03);    //  must be "1"

  // 4. Write contrast
  WriteSpiCommand(CONTRAST);
  WriteSpiData(0x3b);    // contrast

  Delaya(2000);

  // 5. Temperature gradient
  WriteSpiCommand(TEMPGRADIENT);
  for(i=0; i<14; i++) {
    WriteSpiData(0);
  }

  // 6. Booster voltage ON
  WriteSpiCommand(BOOSTVON);


  // Finally - Display On
  WriteSpiCommand(DISPLAYON);

#else


  // Hardware reset
  LCD_RESET_LOW;
  Delay_(1000);
  LCD_RESET_HIGH;
  Delay_(1000);

  // Display vontrol
  WriteSpiCommand(DISCTL);
//  WriteSpiData(0x03); // no division
//  WriteSpiData(0x23); // 160 line
//  WriteSpiData(0x02); // 2 highlighte line
  WriteSpiData(0x00); // default
  WriteSpiData(0x20); // (32 + 1) * 4 = 132 lines (of which 130 are visible)
  WriteSpiData(0x0a); // default

  // COM scan
  WriteSpiCommand(COMSCN);
  WriteSpiData(0x00);  // Scan 1-80

  // Internal oscilator ON
  WriteSpiCommand(OSCON);

  // wait aproximetly 100ms
  Delay_(10000);

  // Sleep out
  WriteSpiCommand(SLPOUT);

  // Voltage control
  WriteSpiCommand(VOLCTR);
  WriteSpiData(43); // middle value of V1
  WriteSpiData(0x03); // middle value of resistance value

  // Temperature gradient
  WriteSpiCommand(TMPGRD);
  WriteSpiData(0x00); // default

  // Power control
  WriteSpiCommand(PWRCTR);
  WriteSpiData(0x0f);   // referance voltage regulator on, circuit voltage follower on, BOOST ON

  // Normal display
  WriteSpiCommand(DISNOR);

  // Inverse display
  WriteSpiCommand(DISINV);

  // Partial area off
  WriteSpiCommand(PTLOUT);

//  // Scroll area set
//  WriteSpiCommand(ASCSET);
//  WriteSpiData(0);
//  WriteSpiData(0);
//  WriteSpiData(40);
//  WriteSpiData(3);

//  // Vertical scrool address start
//  WriteSpiCommand(SCSTART);
//  WriteSpiData(0);


  // Data control
  WriteSpiCommand(DATCTL);
  WriteSpiData(0x00); // all inversions off, column direction
  WriteSpiData(0x03); // RGB sequence
  WriteSpiData(0x02); // Grayscale -> 16

  // Page Address set
  WriteSpiCommand(PASET);
  WriteSpiData(0);
  WriteSpiData(131);

  // Page Column set
  WriteSpiCommand(CASET);
  WriteSpiData(0);
  WriteSpiData(131);

#endif



}


void LCDWrite130x130bmp(unsigned char toto) {

#ifdef GE12

  // Display OFF
  WriteSpiCommand(DISPLAYOFF);

    // WRITE MEMORY
  WriteSpiCommand(MEMWRITE);


  for(j=0; j<sizeof(bmp); j++) {
    WriteSpiData(bmp[j]);
  }


  // Display On
  WriteSpiCommand(DISPLAYON);

#else

  // Display OFF
  // WriteSpiCommand(DISOFF);

  // WRITE MEMORY
  WriteSpiCommand(RAMWR);

  //for(j=0; j<sizeof(bmp)-396; j++) {
  for(j=0; j<9000; j++) //{
//    WriteSpiData(bmp[j]);
    WriteSpiData(0x00);

     for(; j<18000; j++) //{
//    WriteSpiData(bmp[j]);
    WriteSpiData(toto);

     for(; j<25740; j++) //{
//    WriteSpiData(bmp[j]);
    WriteSpiData(0xFF);
  //}

  // wait aproximetly 100ms
  Delay_(10000);

  // Display On
  WriteSpiCommand(DISON);

#endif

}

#endif














void Delay (unsigned long a) {
while (--a!=0);
}




// ******************************************************
// Pointers to AT91SAM7X256 peripheral data structures
// ******************************************************
#if 0
volatile AT91PS_PIO pPIOA = AT91C_BASE_PIOA;
volatile AT91PS_PIO pPIOB = AT91C_BASE_PIOB;
volatile AT91PS_SPI pSPI = AT91C_BASE_SPI0;
volatile AT91PS_PMC pPMC = AT91C_BASE_PMC;
volatile AT91PS_PDC pPDC = AT91C_BASE_PDC_SPI0;
#endif

// *********************************************************************************************
// InitSpi( )
//
// Sets up SPI channel 0 for communications to Nokia 6610 LCD Display
//
// I/O ports used: PA2 = LCD Reset (set to low to reset)
// PA12 = LCD chip select (set to low to select the LCD chip)
// PA16 = SPI0_MISO Master In - Slave Out (not used in LCD interface)
// PA17 = SPI0_MOSI Master Out - Slave In pin (Serial Data to LCD slave)
// PA18 = SPI0_SPCK Serial Clock (to LCD slave)
// PB20 = backlight control (normally PWM control, 1 = full on)
//
// Author: Olimex, James P Lynch August 30, 2007
// *********************************************************************************************
void InitSpi(void)
{
#if 0
// Pin PB20 used for LCD_BL (backlight)
pPIOB->PIO_OER = BIT20; // Configure PB20 as output
pPIOB->PIO_SODR = BIT20; // Set PB20 to HIGH (backlight under PWM control - this will turn it full on)
// Pin PA2 used for LCD_RESET
pPIOA->PIO_OER = BIT2; // Configure PA2 as output
pPIOA->PIO_SODR = BIT2; // Set PA2 to HIGH (assert LCD Reset low then high to reset the controller)
// Pin PA2 used for CS_LCD (chip select)
pPIOA->PIO_OER = BIT12; // Configure PA12 as output
pPIOA->PIO_SODR = BIT12; // Set PA12 to HIGH (assert CS_LCD low to enable transmission)
// Disable the following pins from PIO control (will be used instead by the SPI0 peripheral)
// BIT12 = PA12 -> SPI0_NPCS0 chip select
// BIT16 = PA16 -> SPI0_MISO Master In - Slave Out (not used in LCD interface)
// BIT17 = PA17 -> SPI0_MOSI Master Out - Slave In pin (Serial Data to LCD slave)
// BIT18 = PA18 -> SPI0_SPCK Serial Clock (to LCD slave)
pPIOA->PIO_PDR = BIT12 | BIT16 | BIT17 | BIT18; // Peripheral A Disable Register (Disable PIO control of these 4 bits)
pPIOA->PIO_ASR = BIT12 | BIT16 | BIT17 | BIT18; // Peripheral A Select Register (all 4 bits are in PIOA)
pPIOA->PIO_BSR = 0; // Peripheral B Select Register (none of the bits are in PIOB)
//enable the SPI0 Peripheral clock
pPMC->PMC_PCER = 1 << AT91C_ID_SPI0;

// SPI Control Register SPI_CR
pSPI->SPI_CR = AT91C_SPI_SWRST | AT91C_SPI_SPIEN; //Software reset, SPI Enable (0x81)
pSPI->SPI_CR = AT91C_SPI_SPIEN; //SPI Enable (0x01)
// SPI Mode Register SPI_MR = 0xE0011
pSPI->SPI_MR =
(AT91C_SPI_DLYBCS & (0 << 24)) | // Delay between chip selects (take default: 6 MCK periods)
(AT91C_SPI_PCS & (0xE << 16)) | // Peripheral Chip Select (selects SPI_NPCS0 or PA12)
(AT91C_SPI_LLB & (0 << 7)) | // Local Loopback Enabled (disabled)
(AT91C_SPI_MODFDIS & (1 << 4)) | // Mode Fault Detection (disabled)
(AT91C_SPI_PCSDEC & (0 << 2)) | // Chip Select Decode (chip selects connected directly to peripheral)
(AT91C_SPI_PS & (0 << 1)) | // Peripheral Select (fixed)
(AT91C_SPI_MSTR & (1 << 0)); // Master/Slave Mode (Master)
// SPI Chip Select Register SPI_CSR[0] = 0x01010311
pSPI->SPI_CSR[0] =
(AT91C_SPI_DLYBCT & (0x01 << 24)) | // Delay between Consecutive Transfers (32 MCK periods)
(AT91C_SPI_DLYBS & (0x01 << 16)) | // Delay Before SPCK (1 MCK period)
(AT91C_SPI_SCBR & (0x10 << 8)) | // Serial Clock Baud Rate (baudrate = MCK/8 = 48054841/8 = 6006855 baud
(AT91C_SPI_BITS & (AT91C_SPI_BITS_9)) | // Bits per Transfer (9 bits)
(AT91C_SPI_CSAAT & (0x0 << 3)) | // Chip Select Active After Transfer (is active after xfer)/
(AT91C_SPI_NCPHA & (0x0 << 1)) | // Clock Phase (data captured on falling edge)
(AT91C_SPI_CPOL & (0x01 << 0)); // Clock Polarity (inactive state is logic one)

#endif

#if 0
   uint32_t pclk;

   //Configure P1.20 (SCK0), P1.23 (MISO0) and P1.24 (MOSI0)
   LPC_PINCON->PINSEL3 &= ~(PINSEL3_P1_20_MASK | PINSEL3_P1_23_MASK | PINSEL3_P1_24_MASK);
   LPC_PINCON->PINSEL3 |= PINSEL3_P1_20_SCK0 | PINSEL3_P1_23_MISO0 | PINSEL3_P1_24_MOSI0;

   //Check the SSP0 peripheral clock
   switch(LPC_SC->PCLKSEL1 & PCLKSEL1_PCLK_SSP0)
   {
   case PCLKSEL1_PCLK_SSP0_DIV1:
      pclk = SystemCoreClock;
      break;
   case PCLKSEL1_PCLK_SSP0_DIV2:
      pclk = SystemCoreClock / 2;
      break;
   case PCLKSEL1_PCLK_SSP0_DIV4:
      pclk = SystemCoreClock / 4;
      break;
   default:
      pclk = SystemCoreClock / 8;
      break;
   }

   //Power up SSP0
   LPC_SC->PCONP |= PCONP_PCSSP0;

   //SSP0 module initialization
   LPC_SSP0->CR0 = CR0_DSS_9;
   LPC_SSP0->CR1 = 0;

   //Set clock prescaler
   LPC_SSP0->CPSR = pclk / 1000000;

   //Disable all interrupts
   LPC_SSP0->IMSC = 0;
   //Disable DMA
   LPC_SSP0->DMACR = 0;

   //Enable SPI module
   LPC_SSP0->CR1 |= CR1_SSE;
#endif
}
// *****************************************************************************
// WriteSpiCommand.c
//
// Writes 9-bit command to LCD display via SPI interface
//
// Inputs: data - Epson S1D15G00 controller/driver command
//
//
// Note: clears bit 8 to indicate command transfer
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void WriteSpiCommand(volatile unsigned int command)
{
   //Assert CS
   spiAssertCs();
   //Send command
   spiTransfer(command & 0xFF);
   //Deassert CS
   spiAssertCs();

#if 0
// wait for the previous transfer to complete
while((pSPI->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
// clear bit 8 - indicates a "command"
command = (command & ~0x0100);
// send the command
pSPI->SPI_TDR = command;

   LCD_CS_FIOCLR = LCD_CS_MASK;
   while(!LPC_SSP0->SR & SR_TNF);
   LPC_SSP0->DR = command;
   while(LPC_SSP0->SR & SR_BSY);
   LCD_CS_FIOSET = LCD_CS_MASK;
#endif
}
// *****************************************************************************
// WriteSpiData.c
//
// Writes 9-bit command to LCD display via SPI interface
//
// Inputs: data - Epson S1D15G00 controller/driver command
//
//
// Note: Sets bit 8 to indicate data transfer
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void WriteSpiData(volatile unsigned int data)
{
   //Assert CS
   spiAssertCs();
   //Send data
   spiTransfer(0x0100 | data);
   //Deassert CS
   spiAssertCs();

#if 0
// wait for the transfer to complete
while((pSPI->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
// set bit 8, indicates "data"
data = (data | 0x0100);
// send the data
pSPI->SPI_TDR = data;

  LCD_CS_FIOCLR = LCD_CS_MASK;
  while(!LPC_SSP0->SR & SR_TNF);
  LPC_SSP0->DR = 0x0100 | data;
  while(LPC_SSP0->SR & SR_BSY);
  LCD_CS_FIOSET = LCD_CS_MASK;
#endif
}


// *****************************************************************************
// Backlight.c
//
// Turns the backlight on and off
//
// Inputs: state - 1 = backlight on
// 2 = backlight off
//
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void Backlight(unsigned char state)
{
#if 0
if(state == 1)
pPIOB->PIO_SODR = BIT20; // Set PB20 to HIGH
else
pPIOB->PIO_CODR = BIT20; // Set PB20 to LOW
#endif
}
// *****************************************************************************
// InitLcd.c
//
// Initializes the Epson S1D15G00 LCD Controller
//
// Inputs: none
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
void InitLcd(void)
{
  /* //Configure LCD RST as an output
   LCD_RST_FIOCLR = LCD_RST_MASK;
   LCD_RST_FIODIR |= LCD_RST_MASK;

   //Configure LCD CS as an output
   LCD_CS_FIOSET = LCD_CS_MASK;
   LCD_CS_FIODIR |= LCD_CS_MASK;

   //Configure the backlight control pin
   LCD_BL_FIOCLR = LCD_BL_MASK;
   LCD_BL_FIODIR |= LCD_BL_MASK;

   //Take the LCD controller out of reset
   Delay(10000);
   LCD_RST_FIOSET = LCD_RST_MASK;
   Delay(10000);*/

// Hardware reset

   /////LCD_RESET_LOW;
  // LCD_RST_FIOCLR = LCD_RST_MASK;
//Delay(10000);

   ////LCD_RESET_HIGH;
  // LCD_RST_FIOSET = LCD_RST_MASK;
///Delay(10000);
// Display control
WriteSpiCommand(DISCTL);
WriteSpiData(0x00); // P1: 0x00 = 2 divisions, switching period=8 (default)
WriteSpiData(0x20); // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
WriteSpiData(0x00); // P3: 0x00 = no inversely highlighted lines
// COM scan
WriteSpiCommand(COMSCN);
WriteSpiData(1); // P1: 0x01 = Scan 1->80, 160<-81
// Internal oscilator ON
WriteSpiCommand(OSCON);
// Sleep out
WriteSpiCommand(SLPOUT);
// Power control
WriteSpiCommand(PWRCTR);
WriteSpiData(0x0f); // reference voltage regulator on, circuit voltage follower on, BOOST ON
// Inverse display
WriteSpiCommand(DISINV);
// Data control
WriteSpiCommand(DATCTL);
WriteSpiData(0x01); // P1: 0x01 = page address inverted, column address normal, address scan in column direction
WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A)
// Voltage control (contrast setting)
WriteSpiCommand(VOLCTR);
WriteSpiData(32); // P1 = 32 volume value (experiment with this value to get the best contrast)
WriteSpiData(3); // P2 = 3 resistance ratio (only value that works)
// allow power supply to stabilize
Delay(100000);
// turn on the display
WriteSpiCommand(DISON);
}


// *****************************************************************************
// LCDWrite130x130bmp.c
//
// Writes the entire screen from a bmp file
// Uses Olimex BmpToArray.exe utility
//
// Inputs: picture in bmp.h
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void LCDWrite130x130bmp(void) {
long j; // loop counter
// Data control (need to set "normal" page address for Olimex photograph)
WriteSpiCommand(DATCTL);
WriteSpiData(0x00); // P1: 0x00 = page address normal, column address normal, address scan in column direction
WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16
// Display OFF
WriteSpiCommand(DISOFF);
// Column address set (command 0x2A)
WriteSpiCommand(CASET);
WriteSpiData(0);
WriteSpiData(131);
// Page address set (command 0x2B)
WriteSpiCommand(PASET);
WriteSpiData(0);
WriteSpiData(131);
// WRITE MEMORY
WriteSpiCommand(RAMWR);
for(j = 0; j < 25740; j++) {
WriteSpiData(0xFF);
}
// Data control (return to "inverted" page address)
WriteSpiCommand(DATCTL);
WriteSpiData(0x01); // P1: 0x01 = page address inverted, column address normal, address scan in column direction
WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16
// Display On
WriteSpiCommand(DISON);
}

// *****************************************************************************
// LCDClearScreen.c
//
// Clears the LCD screen to single color (BLACK)
//
// Inputs: none
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
void LCDClearScreen(void) {
long i; // loop counter
// Row address set (command 0x2B)
WriteSpiCommand(PASET);
WriteSpiData(0);
WriteSpiData(131);
// Column address set (command 0x2A)
WriteSpiCommand(CASET);
WriteSpiData(0);
WriteSpiData(131);
// set the display memory to BLACK
WriteSpiCommand(RAMWR);
for(i = 0; i < ((131 * 131) / 2); i++) {
WriteSpiData((BLACK >> 4) & 0xFF);
WriteSpiData(((BLACK & 0xF) << 4) | ((BLACK >> 8) & 0xF));
WriteSpiData(BLACK & 0xFF);
}
}


// *************************************************************************************
// LCDSetPixel.c
//
// Lights a single pixel in the specified color at the specified x and y addresses
//
// Inputs: x = row address (0 .. 131)
// y = column address (0 .. 131)
// color = 12-bit color value rrrrggggbbbb
// rrrr = 1111 full red
// :
// 0000 red is off
//
// gggg = 1111 full green
// :
// 0000 green is off
//
// bbbb = 1111 full blue
// :
// 0000 blue is off
//
// Returns: nothing
//
// Note: see lcd.h for some sample color settings
//
// Author: James P Lynch August 30, 2007
// *************************************************************************************
void LCDSetPixel(int x, int y, int color) {
// Row address set (command 0x2B)
WriteSpiCommand(PASET);
WriteSpiData(x);
WriteSpiData(x);
// Column address set (command 0x2A)
WriteSpiCommand(CASET);
WriteSpiData(y);
WriteSpiData(y);
// Now illuminate the pixel (2nd pixel will be ignored)
WriteSpiCommand(RAMWR);
WriteSpiData((color >> 4) & 0xFF);
WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
WriteSpiData(color & 0xFF);
}
// *************************************************************************************************
// LCDSetLine.c
//
// Draws a line in the specified color from (x0,y0) to (x1,y1)
//
// Inputs: x = row address (0 .. 131)
// y = column address (0 .. 131)
// color = 12-bit color value rrrrggggbbbb
// rrrr = 1111 full red
// :
// 0000 red is off
//
// gggg = 1111 full green
// :
// 0000 green is off
//
// bbbb = 1111 full blue
// :
// 0000 blue is off
//
// Returns: nothing
//
// Note: good write-up on this algorithm in Wikipedia (search for Bresenham's line algorithm)
// see lcd.h for some sample color settings
//
// Authors: Dr. Leonard McMillan, Associate Professor UNC
// Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//
// Note: taken verbatim from Professor McMillan's presentation:
// http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html
//
// *************************************************************************************************
void LCDSetLine(int x0, int y0, int x1, int y1, int color) {
int dy = y1 - y0;
int dx = x1 - x0;
int stepx, stepy;

if (dy < 0) { dy = -dy; stepy = -1; } else { stepy = 1; }
if (dx < 0) { dx = -dx; stepx = -1; } else { stepx = 1; }
dy <<= 1; // dy is now 2*dy
dx <<= 1; // dx is now 2*dx
LCDSetPixel(x0, y0, color);
if (dx > dy) {
int fraction = dy - (dx >> 1); // same as 2*dy - dx
while (x0 != x1) {
if (fraction >= 0) {
y0 += stepy;
fraction -= dx; // same as fraction -= 2*dx
}
x0 += stepx;
fraction += dy; // same as fraction -= 2*dy
LCDSetPixel(x0, y0, color);
}
} else {
int fraction = dx - (dy >> 1);
while (y0 != y1) {
if (fraction >= 0) {
x0 += stepx;
fraction -= dy;
}
y0 += stepy;
fraction += dx;
LCDSetPixel(x0, y0, color);
}
}
}
// *****************************************************************************************
// LCDSetRect.c
//
// Draws a rectangle in the specified color from (x1,y1) to (x2,y2)
// Rectangle can be filled with a color if desired
//
// Inputs: x = row address (0 .. 131)
// y = column address (0 .. 131)
// fill = 0=no fill, 1-fill entire rectangle
// color = 12-bit color value for lines rrrrggggbbbb
// rrrr = 1111 full red
// :
// 0000 red is off
//
// gggg = 1111 full green
// :
// 0000 green is off
//
// bbbb = 1111 full blue
// :
// 0000 blue is off
//
// Returns: nothing
//
// Notes:
//
// The best way to fill a rectangle is to take advantage of the "wrap-around" featute
// built into the Epson S1D15G00 controller. By defining a drawing box, the memory can
// be simply filled by successive memory writes until all pixels have been illuminated.
//
// 1. Given the coordinates of two opposing corners (x0, y0) (x1, y1)
// calculate the minimums and maximums of the coordinates
//
// xmin = (x0 <= x1) ? x0 : x1;
// xmax = (x0 > x1) ? x0 : x1;
// ymin = (y0 <= y1) ? y0 : y1;
// ymax = (y0 > y1) ? y0 : y1;
//
// 2. Now set up the drawing box to be the desired rectangle
//
// WriteSpiCommand(PASET); // set the row boundaries
// WriteSpiData(xmin);
// WriteSpiData(xmax);
// WriteSpiCommand(CASET); // set the column boundaries
// WriteSpiData(ymin);
// WriteSpiData(ymax);
//


// 3. Calculate the number of pixels to be written divided by 2
//
// NumPixels = ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1)
//
// You may notice that I added one pixel to the formula.
// This covers the case where the number of pixels is odd and we
// would lose one pixel due to rounding error. In the case of
// odd pixels, the number of pixels is exact.
// in the case of even pixels, we have one more pixel than
// needed, but it cannot be displayed because it is outside
// the drawing box.
//
// We divide by 2 because two pixels are represented by three bytes.
// So we work through the rectangle two pixels at a time.
//
// 4. Now a simple memory write loop will fill the rectangle
//
// for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1); i++) {
// WriteSpiData((color >> 4) & 0xFF);
// WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
// WriteSpiData(color & 0xFF);
// }
//
//
// In the case of an unfilled rectangle, drawing four lines with the Bresenham line
// drawing algorithm is reasonably efficient.
//
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************************
void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color) {
int xmin, xmax, ymin, ymax;
int i;
// check if the rectangle is to be filled
if (fill == FILL) {
// best way to create a filled rectangle is to define a drawing box
// and loop two pixels at a time
// calculate the min and max for x and y directions
xmin = (x0 <= x1) ? x0 : x1;
xmax = (x0 > x1) ? x0 : x1;
ymin = (y0 <= y1) ? y0 : y1;
ymax = (y0 > y1) ? y0 : y1;
// specify the controller drawing box according to those limits
// Row address set (command 0x2B)
WriteSpiCommand(PASET);
WriteSpiData(xmin);
WriteSpiData(xmax);
// Column address set (command 0x2A)
WriteSpiCommand(CASET);
WriteSpiData(ymin);
WriteSpiData(ymax);
// WRITE MEMORY
WriteSpiCommand(RAMWR);
// loop on total number of pixels / 2
for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 130); i++) {
// use the color value to output three data bytes covering two pixels
WriteSpiData((color >> 4) & 0xFF);
WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
WriteSpiData(color & 0xFF);
}
} else {
// best way to draw un unfilled rectangle is to draw four lines
LCDSetLine(x0, y0, x1, y0, color);
LCDSetLine(x0, y1, x1, y1, color);
LCDSetLine(x0, y0, x0, y1, color);
LCDSetLine(x1, y0, x1, y1, color);
}
}


// *************************************************************************************
// LCDSetCircle.c
//
// Draws a line in the specified color at center (x0,y0) with radius
//
// Inputs: x0 = row address (0 .. 131)
// y0 = column address (0 .. 131)
// radius = radius in pixels
// color = 12-bit color value rrrrggggbbbb
//
// Returns: nothing
//
// Author: Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//
// Note: taken verbatim Wikipedia article on Bresenham's line algorithm
// http://www.wikipedia.org
//
// *************************************************************************************
void LCDSetCircle(int x0, int y0, int radius, int color) {
int f = 1 - radius;
int ddF_x = 0;
int ddF_y = -2 * radius;
int x = 0;
int y = radius;
LCDSetPixel(x0, y0 + radius, color);
LCDSetPixel(x0, y0 - radius, color);
LCDSetPixel(x0 + radius, y0, color);
LCDSetPixel(x0 - radius, y0, color);
while(x < y) {
if(f >= 0) {
y--;
ddF_y += 2;
f += ddF_y;
}
x++;
ddF_x += 2;
f += ddF_x + 1;
LCDSetPixel(x0 + x, y0 + y, color);
LCDSetPixel(x0 - x, y0 + y, color);
LCDSetPixel(x0 + x, y0 - y, color);
LCDSetPixel(x0 - x, y0 - y, color);
LCDSetPixel(x0 + y, y0 + x, color);
LCDSetPixel(x0 - y, y0 + x, color);
LCDSetPixel(x0 + y, y0 - x, color);
LCDSetPixel(x0 - y, y0 - x, color);
}
}
// *****************************************************************************
// LCDPutChar.c
//
// Draws an ASCII character at the specified (x,y) address and color
//
// Inputs: c = character to be displayed
// x = row address (0 .. 131)
// y = column address (0 .. 131)
// size = font pitch (SMALL, MEDIUM, LARGE)
// fcolor = 12-bit foreground color value rrrrggggbbbb
// bcolor = 12-bit background color value rrrrggggbbbb
//
// Returns: nothing
//
// Notes: Here's an example to display "E" at address (20,20)
//
// LCDPutChar('E', 20, 20, MEDIUM, WHITE, BLACK);
//
// (27,20) (27,27)
// | |
// | |
// ^ V V
// : _ # # # # # # # 0x7F
// : _ _ # # _ _ _ # 0x31
// : _ _ # # _ # _ _ 0x34
// x _ _ # # # # _ _ 0x3C
// : _ _ # # _ # _ _ 0x34
// : _ _ # # _ _ _ # 0x31
// : _ # # # # # # # 0x7F
// : _ _ _ _ _ _ _ _ 0x00
// ^ ^
// | |
// | |
// (20,20) (20,27)
//
// ------y----------->
//


//
// The most efficient way to display a character is to make use of the "wrap-around" feature
// of the Epson S1D16G00 LCD controller chip.
//
// Assume that we position the character at (20, 20) that's a (row, col) specification.
// With the row and column address set commands, you can specify an 8x8 box for the SMALL and MEDIUM
// characters or a 16x8 box for the LARGE characters.
//
// WriteSpiCommand(PASET); // set the row drawing limits
// WriteSpiData(20); //
// WriteSpiData(27); // limit rows to (20, 27)
//
// WriteSpiCommand(CASET); // set the column drawing limits
// WriteSpiData(20); //
// WriteSpiData(27); // limit columns to (20,27)
//
// When the algorithm completes col 27, the column address wraps back to 20
// At the same time, the row address increases by one (this is done by the controller)
//
// We walk through each row, two pixels at a time. The purpose is to create three
// data bytes representing these two pixels in the following format
//
// Data for pixel 0: RRRRGGGGBBBB
// Data for Pixel 1: RRRRGGGGBBBB
//
// WriteSpiCommand(RAMWR); // start a memory write (96 data bytes to follow)
//
// WriteSpiData(RRRRGGGG); // first pixel, red and green data
// WriteSpiData(BBBBRRRR); // first pixel, blue data; second pixel, red data
// WriteSpiData(GGGGBBBB); // second pixel, green and blue data
// :
// and so on until all pixels displayed!
// :
// WriteSpiCommand(NOP); // this will terminate the RAMWR command
//
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
void LCDPutChar(char c, int x, int y, int size, int fColor, int bColor) {
extern const unsigned char FONT6x8[97][8];
extern const unsigned char FONT8x8[97][8];
extern const unsigned char FONT8x16[97][16];
int i,j;
unsigned int nCols;
unsigned int nRows;
unsigned int nBytes;
unsigned char PixelRow;
unsigned char Mask;
unsigned int Word0;
unsigned int Word1;
unsigned char *pFont;
unsigned char *pChar;
unsigned char *FontTable[] = {(unsigned char *)FONT6x8, (unsigned char *)FONT8x8,
(unsigned char *)FONT8x16};
// get pointer to the beginning of the selected font table
pFont = (unsigned char *)FontTable[size];
// get the nColumns, nRows and nBytes
nCols = *pFont;
nRows = *(pFont + 1);
nBytes = *(pFont + 2);
// get pointer to the last byte of the desired character
pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;
// Row address set (command 0x2B)
WriteSpiCommand(PASET);
WriteSpiData(x);
WriteSpiData(x + nRows - 1);
// Column address set (command 0x2A)
WriteSpiCommand(CASET);
WriteSpiData(y);
WriteSpiData(y + nCols - 1);


// WRITE MEMORY
WriteSpiCommand(RAMWR);
// loop on each row, working backwards from the bottom to the top
for (i = nRows - 1; i >= 0; i--) {
// copy pixel row from font table and then decrement row
PixelRow = *pChar--;
// loop on each pixel in the row (left to right)
// Note: we do two pixels each loop
Mask = 0x80;
for (j = 0; j < nCols; j += 2) {
// if pixel bit set, use foreground color; else use the background color
// now get the pixel color for two successive pixels
if ((PixelRow & Mask) == 0)
Word0 = bColor;
else
Word0 = fColor;
Mask = Mask >> 1;
if ((PixelRow & Mask) == 0)
Word1 = bColor;
else
Word1 = fColor;
Mask = Mask >> 1;
// use this information to output three data bytes
WriteSpiData((Word0 >> 4) & 0xFF);
WriteSpiData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
WriteSpiData(Word1 & 0xFF);
}
}
// terminate the Write Memory command
WriteSpiCommand(NOP);
}
// *************************************************************************************************
// LCDPutStr.c
//
// Draws a null-terminates character string at the specified (x,y) address, size and color
//
// Inputs: pString = pointer to character string to be displayed
// x = row address (0 .. 131)
// y = column address (0 .. 131)
// Size = font pitch (SMALL, MEDIUM, LARGE)
// fColor = 12-bit foreground color value rrrrggggbbbb
// bColor = 12-bit background color value rrrrggggbbbb
//
//
// Returns: nothing
//
// Notes: Here's an example to display "Hello World!" at address (20,20)
//
// LCDPutChar("Hello World!", 20, 20, LARGE, WHITE, BLACK);
//
//
// Author: James P Lynch August 30, 2007
// *************************************************************************************************
void LCDPutStr(char *pString, int x, int y, int Size, int fColor, int bColor) {
// loop until null-terminator is seen
while (*pString != 0x00) {
// draw the character
LCDPutChar(*pString++, x, y, Size, fColor, bColor);
// advance the y position
if (Size == SMALL)
y = y + 6;
else if (Size == MEDIUM)
y = y + 8;
else
y = y + 8;
// bail out if y exceeds 131
if (y > 131) break;
}
}


// *****************************************************************************
// Delay.c
//
// Simple for loop delay
//
// Inputs: a - loop count
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************

// *********************************************************************************
//
// Font tables for Nokia 6610 LCD Display Driver (S1D15G00 Controller)
//
// FONT6x8 - SMALL font (mostly 5x7)
// FONT8x8 - MEDIUM font (8x8 characters, a bit thicker)
// FONT8x16 - LARGE font (8x16 characters, thicker)
//
// Note: ASCII characters 0x00 through 0x1F are not included in these fonts.
//
// Author: Jim Parise, James P Lynch August 30, 2007
// *********************************************************************************
const unsigned char FONT6x8[97][8] = {
0x06,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00, // !
0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x00, // "
0x50,0x50,0xF8,0x50,0xF8,0x50,0x50,0x00, // #
0x20,0x78,0xA0,0x70,0x28,0xF0,0x20,0x00, // $
0xC0,0xC8,0x10,0x20,0x40,0x98,0x18,0x00, // %
0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00, // &
0x30,0x30,0x20,0x40,0x00,0x00,0x00,0x00, // '
0x10,0x20,0x40,0x40,0x40,0x20,0x10,0x00, // (
0x40,0x20,0x10,0x10,0x10,0x20,0x40,0x00, // )
0x00,0x20,0xA8,0x70,0x70,0xA8,0x20,0x00, // *
0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x30,0x30,0x20,0x40, // ,
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00, // .
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00, // / (forward slash)
0x70,0x88,0x88,0xA8,0x88,0x88,0x70,0x00, // 0 0x30
0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00, // 1
0x70,0x88,0x08,0x70,0x80,0x80,0xF8,0x00, // 2
0xF8,0x08,0x10,0x30,0x08,0x88,0x70,0x00, // 3
0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00, // 4
0xF8,0x80,0xF0,0x08,0x08,0x88,0x70,0x00, // 5
0x38,0x40,0x80,0xF0,0x88,0x88,0x70,0x00, // 6
0xF8,0x08,0x08,0x10,0x20,0x40,0x80,0x00, // 7
0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00, // 8
0x70,0x88,0x88,0x78,0x08,0x10,0xE0,0x00, // 9
0x00,0x00,0x20,0x00,0x20,0x00,0x00,0x00, // :
0x00,0x00,0x20,0x00,0x20,0x20,0x40,0x00, // ;
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00, // <
0x00,0x00,0xF8,0x00,0xF8,0x00,0x00,0x00, // =
0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00, // >
0x70,0x88,0x08,0x30,0x20,0x00,0x20,0x00, // ?
0x70,0x88,0xA8,0xB8,0xB0,0x80,0x78,0x00, // @ 0x40
0x20,0x50,0x88,0x88,0xF8,0x88,0x88,0x00, // A
0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00, // B
0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00, // C
0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00, // D
0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00, // E
0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00, // F
0x78,0x88,0x80,0x80,0x98,0x88,0x78,0x00, // G
0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00, // H
0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // I
0x38,0x10,0x10,0x10,0x10,0x90,0x60,0x00, // J
0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00, // K
0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00, // L
0x88,0xD8,0xA8,0xA8,0xA8,0x88,0x88,0x00, // M
0x88,0x88,0xC8,0xA8,0x98,0x88,0x88,0x00, // N
0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // O
0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00, // P 0x50
0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00, // Q
0xF0,0x88,0x88,0xF0,0xA0,0x90,0x88,0x00, // R
0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00, // S
0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x00, // T
0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00, // U
0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00, // V
0x88,0x88,0x88,0xA8,0xA8,0xA8,0x50,0x00, // W
0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00, // X
0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00, // Y
0xF8,0x08,0x10,0x70,0x40,0x80,0xF8,0x00, // Z
0x78,0x40,0x40,0x40,0x40,0x40,0x78,0x00, // [
0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00, // \ (back slash)
0x78,0x08,0x08,0x08,0x08,0x08,0x78,0x00, // ]
0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00, // _
0x60,0x60,0x20,0x10,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x60,0x10,0x70,0x90,0x78,0x00, // a
0x80,0x80,0xB0,0xC8,0x88,0xC8,0xB0,0x00, // b
0x00,0x00,0x70,0x88,0x80,0x88,0x70,0x00, // c
0x08,0x08,0x68,0x98,0x88,0x98,0x68,0x00, // d
0x00,0x00,0x70,0x88,0xF8,0x80,0x70,0x00, // e
0x10,0x28,0x20,0x70,0x20,0x20,0x20,0x00, // f
0x00,0x00,0x70,0x98,0x98,0x68,0x08,0x70, // g
0x80,0x80,0xB0,0xC8,0x88,0x88,0x88,0x00, // h
0x20,0x00,0x60,0x20,0x20,0x20,0x70,0x00, // i
0x10,0x00,0x10,0x10,0x10,0x90,0x60,0x00, // j
0x80,0x80,0x90,0xA0,0xC0,0xA0,0x90,0x00, // k
0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00, // l
0x00,0x00,0xD0,0xA8,0xA8,0xA8,0xA8,0x00, // m
0x00,0x00,0xB0,0xC8,0x88,0x88,0x88,0x00, // n
0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00, // o
0x00,0x00,0xB0,0xC8,0xC8,0xB0,0x80,0x80, // p 0x70
0x00,0x00,0x68,0x98,0x98,0x68,0x08,0x08, // q
0x00,0x00,0xB0,0xC8,0x80,0x80,0x80,0x00, // r
0x00,0x00,0x78,0x80,0x70,0x08,0xF0,0x00, // s
0x20,0x20,0xF8,0x20,0x20,0x28,0x10,0x00, // t
0x00,0x00,0x88,0x88,0x88,0x98,0x68,0x00, // u
0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00, // v
0x00,0x00,0x88,0x88,0xA8,0xA8,0x50,0x00, // w
0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00, // x
0x00,0x00,0x88,0x88,0x78,0x08,0x88,0x70, // y
0x00,0x00,0xF8,0x10,0x20,0x40,0xF8,0x00, // z
0x10,0x20,0x20,0x40,0x20,0x20,0x10,0x00, // {
0x20,0x20,0x20,0x00,0x20,0x20,0x20,0x00, // |
0x40,0x20,0x20,0x10,0x20,0x20,0x40,0x00, // }
0x40,0xA8,0x10,0x00,0x00,0x00,0x00,0x00, // ~
0x70,0xD8,0xD8,0x70,0x00,0x00,0x00,0x00}; // DEL
const unsigned char FONT8x8[97][8] = {
0x08,0x08,0x08,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00, // !
0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00, // "
0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00, // #
0x18,0x3E,0x60,0x3C,0x06,0x7C,0x18,0x00, // $
0x00,0x63,0x66,0x0C,0x18,0x33,0x63,0x00, // %
0x1C,0x36,0x1C,0x3B,0x6E,0x66,0x3B,0x00, // &
0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00, // '
0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00, // (
0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00, // )
0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00, // *
0x00,0x30,0x30,0xFC,0x30,0x30,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30, // ,
0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00, // .
0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00, // / (forward slash)
0x3E,0x63,0x63,0x6B,0x63,0x63,0x3E,0x00, // 0 0x30
0x18,0x38,0x58,0x18,0x18,0x18,0x7E,0x00, // 1
0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00, // 2
0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00, // 3
0x0E,0x1E,0x36,0x66,0x7F,0x06,0x0F,0x00, // 4
0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00, // 5
0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00, // 6
0x7E,0x66,0x06,0x0C,0x18,0x18,0x18,0x00, // 7
0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00, // 8
0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00, // 9
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00, // :
0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30, // ;
0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00, // <
0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00, // =
0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00, // >
0x3C,0x66,0x06,0x0C,0x18,0x00,0x18,0x00, // ?
0x3E,0x63,0x6F,0x69,0x6F,0x60,0x3E,0x00, // @ 0x40
0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00, // A
0x7E,0x33,0x33,0x3E,0x33,0x33,0x7E,0x00, // B
0x1E,0x33,0x60,0x60,0x60,0x33,0x1E,0x00, // C
0x7C,0x36,0x33,0x33,0x33,0x36,0x7C,0x00, // D
0x7F,0x31,0x34,0x3C,0x34,0x31,0x7F,0x00, // E
0x7F,0x31,0x34,0x3C,0x34,0x30,0x78,0x00, // F
0x1E,0x33,0x60,0x60,0x67,0x33,0x1F,0x00, // G
0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00, // H
0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // I
0x0F,0x06,0x06,0x06,0x66,0x66,0x3C,0x00, // J
0x73,0x33,0x36,0x3C,0x36,0x33,0x73,0x00, // K
0x78,0x30,0x30,0x30,0x31,0x33,0x7F,0x00, // L
0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00, // M
0x63,0x73,0x7B,0x6F,0x67,0x63,0x63,0x00, // N
0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00, // O
0x7E,0x33,0x33,0x3E,0x30,0x30,0x78,0x00, // P 0x50
0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00, // Q
0x7E,0x33,0x33,0x3E,0x36,0x33,0x73,0x00, // R
0x3C,0x66,0x30,0x18,0x0C,0x66,0x3C,0x00, // S
0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00, // T
0x66,0x66,0x66,0x66,0x66,0x66,0x7E,0x00, // U
0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00, // V
0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00, // W
0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00, // X
0x66,0x66,0x66,0x3C,0x18,0x18,0x3C,0x00, // Y
0x7F,0x63,0x46,0x0C,0x19,0x33,0x7F,0x00, // Z
0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00, // [
0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00, // \ (back slash)
0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00, // ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF, // _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x3C,0x06,0x3E,0x66,0x3B,0x00, // a
0x70,0x30,0x3E,0x33,0x33,0x33,0x6E,0x00, // b
0x00,0x00,0x3C,0x66,0x60,0x66,0x3C,0x00, // c
0x0E,0x06,0x3E,0x66,0x66,0x66,0x3B,0x00, // d
0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00, // e
0x1C,0x36,0x30,0x78,0x30,0x30,0x78,0x00, // f
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x7C, // g
0x70,0x30,0x36,0x3B,0x33,0x33,0x73,0x00, // h
0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00, // i
0x06,0x00,0x06,0x06,0x06,0x66,0x66,0x3C, // j
0x70,0x30,0x33,0x36,0x3C,0x36,0x73,0x00, // k
0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00, // l
0x00,0x00,0x66,0x7F,0x7F,0x6B,0x63,0x00, // m
0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00, // n
0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00, // o
0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78, // p 0x70
0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F, // q
0x00,0x00,0x6E,0x3B,0x33,0x30,0x78,0x00, // r
0x00,0x00,0x3E,0x60,0x3C,0x06,0x7C,0x00, // s
0x08,0x18,0x3E,0x18,0x18,0x1A,0x0C,0x00, // t
0x00,0x00,0x66,0x66,0x66,0x66,0x3B,0x00, // u
0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00, // v
0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00, // w
0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00, // x
0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x7C, // y
0x00,0x00,0x7E,0x4C,0x18,0x32,0x7E,0x00, // z
0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00, // {
0x0C,0x0C,0x0C,0x00,0x0C,0x0C,0x0C,0x00, // |
0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00, // }
0x3B,0x6E,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x1C,0x36,0x36,0x1C,0x00,0x00,0x00,0x00}; // DEL
const unsigned char FONT8x16[97][16] = {
0x08,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // columns, rows, num_bytes_per_char
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // space 0x20
0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00, // !
0x00,0x63,0x63,0x63,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // "
0x00,0x00,0x00,0x36,0x36,0x7F,0x36,0x36,0x36,0x7F,0x36,0x36,0x00,0x00,0x00,0x00, // #
0x0C,0x0C,0x3E,0x63,0x61,0x60,0x3E,0x03,0x03,0x43,0x63,0x3E,0x0C,0x0C,0x00,0x00, // $
0x00,0x00,0x00,0x00,0x00,0x61,0x63,0x06,0x0C,0x18,0x33,0x63,0x00,0x00,0x00,0x00, // %
0x00,0x00,0x00,0x1C,0x36,0x36,0x1C,0x3B,0x6E,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // &
0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // '
0x00,0x00,0x0C,0x18,0x18,0x30,0x30,0x30,0x30,0x18,0x18,0x0C,0x00,0x00,0x00,0x00, // (
0x00,0x00,0x18,0x0C,0x0C,0x06,0x06,0x06,0x06,0x0C,0x0C,0x18,0x00,0x00,0x00,0x00, // )
0x00,0x00,0x00,0x00,0x42,0x66,0x3C,0xFF,0x3C,0x66,0x42,0x00,0x00,0x00,0x00,0x00, // *
0x00,0x00,0x00,0x00,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00, // +
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00, // ,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // -
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00, // .
0x00,0x00,0x01,0x03,0x07,0x0E,0x1C,0x38,0x70,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00, // / (forward slash)
0x00,0x00,0x3E,0x63,0x63,0x63,0x6B,0x6B,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 0 0x30
0x00,0x00,0x0C,0x1C,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00, // 1
0x00,0x00,0x3E,0x63,0x03,0x06,0x0C,0x18,0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00, // 2
0x00,0x00,0x3E,0x63,0x03,0x03,0x1E,0x03,0x03,0x03,0x63,0x3E,0x00,0x00,0x00,0x00, // 3
0x00,0x00,0x06,0x0E,0x1E,0x36,0x66,0x66,0x7F,0x06,0x06,0x0F,0x00,0x00,0x00,0x00, // 4
0x00,0x00,0x7F,0x60,0x60,0x60,0x7E,0x03,0x03,0x63,0x73,0x3E,0x00,0x00,0x00,0x00, // 5
0x00,0x00,0x1C,0x30,0x60,0x60,0x7E,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 6
0x00,0x00,0x7F,0x63,0x03,0x06,0x06,0x0C,0x0C,0x18,0x18,0x18,0x00,0x00,0x00,0x00, // 7
0x00,0x00,0x3E,0x63,0x63,0x63,0x3E,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // 8
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x3F,0x03,0x03,0x06,0x3C,0x00,0x00,0x00,0x00, // 9
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00, // :
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00, // ;
0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00, // <
0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00, // =
0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00, // >
0x00,0x00,0x3E,0x63,0x63,0x06,0x0C,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00, // ?
0x00,0x00,0x3E,0x63,0x63,0x6F,0x6B,0x6B,0x6E,0x60,0x60,0x3E,0x00,0x00,0x00,0x00, // @ 0x40
0x00,0x00,0x08,0x1C,0x36,0x63,0x63,0x63,0x7F,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // A
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x33,0x33,0x33,0x33,0x7E,0x00,0x00,0x00,0x00, // B
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x60,0x60,0x61,0x33,0x1E,0x00,0x00,0x00,0x00, // C
0x00,0x00,0x7C,0x36,0x33,0x33,0x33,0x33,0x33,0x33,0x36,0x7C,0x00,0x00,0x00,0x00, // D
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00, // E
0x00,0x00,0x7F,0x33,0x31,0x34,0x3C,0x34,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // F
0x00,0x00,0x1E,0x33,0x61,0x60,0x60,0x6F,0x63,0x63,0x37,0x1D,0x00,0x00,0x00,0x00, // G
0x00,0x00,0x63,0x63,0x63,0x63,0x7F,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // H
0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00, // I
0x00,0x00,0x0F,0x06,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00,0x00,0x00,0x00, // J
0x00,0x00,0x73,0x33,0x36,0x36,0x3C,0x36,0x36,0x33,0x33,0x73,0x00,0x00,0x00,0x00, // K
0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x33,0x7F,0x00,0x00,0x00,0x00, // L
0x00,0x00,0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // M
0x00,0x00,0x63,0x63,0x73,0x7B,0x7F,0x6F,0x67,0x63,0x63,0x63,0x00,0x00,0x00,0x00, // N
0x00,0x00,0x1C,0x36,0x63,0x63,0x63,0x63,0x63,0x63,0x36,0x1C,0x00,0x00,0x00,0x00, // O
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // P 0x50
0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x63,0x6B,0x6F,0x3E,0x06,0x07,0x00,0x00, // Q
0x00,0x00,0x7E,0x33,0x33,0x33,0x3E,0x36,0x36,0x33,0x33,0x73,0x00,0x00,0x00,0x00, // R
0x00,0x00,0x3E,0x63,0x63,0x30,0x1C,0x06,0x03,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // S
0x00,0x00,0xFF,0xDB,0x99,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00, // T
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // U
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x36,0x1C,0x08,0x00,0x00,0x00,0x00, // V
0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x6B,0x6B,0x7F,0x36,0x36,0x00,0x00,0x00,0x00, // W
0x00,0x00,0xC3,0xC3,0x66,0x3C,0x18,0x18,0x3C,0x66,0xC3,0xC3,0x00,0x00,0x00,0x00, // X
0x00,0x00,0xC3,0xC3,0xC3,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00, // Y
0x00,0x00,0x7F,0x63,0x43,0x06,0x0C,0x18,0x30,0x61,0x63,0x7F,0x00,0x00,0x00,0x00, // Z
0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00,0x00,0x00, // [
0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x07,0x03,0x01,0x00,0x00,0x00,0x00, // \ (back slash)
0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,0x00, // ]
0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ^
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00, // _
0x18,0x18,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ` 0x60
0x00,0x00,0x00,0x00,0x00,0x3C,0x46,0x06,0x3E,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // a
0x00,0x00,0x70,0x30,0x30,0x3C,0x36,0x33,0x33,0x33,0x33,0x6E,0x00,0x00,0x00,0x00, // b
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x60,0x60,0x60,0x63,0x3E,0x00,0x00,0x00,0x00, // c
0x00,0x00,0x0E,0x06,0x06,0x1E,0x36,0x66,0x66,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // d
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,0x7E,0x60,0x63,0x3E,0x00,0x00,0x00,0x00, // e
0x00,0x00,0x1C,0x36,0x32,0x30,0x7C,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // f
0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x3E,0x06,0x66,0x3C,0x00,0x00, // g
0x00,0x00,0x70,0x30,0x30,0x36,0x3B,0x33,0x33,0x33,0x33,0x73,0x00,0x00,0x00,0x00, // h
0x00,0x00,0x0C,0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00, // i
0x00,0x00,0x06,0x06,0x00,0x0E,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00,0x00, // j
0x00,0x00,0x70,0x30,0x30,0x33,0x33,0x36,0x3C,0x36,0x33,0x73,0x00,0x00,0x00,0x00, // k
0x00,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00, // l
0x00,0x00,0x00,0x00,0x00,0x6E,0x7F,0x6B,0x6B,0x6B,0x6B,0x6B,0x00,0x00,0x00,0x00, // m
0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x33,0x33,0x00,0x00,0x00,0x00, // n
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00,0x00, // o
0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x3E,0x30,0x30,0x78,0x00,0x00, // p 0x70
0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x3E,0x06,0x06,0x0F,0x00,0x00, // q
0x00,0x00,0x00,0x00,0x00,0x6E,0x3B,0x33,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00, // r
0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x38,0x0E,0x03,0x63,0x3E,0x00,0x00,0x00,0x00, // s
0x00,0x00,0x08,0x18,0x18,0x7E,0x18,0x18,0x18,0x18,0x1B,0x0E,0x00,0x00,0x00,0x00, // t
0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x3B,0x00,0x00,0x00,0x00, // u
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x36,0x36,0x1C,0x1C,0x08,0x00,0x00,0x00,0x00, // v
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,0x6B,0x6B,0x7F,0x36,0x00,0x00,0x00,0x00, // w
0x00,0x00,0x00,0x00,0x00,0x63,0x36,0x1C,0x1C,0x1C,0x36,0x63,0x00,0x00,0x00,0x00, // x
0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x3F,0x03,0x06,0x3C,0x00,0x00, // y
0x00,0x00,0x00,0x00,0x00,0x7F,0x66,0x0C,0x18,0x30,0x63,0x7F,0x00,0x00,0x00,0x00, // z
0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00, // {
0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00, // |
0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00, // }
0x00,0x00,0x3B,0x6E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ~
0x00,0x70,0xD8,0xD8,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; // DEL







void lcdSetContrast(unsigned char contrast) {

  WriteSpiCommand(VOLCTR);
  WriteSpiData(32+contrast); // contrast
  WriteSpiData(3); // contrast
}



//Global variables
static uint_t y;
static uint_t x;
static uint16_t foregroundColor;
static uint16_t backgroundColor;

void lcdInit(void)
{
   y = 0;
   x = 0;
   foregroundColor = WHITE;
   backgroundColor = BLACK;

   //SPI initialization
   spiInit();

//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   //Configure LCD RST as an output
   LCD_RST_BASE_PIO->PIO_PER = LCD_RST_MASK;
   LCD_RST_BASE_PIO->PIO_CODR = LCD_RST_MASK;
   LCD_RST_BASE_PIO->PIO_OER = LCD_RST_MASK;

   //Configure LCD CS as an output
   LCD_CS_BASE_PIO->PIO_PER = LCD_CS_MASK;
   LCD_CS_BASE_PIO->PIO_SODR = LCD_CS_MASK;
   LCD_CS_BASE_PIO->PIO_OER = LCD_CS_MASK;

   //Configure the backlight control pin
   LCD_BL_BASE_PIO->PIO_PER = LCD_BL_MASK;
   LCD_BL_BASE_PIO->PIO_CODR = LCD_BL_MASK;
   LCD_BL_BASE_PIO->PIO_OER = LCD_BL_MASK;

   //Take the LCD controller out of reset
   Delay(10000);
   LCD_RST_BASE_PIO->PIO_SODR = LCD_RST_MASK;
   Delay(10000);

//LPC1766-STK or LPC2378-STK evaluation board?
#elif defined(USE_LPC1766_STK) || defined(USE_LPC2378_STK)
   //Configure LCD RST as an output
   LCD_RST_FIOCLR = LCD_RST_MASK;
   LCD_RST_FIODIR |= LCD_RST_MASK;

   //Configure LCD CS as an output
   LCD_CS_FIOSET = LCD_CS_MASK;
   LCD_CS_FIODIR |= LCD_CS_MASK;

   //Configure the backlight control pin
   LCD_BL_FIOCLR = LCD_BL_MASK;
   LCD_BL_FIODIR |= LCD_BL_MASK;

   //Take the LCD controller out of reset
   Delay(10000);
   LCD_RST_FIOSET = LCD_RST_MASK;
   Delay(10000);

//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   {
      GPIO_InitTypeDef GPIO_InitStructure;

      //Enable GPIOB and GPIOD clocks
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);

      //Configure LCD RST as an output
      GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(LCD_RST_GPIO, &GPIO_InitStructure);

      //Configure LCD CS as an output
      GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
      GPIO_Init(LCD_CS_GPIO, &GPIO_InitStructure);

      //Configure the backlight control pin
      GPIO_InitStructure.GPIO_Pin = LCD_BL_PIN;
      GPIO_Init(LCD_BL_GPIO, &GPIO_InitStructure);

      //Disable backlight
      GPIO_ResetBits(LCD_BL_GPIO, LCD_BL_PIN);
      //Deassert CS
      GPIO_SetBits(LCD_CS_GPIO, LCD_CS_PIN);

      //Reset LCD controller
      GPIO_ResetBits(LCD_RST_GPIO, LCD_RST_PIN);
      Delay(10000);
      GPIO_SetBits(LCD_RST_GPIO, LCD_RST_PIN);
      Delay(10000);
   }
//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   {
      GPIO_InitTypeDef GPIO_InitStructure;

      //Enable GPIO clock
      SCU_APBPeriphClockConfig(__GPIO0, ENABLE);

      //Configure LCD CS as an output
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
      GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
      GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt1;
      GPIO_Init(GPIO0, &GPIO_InitStructure);

      //Deassert CS
      GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_SET);

      //Reset LCD controller
      Delay(1000);
   }
#endif

   //Configure LCD controller
   InitLcd();

   lcdSetContrast(4);
}


void lcdSetBacklight(bool_t state)
{
//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   //Check LCD backlight state
   if(state)
      LCD_BL_BASE_PIO->PIO_SODR = LCD_BL_MASK;
   else
      LCD_BL_BASE_PIO->PIO_CODR = LCD_BL_MASK;

//LPC1766-STK or LPC2378-STK evaluation board?
#elif defined(USE_LPC1766_STK) || defined(USE_LPC2378_STK)
   //Check LCD backlight state
   if(state)
      LCD_BL_FIOSET = LCD_BL_MASK;
   else
      LCD_BL_FIOCLR = LCD_BL_MASK;

//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   //Check LCD backlight state
   if(state)
      GPIO_SetBits(LCD_BL_GPIO, LCD_BL_PIN);
   else
      GPIO_ResetBits(LCD_BL_GPIO, LCD_BL_PIN);
//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   //Not implemented
#endif
}


void lcdSetForegroundColor(uint16_t color)
{
   //Save foreground color
   foregroundColor = color;
}


void lcdSetBackgroundColor(uint16_t color)
{
   //Save background color
   backgroundColor = color;
}


/**
 * @brief Set cursor location
 * @param[in] line Line number
 * @param[in] column Column number
 **/

void lcdSetCursor(uint_t line, uint_t column)
{
   y = MIN(line, 8);
   x = MIN(column, 16);
}


void lcdClear()
{
   uint_t i;
// Row address set (command 0x2B)
WriteSpiCommand(PASET);
WriteSpiData(0);
WriteSpiData(131);
// Column address set (command 0x2A)
WriteSpiCommand(CASET);
WriteSpiData(0);
WriteSpiData(131);
// set the display memory to BLACK
WriteSpiCommand(RAMWR);
for(i = 0; i < ((131 * 131) / 2); i++) {
WriteSpiData((backgroundColor >> 4) & 0xFF);
WriteSpiData(((backgroundColor & 0xF) << 4) | ((backgroundColor >> 8) & 0xF));
WriteSpiData(backgroundColor & 0xFF);
}
}


/**
 * @brief Write a character to the LCD display
 * @param[in] c Character to be written
 **/

void lcdPutChar(char_t c)
{
   if(c == '\r')
   {
      x = 0;
   }
   else if(c == '\n')
   {
      x = 0;
      y++;
   }
   else if(y < 8 && x < 16)
   {
      //Display current character
      LCDPutChar(c, y * 16, x * 8 + 1, LARGE, foregroundColor, backgroundColor);

      //Advance the cursor position
      if(++x >= 16)
      {
         x = 0;
         y++;
      }
   }
}


/**
 * @brief SPI module initialization
 **/

void spiInit(void)
{
//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   //Desired bitrate
   uint32_t bitrate = AT91B_MCK / 1000000;

   //Enable the peripheral clock of SPI module
   AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_SPI0;

   //Disable interrupts on SPI pins
   AT91C_BASE_PIOA->PIO_IDR = AT91C_PA18_SPI0_SPCK | AT91C_PA17_SPI0_MOSI | AT91C_PA16_SPI0_MISO;
   //Internal pull-up resistors
   AT91C_BASE_PIOA->PIO_PPUER = AT91C_PA18_SPI0_SPCK | AT91C_PA17_SPI0_MOSI | AT91C_PA16_SPI0_MISO;
   //Assign corresponding pins to Peripheral A function
   AT91C_BASE_PIOA->PIO_ASR = AT91C_PA18_SPI0_SPCK | AT91C_PA17_SPI0_MOSI | AT91C_PA16_SPI0_MISO;
   //Disable the PIO from controlling the corresponding pins
   AT91C_BASE_PIOA->PIO_PDR = AT91C_PA18_SPI0_SPCK | AT91C_PA17_SPI0_MOSI | AT91C_PA16_SPI0_MISO;

   //Disable SPI module
   AT91C_BASE_SPI0->SPI_CR = AT91C_SPI_SPIDIS;
   //Reset SPI module
   AT91C_BASE_SPI0->SPI_CR = AT91C_SPI_SWRST;
   AT91C_BASE_SPI0->SPI_CR = AT91C_SPI_SWRST;

   //Master mode operation
   AT91C_BASE_SPI0->SPI_MR = AT91C_SPI_MODFDIS | AT91C_SPI_MSTR;
   //SPI configuration
   AT91C_BASE_SPI0->SPI_CSR[0] = (bitrate << 8) | AT91C_SPI_BITS_9 | AT91C_SPI_NCPHA;

   //Enable SPI module
   AT91C_BASE_SPI0->SPI_CR = AT91C_SPI_SPIEN;

//LPC1766-STK evaluation board?
#elif defined(USE_LPC1766_STK)
   uint32_t pclk;

   //Configure P1.20 (SCK0), P1.23 (MISO0) and P1.24 (MOSI0)
   LPC_PINCON->PINSEL3 &= ~(PINSEL3_P1_20_MASK | PINSEL3_P1_23_MASK | PINSEL3_P1_24_MASK);
   LPC_PINCON->PINSEL3 |= PINSEL3_P1_20_SCK0 | PINSEL3_P1_23_MISO0 | PINSEL3_P1_24_MOSI0;

   //Check the SSP0 peripheral clock
   switch(LPC_SC->PCLKSEL1 & PCLKSEL1_PCLK_SSP0)
   {
   case PCLKSEL1_PCLK_SSP0_DIV1:
      pclk = SystemCoreClock;
      break;
   case PCLKSEL1_PCLK_SSP0_DIV2:
      pclk = SystemCoreClock / 2;
      break;
   case PCLKSEL1_PCLK_SSP0_DIV4:
      pclk = SystemCoreClock / 4;
      break;
   default:
      pclk = SystemCoreClock / 8;
      break;
   }

   //Power up SSP0
   LPC_SC->PCONP |= PCONP_PCSSP0;

   //SSP0 module initialization
   LPC_SSP0->CR0 = CR0_DSS_9;
   LPC_SSP0->CR1 = 0;

   //Set clock prescaler
   LPC_SSP0->CPSR = pclk / 1000000;

   //Disable all interrupts
   LPC_SSP0->IMSC = 0;
   //Disable DMA
   LPC_SSP0->DMACR = 0;

   //Enable SPI module
   LPC_SSP0->CR1 |= CR1_SSE;

//LPC2378-STK evaluation board?
#elif defined(USE_LPC2378_STK)
   uint32_t pclk;

   //Configure P1.20 (SCK0), P1.23 (MISO0) and P1.24 (MOSI0)
   PINSEL3 &= ~(PINSEL3_P1_20_MASK | PINSEL3_P1_23_MASK | PINSEL3_P1_24_MASK);
   PINSEL3 |= PINSEL3_P1_20_SCK0 | PINSEL3_P1_23_MISO0 | PINSEL3_P1_24_MOSI0;

   //Check the SSP0 peripheral clock
   switch(PCLKSEL1 & PCLKSEL1_PCLK_SSP0)
   {
   case PCLKSEL1_PCLK_SSP0_DIV1:
      pclk = CPU_CLOCK_FREQ;
      break;
   case PCLKSEL1_PCLK_SSP0_DIV2:
      pclk = CPU_CLOCK_FREQ / 2;
      break;
   case PCLKSEL1_PCLK_SSP0_DIV4:
      pclk = CPU_CLOCK_FREQ / 4;
      break;
   default:
      pclk = CPU_CLOCK_FREQ / 8;
      break;
   }

   //Power up SSP0
   PCONP |= PCONP_PCSSP0;

   //SSP0 module initialization
   SSP0CR0 = CR0_DSS_9;
   SSP0CR1 = 0;

   //Set clock prescaler
   SSP0CPSR = pclk / 1000000;

   //Disable all interrupts
   SSP0IMSC = 0;
   //Disable DMA
   SSP0DMACR = 0;

   //Enable SPI module
   SSP0CR1 |= CR1_SSE;

//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   {
      GPIO_InitTypeDef GPIO_InitStructure;

      //Enable GPIOA, GPIOB and GPIOC clocks
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
         RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

      //Configure LCD SCK as an output
      GPIO_InitStructure.GPIO_Pin = LCD_SCK_PIN;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_Init(LCD_SCK_GPIO, &GPIO_InitStructure);

      //Configure LCD MOSI as an output
      GPIO_InitStructure.GPIO_Pin = LCD_MOSI_PIN;
      GPIO_Init(LCD_MOSI_GPIO, &GPIO_InitStructure);

      //Set SCK and MOSI to default value
      GPIO_ResetBits(LCD_SCK_GPIO, LCD_SCK_PIN);
      GPIO_ResetBits(LCD_MOSI_GPIO, LCD_MOSI_PIN);
   }
//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   {
      GPIO_InitTypeDef GPIO_InitStructure;
      SSP_InitTypeDef SSP_InitStructure;

      //Enable GPIO3 clock
      SCU_APBPeriphClockConfig(__GPIO3, ENABLE);
      //Enable SSP1 clock
      SCU_APBPeriphClockConfig(__SSP1, ENABLE);

      //Configure SSP1_CLK and SSP1_MOSI
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
      GPIO_InitStructure.GPIO_Direction = GPIO_PinOutput;
      GPIO_InitStructure.GPIO_Type = GPIO_Type_PushPull ;
      GPIO_InitStructure.GPIO_Alternate = GPIO_OutputAlt2;
      GPIO_Init(GPIO3, &GPIO_InitStructure);

      //SSP1 configuration
      SSP_DeInit(SSP1);
      SSP_InitStructure.SSP_FrameFormat = SSP_FrameFormat_Motorola;
      SSP_InitStructure.SSP_Mode = SSP_Mode_Master;
      SSP_InitStructure.SSP_CPOL = SSP_CPOL_Low;
      SSP_InitStructure.SSP_CPHA = SSP_CPHA_1Edge;
      SSP_InitStructure.SSP_DataSize = SSP_DataSize_9b;
      SSP_InitStructure.SSP_ClockRate = 0;
      SSP_InitStructure.SSP_ClockPrescaler = 50;
      SSP_Init(SSP1, &SSP_InitStructure);

      //Enable SSP1
      SSP_Cmd(SSP1, ENABLE);
   }
#endif
}


/**
 * @brief Assert CS
 **/

void spiAssertCs(void)
{
//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   //Assert CS signal
   LCD_CS_BASE_PIO->PIO_CODR = LCD_CS_MASK;
//LPC1766-STK or LPC2378-STK evaluation board?
#elif defined(USE_LPC1766_STK) || defined(USE_LPC2378_STK)
   //Assert CS signal
   LCD_CS_FIOCLR = LCD_CS_MASK;
//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   //Assert CS signal
   GPIO_ResetBits(LCD_CS_GPIO, LCD_CS_PIN);
//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   //Assert CS signal
   GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_RESET);
#endif
}


/**
 * @brief Deassert CS
 **/

void spiDeassertCs(void)
{
//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   //Deassert CS signal
   LCD_CS_BASE_PIO->PIO_SODR = LCD_CS_MASK;
//LPC1766-STK or LPC2378-STK evaluation board?
#elif defined(USE_LPC1766_STK) || defined(USE_LPC2378_STK)
   //Deassert CS signal
   LCD_CS_FIOSET = LCD_CS_MASK;
//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   //Deassert CS signal
   GPIO_SetBits(LCD_CS_GPIO, LCD_CS_PIN);
//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   //Deassert CS signal
   GPIO_WriteBit(GPIO0, GPIO_Pin_1, Bit_SET);
#endif
}

/**
 * Transfer a single data word
 * @param[in] data Data to be transmitted to the slave device
 * @return Received data received from the slave device
 **/

uint16_t spiTransfer(uint16_t data)
{
//SAM7-EX256 evaluation board?
#if defined(USE_SAM7_EX256)
   //Ensure the TX buffer is empty
   while(!(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TDRE));
   //Start to transfer data
   AT91C_BASE_SPI0->SPI_TDR = data;
   //Wait for the operation to complete
   while(!(AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_RDRF));
   //Return the received data
   return (uint16_t) AT91C_BASE_SPI0->SPI_RDR;

//LPC1766-STK evaluation board?
#elif defined(USE_LPC1766_STK)
   //Ensure the transmit FIFO is not full
   while(!LPC_SSP0->SR & SR_TNF);
   //Start to transfer data
   LPC_SSP0->DR = data & DR_DATA;
   //Wait for the operation to complete
   while(LPC_SSP0->SR & SR_BSY);
   //Return the received data
   return (uint16_t) LPC_SSP0->DR;

//LPC2378-STK evaluation board?
#elif defined(USE_LPC2378_STK)
   //Ensure the transmit FIFO is not full
   while(!SSP0SR & SR_TNF);
   //Start to transfer data
   SSP0DR = data & DR_DATA;
   //Wait for the operation to complete
   while(SSP0SR & SR_BSY);
   //Return the received data
   return (uint16_t) SSP0DR;

//STM32-P407 evaluation board?
#elif defined(USE_STM32_P407)
   {
      uint_t i;

      //Send data
      for(i = 0; i < 9; i++)
      {
         //Check data value
         if(data & 0x100)
            GPIO_SetBits(LCD_MOSI_GPIO, LCD_MOSI_PIN);
         else
            GPIO_ResetBits(LCD_MOSI_GPIO, LCD_MOSI_PIN);

         //Set SCK signal
         Delay(10);
         GPIO_SetBits(LCD_SCK_GPIO, LCD_SCK_PIN);
         Delay(10);
         GPIO_ResetBits(LCD_SCK_GPIO, LCD_SCK_PIN);
         Delay(10);

         //Rotate data
         data <<= 1;
      }

      //Set MOSI to default state
      GPIO_ResetBits(LCD_MOSI_GPIO, LCD_MOSI_PIN);

      //Return the received data
      return (uint16_t) 0;
   }

//STR-E912 evaluation board?
#elif defined(USE_STR_E912)
   while(SSP_GetFlagStatus(SSP1, SSP_FLAG_TxFifoNotFull) == RESET);
   //Start to transfer data
   SSP_SendData(SSP1, data);
   //Return the received data
   return 0;
#endif
}
