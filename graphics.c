#include "lcd.c"
#include "lcd_font.c"

#define LCD_BLACK             0x0000
#define LCD_BLUE              0x001F
#define LCD_RED               0xF800
#define LCD_GREEN             0x07E0
#define LCD_CYAN              0x07FF
#define LCD_MAGENTA           0xF81F
#define LCD_YELLOW            0xFFE0
#define LCD_WHITE             0xFFFF

#define COLOR_565(R, G, B) \
	((((R) & 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))

static uint16_t _color;

static int16_t abs(int16_t v)
{
	return v < 0 ? -v : v;
}

static void lcd_clear(void)
{
	uint16_t i;
	uint8_t hi, lo;
	i = LCD_WIDTH * (LCD_HEIGHT / 4);
	hi = (uint8_t)(_color >> 8);
	lo = (uint8_t)_color;
	LCD_WINDOW_BEGIN(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	while(i--)
	{
		WRITE_8(hi);
		WRITE_8(lo);

		WRITE_8(hi);
		WRITE_8(lo);

		WRITE_8(hi);
		WRITE_8(lo);

		WRITE_8(hi);
		WRITE_8(lo);
	}

	LCD_WINDOW_END();
}

static void lcd_rect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	uint16_t end;
	uint8_t hi, lo;
	hi = (uint8_t)(_color >> 8);
	lo = (uint8_t)_color;
	if(!w || !h)
	{
		return;
	}

	if(x < 0)
	{
		x = 0;
	}

	if(y < 0)
	{
		y = 0;
	}

	if(x + w >= LCD_WIDTH)
	{
		w = LCD_WIDTH - x;
	}

	if(y + h >= LCD_HEIGHT)
	{
		h = LCD_HEIGHT - y;
	}

	LCD_WINDOW_BEGIN(x, y, x + w - 1, y + h - 1);
	do
	{
		end = w;
		do
		{
			WRITE_8(hi);
			WRITE_8(lo);
		}
		while(--end);
	}
	while(--h);
	LCD_WINDOW_END();
}

static void lcd_pixel(int16_t x, int16_t y)
{
	if(x < 0 || y < 0 || x >= LCD_WIDTH || y >= LCD_HEIGHT)
	{
		return;
	}

	LCD_WINDOW_BEGIN((uint16_t)x, (uint16_t)y, (uint16_t)x + 1, (uint16_t)y + 1);
	WRITE_8((uint8_t)(_color >> 8));
	WRITE_8((uint8_t)_color);
	LCD_WINDOW_END();
}

static void lcd_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	int16_t dx, dy, sx, sy, e0, e1;
	dx = abs(x1 - x0);
	sx = x0 < x1 ? 1 : -1;
	dy = abs(y1 - y0);
	sy = y0 < y1 ? 1 : -1;
	e0 = (dx > dy ? dx : -dy) / 2;
	while(x0 != x1 && y0 != y1)
	{
		lcd_pixel(x0, y0);
		e1 = e0;
		if(e1 > -dx)
		{
			e0 -= dy;
			x0 += sx;
		}

		if(e1 < dy)
		{
			e0 += dx;
			y0 += sy;
		}
	}
}

static uint8_t lcd_char(uint16_t x, uint16_t y, char c)
{
	uint8_t width = 0;
	if(c >= 32 && c <= 126)
	{
		uint8_t col, row, data;
		c -= 32;
		width = pgm_read_byte(font_default_widths + c);
		for(row = 0; row < FONT_HEIGHT; ++row)
		{
			data = pgm_read_byte(font_default_chars + (c * FONT_HEIGHT + row));
			for(col = 0; col < width; ++col)
			{
				if((data >> col) & 1)
				{
					lcd_pixel(x + col, y + row);
				}
			}
		}
	}

	return width;
}

static uint16_t lcd_string(uint16_t x, uint16_t y, const char *s)
{
	char c;
	for(; (c = *s); ++s)
	{
		x += lcd_char(x, y, c);
	}

	return x;
}

static uint16_t lcd_string_P(uint16_t x, uint16_t y, const char *s)
{
	char c;
	for(; (c = pgm_read_byte(s)); ++s)
	{
		x += lcd_char(x, y, c);
	}

	return x;
}

static uint8_t lcd_width_char(char c)
{
	uint8_t x = 0;
	if(c >= 32 && c <= 126)
	{
		c -= 32;
		x += pgm_read_byte(font_default_widths + c);
	}

	return x;
}

static uint16_t lcd_string_width(const char *s)
{
	uint16_t x = 0;
	char c;
	for(; (c = *s); ++s)
	{
		x += lcd_width_char(c);
	}

	return x;
}

static uint16_t lcd_string_width_P(const char *s)
{
	uint16_t x = 0;
	char c;
	for(; (c = pgm_read_byte(s)); ++s)
	{
		x += lcd_width_char(c);
	}

	return x;
}

static uint16_t lcd_string_width_len(const char *s, uint8_t len)
{
	uint16_t x = 0;
	uint8_t i;
	char c;
	for(i = 0; i < len; ++i)
	{
		c = s[i];
		x += lcd_width_char(c);
	}

	return x;
}

static void lcd_string_ml(uint16_t x, uint16_t y, uint8_t h, const char *s)
{
	uint16_t x0;
	char c;
	for(x0 = x; (c = *s); ++s)
	{
		if(c == '\n')
		{
			y += h;
			x0 = x;
			continue;
		}

		x0 += lcd_char(x0, y, c);
	}
}

#define lcd_vline(x, y, h) lcd_rect(x, y, 1, h)
#define lcd_hline(x, y, w) lcd_rect(x, y, w, 1)
#define lcd_color(color) _color = color

static void lcd_rect_border(int16_t x, int16_t y, int16_t w, int16_t h)
{
	lcd_hline(x, y, w);
	lcd_hline(x, y + h - 1, w);

	lcd_vline(x, y + 1, h - 2);
	lcd_vline(x + w - 1, y + 1, h - 2);
}

static void lcd_rect_border2(int16_t x, int16_t y, int16_t w, int16_t h)
{
	lcd_rect(x, y, w, 2);
	lcd_rect(x, y + h - 2, w, 2);

	lcd_rect(x, y + 2, 2, h - 4);
	lcd_rect(x + w - 2, y + 2, 2, h - 4);
}

static void lcd_image_rgb565(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t scale, const uint8_t *data)
{
	uint8_t hi, lo;
	uint16_t i, j, x0, y0;
	const uint8_t *line;
	LCD_WINDOW_BEGIN(x, y, x + scale * w - 1, y + scale * h - 1);
	for(y0 = y; y0 < y + h; ++y0)
	{
		line = data;
		for(j = 0; j < scale; ++j)
		{
			data = line;
			for(x0 = x; x0 < x + w; ++x0)
			{
				lo = pgm_read_byte(data++);
				hi = pgm_read_byte(data++);
				for(i = 0; i < scale; ++i)
				{
					WRITE_8(hi);
					WRITE_8(lo);
				}
			}
		}
	}
}

