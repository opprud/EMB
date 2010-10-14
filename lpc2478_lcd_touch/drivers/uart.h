#ifndef _UART_H_
#define _UART_H_

//Defines and typedefs

//bit definitions in LCR and FCR registers in the UART
#define ULCR_CHAR_8   0x03
#define ULCR_PAR_NO   0x00
#define ULCR_STOP_1   0x00

//define for determning the correct uart clock division factor
//Note that the expressions should always be constants and fully evaluated at
//compile time, else a lot of code will be generated.
#define UART_BPS(pclk,bps) (unsigned short)((pclk / ((bps) * 16.0)) + 0.5)

//definitions for some common bitrates
#define B1200(pclk)         UART_BPS(pclk,1200)
#define B2400(pclk)         UART_BPS(pclk,2400)
#define B9600(pclk)         UART_BPS(pclk,9600)
#define B19200(pclk)        UART_BPS(pclk,19200)
#define B38400(pclk)        UART_BPS(pclk,38400)
#define B57600(pclk)        UART_BPS(pclk,57600)
#define B115200(pclk)       UART_BPS(pclk,115200)

//definitions for mode settings
#define UART_8N1      (unsigned char)(ULCR_CHAR_8 + ULCR_PAR_NO + ULCR_STOP_1)

// prototypes
void initUart0(unsigned short div_factor, unsigned char mode);
//int sendchar(int ch);
int putchar (int character);
//int getkey(void);
int getchar(void);

#endif
