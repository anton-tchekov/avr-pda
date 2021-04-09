static uint16_t count_up = 0xFFFF;

static void timer_update(void);
static void form_clock_button_get_click(void);
static void form_clock_button_toggle_click(void);
static void form_clock_key(uint8_t button, uint8_t action);

static Element clock_elements[] =
{
	/* Get Time and Date Button */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 5,
			.W = 230,
			.H = 20,
			.Text = { .Flash = get_time_and_date },
			.Click = form_clock_button_get_click
		}}
	},

	/* Start/Stop Timer Button */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 55,
			.W = 230,
			.H = 20,
			.Text = { .Flash = start_timer },
			.Click = form_clock_button_toggle_click
		}}
	}
};

static Form clock_form =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = clock },
	.Elements = clock_elements,
	.Count = arrlen(clock_elements),
	.OnKey = form_clock_key
};

static void form_clock_button_get_click(void)
{
	network_request("datetime");
	lcd_color(LCD_WHITE);
	lcd_rect(0, 50, 240, 20);
	lcd_color(LCD_BLACK);
	lcd_string(5, 50, _response);
}

static void form_clock_button_toggle_click(void)
{
	if(clock_elements[1].E.B.Text.Flash == start_timer)
	{
		count_up = 0;
		timer_update();
		clock_elements[1].E.B.Text.Flash = stop_timer;
	}
	else
	{
		count_up = 0xFFFF;
		clock_elements[1].E.B.Text.Flash = start_timer;
	}

	button_render(&clock_elements[1].E.B, 1);
}

static void clock_update(void)
{
	if(count_up >= 60000)
	{
		return;
	}

	++count_up;
	timer_update();
}

static void timer_update(void)
{
	uint8_t i;

	i = '0' + ((count_up / 10000) % 10);
	if(i != _response[0])
	{
		lcd_color(LCD_WHITE);
		lcd_char(5, 100, _response[0]);
		_response[0] = i;
		lcd_color(LCD_BLACK);
		lcd_char(5, 100, _response[0]);
	}

	i = '0' + ((count_up / 1000) % 10);
	if(i != _response[1])
	{
		lcd_color(LCD_WHITE);
		lcd_char(15, 100, _response[1]);
		_response[1] = i;
		lcd_color(LCD_BLACK);
		lcd_char(15, 100, _response[1]);
	}

	i = '0' + ((count_up / 100) % 10);
	if(i != _response[2])
	{
		lcd_color(LCD_WHITE);
		lcd_char(25, 100, _response[2]);
		_response[2] = i;
		lcd_color(LCD_BLACK);
		lcd_char(25, 100, _response[2]);
	}

	i = '0' + ((count_up / 10) % 10);
	if(i != _response[3])
	{
		lcd_color(LCD_WHITE);
		lcd_char(35, 100, _response[3]);
		_response[3] = i;
		lcd_color(LCD_BLACK);
		lcd_char(35, 100, _response[3]);
	}

	i = '0' + (count_up % 10);
	if(i != _response[4])
	{
		lcd_color(LCD_WHITE);
		lcd_char(45, 100, _response[4]);
		_response[4] = i;
		lcd_color(LCD_BLACK);
		lcd_char(45, 100, _response[4]);
	}
}

static void form_clock_key(uint8_t button, uint8_t action)
{
	if(button == BUTTON_B)
	{
		count_up = 0xFFFF;
		clock_elements[1].E.B.Text.Flash = start_timer;
		apps_open();
	}
}

static void clock_open(void)
{
	form_open(&clock_form);
}

