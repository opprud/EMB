/**********************************
 * Small rprintf test program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "keys.h"
#include "rprintf.h"
#include "timer.h"
#include "VIClowlevel.h"
#include "irq.h"

#define TRUE 1

#define FPGA_BASE_ADDR	0x82000000
#define WBO_REG1       (*(volatile unsigned short *)(FPGA_BASE_ADDR + 0x000))
#define WBO_REG2       (*(volatile unsigned short *)(FPGA_BASE_ADDR + 0x002))
#define WBO_REG3       (*(volatile unsigned short *)(FPGA_BASE_ADDR + 0x004))

#define FPGA_RESET		(1<<19)

/* task timers, in [ms] */
#define TASK0_INTERVAL	230
#define TASK1_INTERVAL	100

typedef enum
{
	CNT_UP, CNT_DWN, PWM_UP, PWM_DWN
} cmd_t;

void myCallback(void)
{
	int i, adVal;
	i++;
	//rprintf("c");
	//HI?
	//if(FIO2PIN & (1<<10))
	if (i % 2)
		FIO2CLR = (1 << 10);
	else
		FIO2SET = (1 << 10);

	adVal = readAdc(2);
}

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	short Sval;
	long Lval;
	short count = 0;
	short pwm = 0x8000;
	int pressed, released;
	cmd_t cmd;

	/* init low level stuff */
	lowLevelInit();

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart0(B115200(Fpclk), UART_8N1);

	// initialize rprintf system
	// - use uartSendByte as the output for all rprintf statements
	//   this will cause all rprintf library functions to direct their
	//   output to the uart
	// - rprintf can be made to output to any device which takes characters.
	//   You must write a function which takes an unsigned char as an argument
	//   and then pass this to rprintfInit like this: rprintfInit(YOUR_FUNCTION);
	rprintfInit(putchar);

	/* set reset FPGA pin to out */FIO2DIR |= FPGA_RESET;

	/*..and LED*/FIO2DIR |= (1 << 10);

	// toggle reset
	FIO2SET = FPGA_RESET;

	// print a little intro message so we know things are working
	rprintf("\r\nWelcome to FPGA + ARM7 test program!\r\n");
	rprintf("\r\nPush joystick \r\n UP/DOWN/RIGHT/LEFT \r\n to light an LED on the Spartan 3A board");

	// toggle reset
	FIO2CLR = FPGA_RESET;

	/* DAC channel w small speaker is P0.26*/

	/* ADC channel 0.25 is AD2 */
	initAdc(2);

	/* init 2 GPIOs for debug */
	//FIO2DIR |= (1 << 21);
	//FIO2DIR |= (1 << 22);
#if 0

	/* init timer 0, to generate IRQ's every 10mS */
	//initTimer(0, 10);
//		initTimerTicks(0, (27000000/3), myCallback);
	/* 57,6MHz / 44.1KHz = 653 (almost) */
	initTimerTicks(0, 326, myCallback);

	/* enable IRQ's globally */
	enableIRQ();

	/* start timer */
	enableTimer(0);

	/* reset soft timers */
	timer(0, RESET);
	timer(1, RESET);
#endif
	while (1)
	{

		/* detect button press/releases */
		if (pressed & released)
		{
			/* evaluate command */
			switch (cmd)
			{
			case CNT_UP:
				count++;
				rprintf("\r\n Count is %d:", count);
				break;
			case CNT_DWN:
				count--;
				rprintf("\r\n Count is %d:", count);
				break;
			case PWM_UP:
				pwm += 0x1000;
				rprintf("\r\n PWM is %d:", pwm);
				break;
			case PWM_DWN:
				pwm -= 0x1000;
				rprintf("\r\n PWM is %d:", pwm);
				break;
			default:
				break;
			}
			pressed = 0;
			released = 0;
		}

		switch (readKeys())
		{
		/*WBO_REG1 is LED outputs*/
		case UP:
			WBO_REG1 = count;
			cmd = CNT_UP;
			pressed = 1;
			break;
		case DOWN:
			WBO_REG1 = count;
			cmd = CNT_DWN;
			pressed = 1;
			break;
			/*WBO_REG2 is LED PWM*/
		case LEFT:
			WBO_REG2 = pwm;
			cmd = PWM_UP;
			pressed = 1;
			break;
		case RIGHT:
			WBO_REG2 = pwm;
			cmd = PWM_DWN;
			pressed = 1;
			break;
		case CENTER:
			WBO_REG1 = 0x0000;
			WBO_REG2 = 0x8000;
			break;
		default:
			released = 1;
			break;

		}
#if 0
		/* task 0, needs service ? */
		if (timer(0, RUN) > TASK0_INTERVAL)
		{
			rprintf("#");
			/*reset soft timer */
			timer(0, RESET); /* toggle GPIO */
			FIO2PIN = ((~FIO2PIN ) & (1 << 22));
		}
#endif
	}
}
