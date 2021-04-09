static void form_request_button_send_click(void);
static void form_request_key(uint8_t button, uint8_t action);

static Element request_elements[] =
{
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 5,
			.Y = 5,
			.W = 230,
			.Position = 0,
			.Length = 0,
			.Size = sizeof(input_message) - 4,
			.Text = input_message + 4
		}}
	},

	/* Request */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 155,
			.Y = 30,
			.W = 80,
			.H = 20,
			.Text = { .Flash = request },
			.Click = form_request_button_send_click
		}}
	},
};

static Form request_form =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = request },
	.Elements = request_elements,
	.Count = arrlen(request_elements),
	.OnKey = form_request_key
};

static void form_request_button_send_click(void)
{
	network_request(input_message + 4);
	lcd_color(LCD_WHITE);
	lcd_rect(0, 75, 240, 165);
	lcd_color(LCD_BLACK);
	lcd_string_ml(5, 75, 12, _response);
}

static void form_request_key(uint8_t button, uint8_t action)
{
	if(button == BUTTON_B)
	{
		apps_open();
	}
}

static void request_open(void)
{
	input_clear(&request_elements[0].E.I);
	form_open(&request_form);
}

