#define LCD_WIDTH      240
#define LCD_HEIGHT     320

#define LCD_DELAY         0xFFFF
#define LCD_DELAY8        0xFF

#define RD_OUT            PORTC
#define RD_PIN           2

#define WR_OUT            PORTC
#define WR_PIN           3

#define CD_OUT            PORTC
#define CD_PIN           4

#define CS_OUT            PORTC
#define CS_PIN           5

#define RESET_OUT         PORTC
#define RESET_PIN        6

#define PIN_LOW(p, b)     (p) &= ~(1 << (b))
#define PIN_HIGH(p, b)    (p) |= (1 << (b))
#define PIN_OUTPUT(p, b)  *(&p - 1) |= (1 << (b))

#define RD_ACTIVE()       PIN_LOW(RD_OUT, RD_PIN)
#define RD_IDLE()         PIN_HIGH(RD_OUT, RD_PIN)
#define RD_OUTPUT()       PIN_OUTPUT(RD_OUT, RD_PIN)

#define WR_ACTIVE()       PIN_LOW(WR_OUT, WR_PIN)
#define WR_IDLE()         PIN_HIGH(WR_OUT, WR_PIN)
#define WR_OUTPUT()       PIN_OUTPUT(WR_OUT, WR_PIN)

#define CD_COMMAND()      PIN_LOW(CD_OUT, CD_PIN)
#define CD_DATA()         PIN_HIGH(CD_OUT, CD_PIN)
#define CD_OUTPUT()       PIN_OUTPUT(CD_OUT, CD_PIN)

#define CS_ACTIVE()       PIN_LOW(CS_OUT, CS_PIN)
#define CS_IDLE()         PIN_HIGH(CS_OUT, CS_PIN)
#define CS_OUTPUT()       PIN_OUTPUT(CS_OUT, CS_PIN)

#define RESET_ACTIVE()    PIN_LOW(RESET_OUT, RESET_PIN)
#define RESET_IDLE()      PIN_HIGH(RESET_OUT, RESET_PIN)
#define RESET_OUTPUT()    PIN_OUTPUT(RESET_OUT, RESET_PIN)

#define WR_STROBE() \
do { \
	WR_ACTIVE(); \
	WR_IDLE(); \
} while(0)

#define RD_STROBE() \
do { \
	RD_IDLE(); \
	RD_ACTIVE(); \
	RD_ACTIVE(); \
	RD_ACTIVE(); \
} while(0)

#define _WRITE_8(x) \
do { \
	PORTA = x; \
} while(0)

#define WRITE_8(x) \
do { \
	_WRITE_8(x); \
	WR_STROBE(); \
} while(0)

#define WRITE_16(x) \
do { \
	WRITE_8((x) >> 8); \
	WRITE_8(x); \
} while(0)

#define SET_WRITE_DIR() \
do { \
	DDRA = 0xFF; \
} while(0)

#define CTL_INIT() \
do { \
	RD_OUTPUT(); \
	WR_OUTPUT(); \
	CD_OUTPUT(); \
	CS_OUTPUT(); \
	RESET_OUTPUT(); \
} while(0)

#define WRITE_CMD(x) \
do { \
	CD_COMMAND(); \
	WRITE_16(x); \
} while(0)

#define WRITE_DATA(x) \
do { \
	CD_DATA(); \
	WRITE_16(x); \
} while(0)

#define WRITE_CMD_DATA(cmd, data) \
do { \
	CS_ACTIVE(); \
	WRITE_CMD((cmd)); \
	WRITE_DATA((data)); \
	CS_IDLE(); \
} while(0)

#define WRITE_CMD_PARAM_N(cmd, n, block) \
do { \
	uint8_t i, u8; \
	CS_ACTIVE(); \
	WRITE_CMD((cmd)); \
	for(i = 0; i < n; ++i) \
	{ \
		u8 = block[i]; \
		CD_DATA(); \
		WRITE_8(u8); \
	} \
\
	CS_IDLE(); \
} while(0)

#define WRITE_CMD_PARAM_4(cmd, d0, d1, d2, d3) \
do { \
	CS_ACTIVE(); \
	WRITE_CMD((cmd)); \
	CD_DATA(); \
	WRITE_8((d0)); \
	CD_DATA(); \
	WRITE_8((d1)); \
	CD_DATA(); \
	WRITE_8((d2)); \
	CD_DATA(); \
	WRITE_8((d3)); \
	CS_IDLE(); \
} while(0)

#define LCD_WINDOW_BEGIN(x0, y0, x1, y1) \
do { \
	WRITE_CMD_PARAM_4(0x2A, (uint8_t)((x0) >> 8), (uint8_t)(x0), (uint8_t)((x1) >> 8), (uint8_t)(x1)); \
	WRITE_CMD_PARAM_4(0x2B, (uint8_t)((y0) >> 8), (uint8_t)(y0), (uint8_t)((y1) >> 8), (uint8_t)(y1)); \
	CS_ACTIVE(); \
	WRITE_CMD(0x2C); \
	CD_DATA(); \
} while(0)

#define LCD_WINDOW_END() \
do { \
	CS_IDLE(); \
} while(0)

static void lcd_init(void)
{
	static const uint8_t ILI9341[] PROGMEM =
	{
		0x01, 0,
		LCD_DELAY8, 50,
		0x28, 0,
		0xF6, 3, 0x01, 0x01, 0x00,
		0xCF, 3, 0x00, 0x81, 0x30,
		0xED, 4, 0x64, 0x03, 0x12, 0x81,
		0xE8, 3, 0x85, 0x10, 0x78,
		0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
		0xF7, 1, 0x20,
		0xEA, 2, 0x00, 0x00,
		0xB0, 1, 0x00,
		0xB1, 2, 0x00, 0x1B,
		0xB4, 1, 0x00,
		0xC0, 1, 0x21,
		0xC1, 1, 0x11,
		0xC5, 2, 0x3F, 0x3C,
		0xC7, 1, 0xB5,
		0x36, 1, 0x48,
		0xF2, 1, 0x00,
		0x26, 1, 0x01,
		0xE0, 15, 0x0f, 0x26, 0x24, 0x0b, 0x0e, 0x09, 0x54, 0xa8, 0x46, 0x0c, 0x17, 0x09, 0x0f, 0x07, 0x00,
		0xE1, 15, 0x00, 0x19, 0x1b, 0x04, 0x10, 0x07, 0x2a, 0x47, 0x39, 0x03, 0x06, 0x06, 0x30, 0x38, 0x0f,
		0x11, 0,
		LCD_DELAY8, 150,
		0x29, 0,
		0x3A, 1, 0x55,
	};

	uint16_t size;
	uint8_t i, cmd, len, d[16];
	const uint8_t *p;
	SET_WRITE_DIR();
	CTL_INIT();
	CS_IDLE();
	RD_IDLE();
	WR_IDLE();
	RESET_IDLE();
	wait_ms(50);
	RESET_ACTIVE();
	wait_ms(100);
	RESET_IDLE();
	wait_ms(100);
	WRITE_CMD_DATA(0xB0, 0x0000);
	p = ILI9341;
	size = sizeof(ILI9341);
	while(size)
	{
		cmd = pgm_read_byte(p++);
		len = pgm_read_byte(p++);
		if(cmd == LCD_DELAY8)
		{
			wait_ms(len);
			len = 0;
		}
		else
		{
			for(i = 0; i < len; ++i)
			{
				d[i] = pgm_read_byte(p++);
			}

			WRITE_CMD_PARAM_N(cmd, len, d);
		}

		size -= len + 2;
	}

	d[0] = 0x48;
	WRITE_CMD_PARAM_N(0x36, 1, d);
	WRITE_CMD_PARAM_4(0x2A, (uint8_t)((0) >> 8), (uint8_t)(0), (uint8_t)((LCD_WIDTH - 1) >> 8), (uint8_t)(LCD_WIDTH - 1));
	WRITE_CMD_PARAM_4(0x2B, (uint8_t)((0) >> 8), (uint8_t)(0), (uint8_t)((LCD_HEIGHT - 1) >> 8), (uint8_t)(LCD_HEIGHT - 1));
	d[0] = 0;
	d[1] = 0;
	d[2] = (uint8_t)(LCD_HEIGHT >> 8);
	d[3] = (uint8_t)LCD_HEIGHT;
	d[4] = 0;
	d[5] = 0;
	WRITE_CMD_PARAM_N(0x33, 6, d);
	d[0] = 0;
	d[1] = 0;
	WRITE_CMD_PARAM_N(0x37, 2, d);

	CS_ACTIVE();
	WRITE_CMD(0x13);
	CS_IDLE();

	CS_ACTIVE();
	WRITE_CMD(0x20);
	CS_IDLE();
}

