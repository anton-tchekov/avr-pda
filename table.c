static void form_table_key(uint8_t button, uint8_t action);
static void form_table_input_button_ok_click(void);
static void form_table_input_button_prev_click(void);
static void form_table_input_button_next_click(void);

static char input_x_buffer[16] = "0.0";
static char input_table_step_buffer[16] = "0.5";

static float tstep, tx, ty;

static Element table_elements[] =
{
	/* Expression */
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 5,
			.Y = 5,
			.W = 185,
			.Position = 0,
			.Length = 0,
			.Size = sizeof(input_fx_buffer),
			.Text = input_fx_buffer
		}}
	},
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 195,
			.Y = 5,
			.W = 40,
			.H = 20,
			.Text = { .Flash = ok },
			.Click = form_table_input_button_ok_click
		}}
	},

	/* X */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 35,
			.Text = { .Flash = xeq }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 45,
			.Y = 30,
			.W = 190,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_x_buffer),
			.Text = input_x_buffer
		}}
	},

	/* Step */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 60,
			.Text = { .Flash = step }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 45,
			.Y = 55,
			.W = 190,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_table_step_buffer),
			.Text = input_table_step_buffer
		}}
	},

	/* Prev */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 80,
			.W = 110,
			.H = 20,
			.Text = { .Flash = prev },
			.Click = form_table_input_button_prev_click
		}}
	},

	/* Next */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 125,
			.Y = 80,
			.W = 110,
			.H = 20,
			.Text = { .Flash = next },
			.Click = form_table_input_button_next_click
		}}
	},

	/* Result */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 110,
			.Text = { .Flash = xeq + 2 }
		}}
	}
};

static Form table_form =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = calculator },
	.Elements = table_elements,
	.Count = arrlen(table_elements),
	.OnKey = form_table_key
};

static void form_table_key(uint8_t button, uint8_t action)
{
	if(button == BUTTON_B)
	{
		apps_open();
	}
}

static void print_error(uint8_t code)
{
	const char *s = error_syntax;
	switch(code)
	{
		case ERROR_MATH:
			s = error_math;
			break;

		case ERROR_NOMEM:
			s = error_nomem;
			break;

		case ERROR_RANGE:
			s = error_range;
			break;
	}

	lcd_color(LCD_WHITE);
	lcd_rect(50, 130, 120, 11);
	lcd_color(LCD_BLACK);
	lcd_string_P(50, 130, s);
}

static void print_result(void)
{
	char s[16];
	dtostrf(ty, -15, 6, s);
	lcd_color(LCD_WHITE);
	lcd_rect(50, 130, 120, 11);
	lcd_color(LCD_BLACK);
	lcd_string(50, 130, s);
}

static void form_table_input_button_ok_click(void)
{
	uint8_t e;
	tx = atof(input_x_buffer);
	if((e = calc_prepare(input_fx_buffer)))
	{
		print_error(e);
		return;
	}

	if((e = calc_solve(tx, &ty)))
	{
		print_error(e);
		return;
	}

	print_result();
}

static uint8_t flen(char *s)
{
	uint8_t i;
	char c;
	for(i = 0; (c = *s); ++s, ++i)
	{
		if(!((c >= '0' && c <= '9') || c == '.'))
		{
			break;
		}
	}

	s[i] = '\0';
	return i;
}

static void update_x(float tx)
{
	uint8_t len;
	dtostrf(tx, -15, 6, table_elements[3].E.I.Text);
	len = flen(table_elements[3].E.I.Text);
	table_elements[3].E.I.Position = len;
	table_elements[3].E.I.Length = len;
	input_render(&table_elements[3].E.I, 0);
}

static void form_table_input_button_prev_click(void)
{
	uint8_t e;
	tstep = atof(input_table_step_buffer);
	tx = atof(input_x_buffer);
	tx -= tstep;
	update_x(tx);
	if((e = calc_solve(tx, &ty)))
	{
		print_error(e);
		return;
	}

	print_result();
}

static void form_table_input_button_next_click(void)
{
	uint8_t e;
	tstep = atof(input_table_step_buffer);
	tx = atof(input_x_buffer);
	tx += tstep;
	update_x(tx);
	if((e = calc_solve(tx, &ty)))
	{
		print_error(e);
		return;
	}

	print_result();
}

static void table_open(void)
{
	form_open(&table_form);
	lcd_color(LCD_BLACK);
	lcd_rect_border(45, 125, 190, 20);
}
