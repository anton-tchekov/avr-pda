static uint8_t cur_key, kbd_mode = 2;

typedef struct KEYBOARD_KEY
{
	uint8_t X, Y, W, Left, Right, Up, Down;
	char Key, KeyShift;

} KeyboardKey;

static const KeyboardKey keys[51] PROGMEM =
{
	/* Row 0 - 14 Keys */
	{ .X =   0, .Y =  1, .W = 16, .Left = 0xFF, .Right =    1, .Up = 0xFF, .Down =   14, .Key = '`',  .KeyShift = '~'  }, /*  0 */
	{ .X =  17, .Y =  1, .W = 15, .Left =    0, .Right =    2, .Up = 0xFF, .Down =   15, .Key = '1',  .KeyShift = '!'  }, /*  1 */
	{ .X =  33, .Y =  1, .W = 15, .Left =    1, .Right =    3, .Up = 0xFF, .Down =   16, .Key = '2',  .KeyShift = '@'  }, /*  2 */
	{ .X =  49, .Y =  1, .W = 15, .Left =    2, .Right =    4, .Up = 0xFF, .Down =   17, .Key = '3',  .KeyShift = '#'  }, /*  3 */
	{ .X =  65, .Y =  1, .W = 15, .Left =    3, .Right =    5, .Up = 0xFF, .Down =   18, .Key = '4',  .KeyShift = '$'  }, /*  4 */
	{ .X =  81, .Y =  1, .W = 15, .Left =    4, .Right =    6, .Up = 0xFF, .Down =   19, .Key = '5',  .KeyShift = '%'  }, /*  5 */
	{ .X =  97, .Y =  1, .W = 15, .Left =    5, .Right =    7, .Up = 0xFF, .Down =   20, .Key = '6',  .KeyShift = '^'  }, /*  6 */
	{ .X = 113, .Y =  1, .W = 15, .Left =    6, .Right =    8, .Up = 0xFF, .Down =   21, .Key = '7',  .KeyShift = '&'  }, /*  7 */
	{ .X = 129, .Y =  1, .W = 15, .Left =    7, .Right =    9, .Up = 0xFF, .Down =   22, .Key = '8',  .KeyShift = '*'  }, /*  8 */
	{ .X = 145, .Y =  1, .W = 15, .Left =    8, .Right =   10, .Up = 0xFF, .Down =   23, .Key = '9',  .KeyShift = '('  }, /*  9 */
	{ .X = 161, .Y =  1, .W = 15, .Left =    9, .Right =   11, .Up = 0xFF, .Down =   24, .Key = '0',  .KeyShift = ')'  }, /* 10 */
	{ .X = 177, .Y =  1, .W = 15, .Left =   10, .Right =   12, .Up = 0xFF, .Down =   25, .Key = '-',  .KeyShift = '_'  }, /* 11 */
	{ .X = 193, .Y =  1, .W = 15, .Left =   11, .Right =   13, .Up = 0xFF, .Down =   26, .Key = '+',  .KeyShift = '='  }, /* 12 */
	{ .X = 209, .Y =  1, .W = 31, .Left =   12, .Right = 0xFF, .Up = 0xFF, .Down =   27, .Key = '\b', .KeyShift = '\b'  }, /* 13 */

	/* Row 1 - 14 Keys */
	{ .X =   0, .Y = 17, .W = 24, .Left = 0xFF, .Right =   15, .Up =    0, .Down = 0xFF, .Key = '\t', .KeyShift = '\t' }, /* 14 */
	{ .X =  25, .Y = 17, .W = 15, .Left =   14, .Right =   16, .Up =    1, .Down =   28, .Key = 'q',  .KeyShift = 'Q'  }, /* 15 */
	{ .X =  41, .Y = 17, .W = 15, .Left =   15, .Right =   17, .Up =    2, .Down =   29, .Key = 'w',  .KeyShift = 'W'  }, /* 16 */
	{ .X =  57, .Y = 17, .W = 15, .Left =   16, .Right =   18, .Up =    3, .Down =   30, .Key = 'e',  .KeyShift = 'E'  }, /* 17 */
	{ .X =  73, .Y = 17, .W = 15, .Left =   17, .Right =   19, .Up =    4, .Down =   31, .Key = 'r',  .KeyShift = 'R'  }, /* 18 */
	{ .X =  89, .Y = 17, .W = 15, .Left =   18, .Right =   20, .Up =    5, .Down =   32, .Key = 't',  .KeyShift = 'T'  }, /* 19 */
	{ .X = 105, .Y = 17, .W = 15, .Left =   19, .Right =   21, .Up =    6, .Down =   33, .Key = 'y',  .KeyShift = 'Y'  }, /* 20 */
	{ .X = 121, .Y = 17, .W = 15, .Left =   20, .Right =   22, .Up =    7, .Down =   34, .Key = 'u',  .KeyShift = 'U'  }, /* 21 */
	{ .X = 137, .Y = 17, .W = 15, .Left =   21, .Right =   23, .Up =    8, .Down =   35, .Key = 'i',  .KeyShift = 'I'  }, /* 22 */
	{ .X = 153, .Y = 17, .W = 15, .Left =   22, .Right =   24, .Up =    9, .Down =   36, .Key = 'o',  .KeyShift = 'O'  }, /* 23 */
	{ .X = 169, .Y = 17, .W = 15, .Left =   23, .Right =   25, .Up =   10, .Down =   37, .Key = 'p',  .KeyShift = 'P'  }, /* 24 */
	{ .X = 185, .Y = 17, .W = 15, .Left =   24, .Right =   26, .Up =   11, .Down =   38, .Key = '{',  .KeyShift = '['  }, /* 25 */
	{ .X = 201, .Y = 17, .W = 15, .Left =   25, .Right =   27, .Up =   12, .Down =   39, .Key = '}',  .KeyShift = ']'  }, /* 26 */
	{ .X = 217, .Y = 17, .W = 23, .Left =   26, .Right = 0xFF, .Up =   13, .Down =   39, .Key = '|',  .KeyShift = '\\' }, /* 27 */

	/* Row 2 - 11 Keys */
	{ .X =  29, .Y = 33, .W = 15, .Left = 0xFF, .Right =   29, .Up =   15, .Down =   40, .Key = 'a',  .KeyShift = 'A'  }, /* 28 */
	{ .X =  45, .Y = 33, .W = 15, .Left =   28, .Right =   30, .Up =   16, .Down =   41, .Key = 's',  .KeyShift = 'S'  }, /* 29 */
	{ .X =  61, .Y = 33, .W = 15, .Left =   29, .Right =   31, .Up =   17, .Down =   42, .Key = 'd',  .KeyShift = 'D'  }, /* 30 */
	{ .X =  77, .Y = 33, .W = 15, .Left =   30, .Right =   32, .Up =   18, .Down =   43, .Key = 'f',  .KeyShift = 'F'  }, /* 31 */
	{ .X =  93, .Y = 33, .W = 15, .Left =   31, .Right =   33, .Up =   19, .Down =   44, .Key = 'g',  .KeyShift = 'G'  }, /* 32 */
	{ .X = 109, .Y = 33, .W = 15, .Left =   32, .Right =   34, .Up =   20, .Down =   45, .Key = 'h',  .KeyShift = 'H'  }, /* 33 */
	{ .X = 125, .Y = 33, .W = 15, .Left =   33, .Right =   35, .Up =   21, .Down =   46, .Key = 'j',  .KeyShift = 'J'  }, /* 34 */
	{ .X = 141, .Y = 33, .W = 15, .Left =   34, .Right =   36, .Up =   22, .Down =   47, .Key = 'k',  .KeyShift = 'K'  }, /* 35 */
	{ .X = 157, .Y = 33, .W = 15, .Left =   35, .Right =   37, .Up =   23, .Down =   48, .Key = 'l',  .KeyShift = 'L'  }, /* 36 */
	{ .X = 173, .Y = 33, .W = 15, .Left =   36, .Right =   38, .Up =   24, .Down =   49, .Key = ':',  .KeyShift = ';'  }, /* 37 */
	{ .X = 189, .Y = 33, .W = 15, .Left =   37, .Right =   39, .Up =   25, .Down =   49, .Key = '"',  .KeyShift = '\'' }, /* 38 */
	{ .X = 205, .Y = 33, .W = 35, .Left =   38, .Right = 0xFF, .Up =   26, .Down =   49, .Key = '\n', .KeyShift = '\n' }, /* 39 */

	/* Row 3 - 10 Keys */
	{ .X =  33, .Y = 49, .W = 15, .Left = 0xFF, .Right =   41, .Up =   28, .Down =   50, .Key = 'z', .KeyShift = 'Z'  }, /* 40 */
	{ .X =  49, .Y = 49, .W = 15, .Left =   40, .Right =   42, .Up =   29, .Down =   50, .Key = 'x', .KeyShift = 'X'  }, /* 41 */
	{ .X =  65, .Y = 49, .W = 15, .Left =   41, .Right =   43, .Up =   30, .Down =   50, .Key = 'c', .KeyShift = 'C'  }, /* 42 */
	{ .X =  81, .Y = 49, .W = 15, .Left =   42, .Right =   44, .Up =   31, .Down =   50, .Key = 'v', .KeyShift = 'V'  }, /* 43 */
	{ .X =  97, .Y = 49, .W = 15, .Left =   43, .Right =   45, .Up =   32, .Down =   50, .Key = 'b', .KeyShift = 'B'  }, /* 44 */
	{ .X = 113, .Y = 49, .W = 15, .Left =   44, .Right =   46, .Up =   33, .Down =   50, .Key = 'n', .KeyShift = 'N'  }, /* 45 */
	{ .X = 129, .Y = 49, .W = 15, .Left =   45, .Right =   47, .Up =   34, .Down =   50, .Key = 'm', .KeyShift = 'M'  }, /* 46 */
	{ .X = 145, .Y = 49, .W = 15, .Left =   46, .Right =   48, .Up =   35, .Down =   50, .Key = '<', .KeyShift = ','  }, /* 47 */
	{ .X = 161, .Y = 49, .W = 15, .Left =   47, .Right =   49, .Up =   36, .Down =   50, .Key = '>', .KeyShift = '.'  }, /* 48 */
	{ .X = 177, .Y = 49, .W = 15, .Left =   48, .Right = 0xFF, .Up =   37, .Down =   50, .Key = '?', .KeyShift = '/'  }, /* 49 */

	/* Row 4 - 1 Key */
	{ .X =  65, .Y = 65, .W = 95, .Left = 0xFF, .Right = 0xFF, .Up =   44, .Down = 0xFF, .Key = ' ', .KeyShift = ' '  }, /* 50 */
};

static void event_key(uint8_t key, uint8_t ascii);

static uint8_t key_shift(uint8_t k)
{
	return kbd_mode ? pgm_read_byte(&(keys[k].KeyShift)) : pgm_read_byte(&(keys[k].Key));
}

static void render_key(uint8_t k)
{
	uint8_t x, y, w;
	x = pgm_read_byte(&(keys[k].X));
	y = pgm_read_byte(&(keys[k].Y));
	w = pgm_read_byte(&(keys[k].W));

	lcd_color(LCD_WHITE);
	lcd_rect(x, LCD_HEIGHT - 80 + y, w, 15);
	lcd_color(LCD_BLACK);
	lcd_char(x + w / 2 - pgm_read_byte(&font_default_widths[key_shift(k) - 32]) / 2,
			LCD_HEIGHT - 80 + y + 2, key_shift(k));
}

static void render_key_sel(uint8_t k)
{
	uint8_t x, y, w;
	x = pgm_read_byte(&(keys[k].X));
	y = pgm_read_byte(&(keys[k].Y));
	w = pgm_read_byte(&(keys[k].W));

	lcd_color(LCD_BLUE);
	lcd_rect(x, LCD_HEIGHT - 80 + y, w, 15);
	lcd_color(LCD_WHITE);
	lcd_char(x + w / 2 - pgm_read_byte(&font_default_widths[key_shift(k) - 32]) / 2,
			LCD_HEIGHT - 80 + y + 2, key_shift(k));
}

static void render_key_move(uint8_t old, uint8_t new)
{
	render_key(old);
	render_key_sel(new);
}

static void render_grid(void)
{
	uint8_t i;
	lcd_color(LCD_BLACK);
	lcd_hline(0, LCD_HEIGHT - 80, LCD_WIDTH);
	lcd_hline(0, LCD_HEIGHT - 80 + 16, LCD_WIDTH);
	lcd_hline(0, LCD_HEIGHT - 80 + 32, LCD_WIDTH);
	lcd_hline(0, LCD_HEIGHT - 80 + 48, LCD_WIDTH);
	lcd_hline(0, LCD_HEIGHT - 80 + 64, LCD_WIDTH);

	/* Middle Bottom Row */
	lcd_rect(0, LCD_HEIGHT - 80 + 33, 29, 15);

	/* Second Bottom Row */
	lcd_rect(0, LCD_HEIGHT - 80 + 49, 33, 15);
	lcd_rect(177 + 16, LCD_HEIGHT - 80 + 49, LCD_WIDTH - (177 + 16), 15);

	/* Enter Sides */
	lcd_rect(0, LCD_HEIGHT - 80 + 65, 65, 15);
	lcd_rect(65 + 96, LCD_HEIGHT - 80 + 65, LCD_WIDTH - (65 + 96), 15);

	/* Vertical */
	for(i = 0; i < 51; ++i)
	{
		uint8_t x, y, w;
		x = pgm_read_byte(&(keys[i].X));
		y = pgm_read_byte(&(keys[i].Y));
		w = pgm_read_byte(&(keys[i].W));
		if(x + w < LCD_WIDTH - 1)
		{
			lcd_vline(x + w, LCD_HEIGHT - 80 + y, 15);
		}
	}
}

static void render_keys(void)
{
	uint8_t i;
	for(i = 0; i < 51; ++i)
	{
		if(i == cur_key)
		{
			render_key_sel(i);
			continue;
		}

		render_key(i);
	}
}

static void render_key_feedback(void)
{
	lcd_color(LCD_WHITE);
	lcd_rect(0, LCD_HEIGHT - 80, LCD_WIDTH, 80);

	lcd_color(LCD_BLACK);
	lcd_hline(0, LCD_HEIGHT - 80, LCD_WIDTH);

	/* A */
	lcd_rect_border(5, LCD_HEIGHT - 80 + 5, 20, 20);

	/* B */
	lcd_rect_border(5, LCD_HEIGHT - 80 + 30, 20, 20);

	/* Center */
	lcd_rect_border(110, LCD_HEIGHT - 80 + 30, 20, 20);

	/* Right */
	lcd_rect_border(135, LCD_HEIGHT - 80 + 30, 20, 20);

	/* Left */
	lcd_rect_border(85, LCD_HEIGHT - 80 + 30, 20, 20);

	/* Top */
	lcd_rect_border(110, LCD_HEIGHT - 80 + 5, 20, 20);

	/* Bottom */
	lcd_rect_border(110, LCD_HEIGHT - 80 + 55, 20, 20);
}

static void render_keyboard(void)
{
	switch(kbd_mode)
	{
		case 0:
			render_grid();
			render_keys();
			break;

		case 1:
			render_keys();
			break;

		case 2:
			render_key_feedback();
			break;
	}
}

static void keyboard_key(uint8_t button, uint8_t action)
{
	if(button == BUTTON_A && action == BUTTON_PRESS)
	{
		++kbd_mode;
		if(kbd_mode == 3)
		{
			kbd_mode = 0;
		}

		render_keyboard();
	}
	else
	{
		if(kbd_mode != 2)
		{
			if(action == BUTTON_PRESS || action == BUTTON_REPEAT)
			{
				switch(button)
				{
					case BUTTON_UP:
					{
						uint8_t new;
						new = pgm_read_byte(&(keys[cur_key].Up));
						if(new == 0xFF) { return; }
						render_key_move(cur_key, new);
						cur_key = new;
						break;
					}

					case BUTTON_DOWN:
					{
						uint8_t new;
						new = pgm_read_byte(&(keys[cur_key].Down));
						if(new == 0xFF) { return; }
						render_key_move(cur_key, new);
						cur_key = new;
						break;
					}

					case BUTTON_LEFT:
					{
						uint8_t new;
						new = pgm_read_byte(&(keys[cur_key].Left));
						if(new == 0xFF) { return; }
						render_key_move(cur_key, new);
						cur_key = new;
						break;
					}

					case BUTTON_RIGHT:
					{
						uint8_t new;
						new = pgm_read_byte(&(keys[cur_key].Right));
						if(new == 0xFF) { return; }
						render_key_move(cur_key, new);
						cur_key = new;
						break;
					}

					case BUTTON_CENTER:
					{
						event_key(cur_key, key_shift(cur_key));
						break;
					}
				}
			}
		}
		else
		{
			switch(action)
			{
				case BUTTON_PRESS:
					lcd_color(LCD_BLUE);
					break;

				case BUTTON_RELEASE:
					lcd_color(LCD_WHITE);
					break;
			}

			switch(button)
			{
				case BUTTON_A:
					lcd_rect(5 + 1, LCD_HEIGHT - 80 + 5 + 1, 18, 18);
					break;

				case BUTTON_B:
					lcd_rect(5 + 1, LCD_HEIGHT - 80 + 30 + 1, 18, 18);
					break;

				case BUTTON_CENTER:
					lcd_rect(110 + 1, LCD_HEIGHT - 80 + 30 + 1, 18, 18);
					break;

				case BUTTON_RIGHT:
					lcd_rect(135 + 1, LCD_HEIGHT - 80 + 30 + 1, 18, 18);
					break;

				case BUTTON_LEFT:
					lcd_rect(85 + 1, LCD_HEIGHT - 80 + 30 + 1, 18, 18);
					break;

				case BUTTON_UP:
					lcd_rect(110 + 1, LCD_HEIGHT - 80 + 5 + 1, 18, 18);
					break;

				case BUTTON_DOWN:
					lcd_rect(110 + 1, LCD_HEIGHT - 80 + 55 + 1, 18, 18);
					break;
			}
		}
	}
}
