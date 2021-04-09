static uint8_t any_key = 1;

static void form_init_open(void)
{
	uint16_t y;
	buttons_init();
	mstimer_init();
	sei();
	lcd_init();
	lcd_color(LCD_WHITE);
	lcd_clear();

	lcd_color(LCD_BLUE);
	lcd_rect(0, 0, LCD_WIDTH, 20);

	lcd_color(LCD_WHITE);
	lcd_string(5, 5, "Initializing ...");

	lcd_color(LCD_BLACK);

	/* Hardware */
	y = 25;
	lcd_string(5, y, "[ HW ] Initialized");
	y += 20;

	/* Press A to disable */
	if(PINC & (1 << 0))
	{
		/* UART */
		uart_init();
		lcd_string(5, y, "[ UART ] Initialized");
		y += 20;

		lcd_string(5, y, "[ NET ] Waiting for Connection");
		y += 20;
		network_init();

		if(connected)
		{
			uint16_t x0 = lcd_string(5, 85, "[ NET ] IP Address: ");
			lcd_string(x0, y, _ip);
		}
		else
		{
			lcd_string(5, y, "[ NET ] Timed out");
		}

		y += 20;
	}
	else
	{
		lcd_string(5, y, "[ NET ] Disabled");
		y += 20;
	}

	while(any_key)
	{
		lcd_color(LCD_BLACK);
		lcd_string(5, y, "[ OS ] Press any key to continue ...");
		wait_ms(500);

		lcd_color(LCD_WHITE);
		lcd_rect(0, y - 5, LCD_WIDTH, 20);
		wait_ms(500);
	}
}

static void init_key(void)
{
	any_key = 0;
}