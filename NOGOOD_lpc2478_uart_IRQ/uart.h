#ifndef _UART_H_
#define _UART_H_

//Defines and typedefs

//bit definitions in LCR and FCR registers in the UART
#define ULCR_CHAR_8   0x03
#define ULCR_PAR_NO   0x00
#define ULCR_STOP_1   0x00

//bit definitions in UnIIR registers in the UART
#define UIIR_RLS   0x06
#define UIIR_RDA   0x04
#define UIIR_CTI   0x0C
#define UIIR_THRE  0x02

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

// base addresses
#define UART0_BASE_ADDR		0xE000C000
#define UART1_BASE_ADDR		0xE0010000
#define UART2_BASE_ADDR		0xE0078000
#define UART3_BASE_ADDR		0xE007C000

#define UART0 UART0_BASE_ADDR
#define UART1 UART1_BASE_ADDR
#define UART2 UART2_BASE_ADDR
#define UART3 UART3_BASE_ADDR


typedef struct
{
	unsigned long RBR;
	unsigned long THR;
	unsigned long DLL;
	unsigned long DLM;
	unsigned long IER;
	unsigned long IIR;
	unsigned long FCR;
	unsigned long LCR;
	unsigned long MCR;
	unsigned long LSR;
	unsigned long MSR;
	unsigned long SCR;
	unsigned long ACR;
	unsigned long FDR;
	unsigned long TER;
}uart_t;

// prototypes
int initUart(unsigned long uart, unsigned short div_factor, unsigned char mode);
int sendchar(unsigned long uart, int ch);
char getkey(unsigned long uart);

#endif
