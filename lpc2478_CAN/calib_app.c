/**
 * @defgroup app
 * @file
 * @brief Touch screen calibration
 *
 * @author  Morten Opprud <morten@hih.au.dk>
 * @version 1.0
 * @date    28/10/2010
 *
 * @section DESCRIPTION
 * This is the lcd touch screen applications
 * It sets up the touch controller, and performs calibration
 *
 * @todo add a non volatile storage e.g EEPROM to store parameters
 */
#include <stdio.h>
/* LCD includes */
#include "drivers\swim\lpc_types.h"
#include "drivers\swim\lpc_swim.h"
#include "drivers\swim\lpc_swim_font.h"
#include "drivers\swim\lpc_swim.h"
#include "drivers\swim\lpc_rom8x16.h"
#include "drivers\swim\lpc_winfreesystem14x16.h"
#include "drivers\swim\lpc_x6x13.h"
#include "drivers\lcd\lpc_lcd_params.h"      /* LCD panel parameters */
#include "drivers\lcd\lcd_driver.h"
#include "drivers\touch.h"
#include "drivers\timer_delay.h"
#include "drivers\spi.h"
#include "drivers\lcd\ex_sdram.h"
#include "type.h"
#include "img\crsr.h"

/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/

#define BACK_COLOR WHITE
#define FORE_COLOR BLACK

/******************************************************************************
 * Local variables
 *****************************************************************************/
SWIM_WINDOW_T win1;

static int calibPoint = 0;
static tTouchPoint refPoint1 = { 15, 300 - 18 };
static tTouchPoint refPoint2 = { 120, 80 - 18 };
static tTouchPoint refPoint3 = { 220, 280 - 18 };

static tTouchPoint scr1 = { 0, 0 };
static tTouchPoint scr2 = { 0, 0 };
static tTouchPoint scr3 = { 0, 0 };

static tBool releaseNeeded = FALSE;
static tBool touchReleased = FALSE;

/* DEFINES */
/*  lcd defs */
long dev_lcd;

#define EA2478_LCD_FRAME_BUF 0xA0000000
#define LCD_DISPLAY truly_tft_g240320ltsw_118w_e_portrait	// EA2478 built-in display
#define FONT  font_winfreesys14x16  //font_x6x13 // font_rom8x16 font_rom8x16 font_winfreesys14x16
#define LINE_GAP 20
#define SCREN_END_Y 280

#define CURSOR_32PX	0

/*gui layout*/
#define	LCD_RIGHT		240

#define BUTTON_WIDTH	100
#define BUTTON_HEIGHT	30

#define BUTTON_1_Y		260
#define BUTTON_1_X		5
#define BUTTON_1_WIDTH	BUTTON_WIDTH
#define BUTTON_1_HEIGHT	BUTTON_HEIGHT
#define BUTTON_1_COLOR	RED
#define BUTTON_1_TEXT	"OFF"

#define BUTTON_2_Y		260
#define BUTTON_2_X		LCD_RIGHT-5-BUTTON_WIDTH
#define BUTTON_2_WIDTH	BUTTON_WIDTH
#define BUTTON_2_HEIGHT	BUTTON_HEIGHT
#define BUTTON_2_COLOR	GREEN
#define BUTTON_2_TEXT	"ON"

/*****************************************************************************
 *
 * Description:
 *    draw a square for calibration.
 *
 * Parameters:
 *    [in] x - Screen coordinate
 *    [in] y - Screen coordinate
 *
 ****************************************************************************/
static void drawCalibPoint(tU16 x, tU16 y)
{
	//lcd_fillScreen(BACK_COLOR);
	//paintMessage("Calibrating, touch marked box...");
	//lcd_drawRect(x - 5, y - 5, x + 5, y + 5, BLACK);
	swim_put_box(&win1, x - 5, y - 5, x + 5, y + 5);
}

static void init_lcd(void)
{
	//COLOR_T clr, *fblog;
	COLOR_T *fblog;
	char str[40];

	/* Set LCD frame buffer address */
	fblog = (COLOR_T *) EA2478_LCD_FRAME_BUF;
	/* Create a SWIM window */
	swim_window_open(&win1, LCD_DISPLAY.pixels_per_line, LCD_DISPLAY.lines_per_panel, fblog, 0, 0,
			(LCD_DISPLAY.pixels_per_line - 1), (LCD_DISPLAY.lines_per_panel - 1), 1, LIGHTGRAY, WHITE, LIGHTGRAY);

	/* select the font to use */
	swim_set_font(&win1, (FONT_T *) &FONT);

	/* set the pen color to use */
	swim_set_pen_color(&win1, BLACK);

	/* Add a title bar */
	swim_set_title(&win1, "    -- Touch screen calibration --", LIGHTGRAY);

	/* set the location to write text */
	//swim_set_xy(&win1, 60, 160);

	/* set the pen color to use */
	swim_set_pen_color(&win1, BLACK);

	/* select the font to use */
	swim_set_font(&win1, (FONT_T *) &font_rom8x16);

	sprintf(str, "Calibrating, touch boxes");
	swim_set_xy(&win1, 15, 30);
	swim_put_text(&win1, str);

}

int open_lcd()
{

	/* Open LCD */
	if ((dev_lcd = lcd_open(CLCDC, (long) &LCD_DISPLAY)) == 0x0)
	{
		return FAIL;
	}

	/* pass SPI function to the LCD driver */
	lcd_ioctl(dev_lcd, LCD_SET_SPIFUNC, SpiXfer);

	/* Set LCD display pointer */
	lcd_ioctl(dev_lcd, LCD_SET_BUFFER, (long) SDRAM_BASE_ADDR);

	/* Enable LCD */
	lcd_ioctl(dev_lcd, LCD_ENABLE, 1);

	/* Turn on LCD */
	lcd_ioctl(dev_lcd, LCD_PWR_ON, 1);

	/*Create a HW cursor*/
	/* set cursor size to 32x32 */
	lcd_ioctl(dev_lcd, LCD_CRSR_SIZE, CURSOR_32PX);
	lcd_ioctl(dev_lcd, LCD_CRSR_NUM, 0);

	/* set cursor image */
	lcd_ioctl(dev_lcd, LCD_CRSR_SET_BUFFER, (long) _crsr);

	/* enable cursor */
	lcd_ioctl(dev_lcd, LCD_CRSR_ENABLE, 1);

	return OK;

}

int lcd_button(int x, int y, int w, int h, int col, char *lbl)
{
	/* select the font to use 4x16 */
	swim_set_font(&win1, (FONT_T *) &FONT);

	swim_set_fill_color(&win1, col);

	swim_put_box(&win1, x, y, x + w, y + h);

	swim_set_font_trasparency(&win1, FALSE);

	swim_put_text_xy(&win1, lbl, x + (BUTTON_WIDTH / 2) - (6 * (strlen(lbl))), y + (BUTTON_HEIGHT / 2) - 10);


}

void lcd_test(void)
{
	signed int x, y, z = 0;
	static int firstRun = 0;

	if (!firstRun)
	{
		firstRun = 1;
		swim_set_title(&win1, "    -- Press key UP to exit --", GREEN);
		lcd_button(BUTTON_1_X, BUTTON_1_Y, BUTTON_1_WIDTH, BUTTON_1_HEIGHT, BUTTON_1_COLOR, BUTTON_1_TEXT);
		lcd_button(BUTTON_2_X, BUTTON_2_Y, BUTTON_2_WIDTH, BUTTON_2_HEIGHT, BUTTON_2_COLOR, BUTTON_2_TEXT);

	}

	touch_xyz(&x, &y, &z);
	//	swim_put_pixel(&win1, x, y);
	/* enable cursor */
	lcd_ioctl(dev_lcd, LCD_CRSR_XY, (long) ((y << 16) | (x)));

}

void calibrateStart(void)
{
	char str[40];
	tS32 x, y, z = 0;

	tBool calibrated = FALSE;
	releaseNeeded = FALSE;
	calibPoint = 0;

	/* SSD1289 fclk max 13MHz, clk HI idle, MSB first */
	/* SPIInit(wClock, nFramesize, bCPHA, bCPOL, bLSBF)*/
	SPIInit(CLK5M, 8, 0, 1, 0);

	/* open and init LCD */
	open_lcd();

	// create a SWIM window on the LCD
	init_lcd();

	/* tsc2046:fclk max 2.5MHz, clk LO idle, MSB first
	 * SPIInit(wClock, nFramesize, bCPHA, bCPOL, bLSBF)*/
	SPIInit(CLK2M5, 8, 0, 0, 0);

	/* init touch driver with spi function */
	touch_init(SpiXfer);

	// draw point 1
	drawCalibPoint(refPoint1.x, refPoint1.y);

	touch_xyz(&x, &y, &z);

	while (!calibrated)
	{
		delayMs(100);

		touch_xyz(&x, &y, &z);

		if (z == 0)
		{
			touchReleased = TRUE;
			continue;
		}

		if (releaseNeeded && !touchReleased)
			continue;

		touchReleased = FALSE;

		switch (calibPoint++)
		{
		case 0:
			scr1.x = x;
			scr1.y = y;
			drawCalibPoint(refPoint2.x, refPoint2.y);

			releaseNeeded = TRUE;
			break;
		case 1:
			scr2.x = x;
			scr2.y = y;
			drawCalibPoint(refPoint3.x, refPoint3.y);

			releaseNeeded = TRUE;
			break;
		case 2:
			scr3.x = x;
			scr3.y = y;
			//lcd_fillScreen(BACK_COLOR);
			swim_clear_screen(&win1, BACK_COLOR);
			//			paintMessage("Calibrated!");
			sprintf(str, "Calibrated !");
			swim_set_xy(&win1, 15, 30);
			swim_put_text(&win1, str);

			releaseNeeded = TRUE;
			touch_calibrate(refPoint1, refPoint2, refPoint3, scr1, scr2, scr3);

			calibrated = TRUE;
			break;

		}
	}
}
