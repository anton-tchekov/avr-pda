static void form_message_button_send_click(void);
static void form_message_key(uint8_t button, uint8_t action);

static Element message_elements[] =
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

	/* Send */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 155,
			.Y = 30,
			.W = 80,
			.H = 20,
			.Text = { .Flash = send },
			.Click = form_message_button_send_click
		}}
	},
};

static Form message_form =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = send_message },
	.Elements = message_elements,
	.Count = arrlen(message_elements),
	.OnKey = form_message_key
};

static void message_open(void)
{
	input_clear(&request_elements[0].E.I);
	form_open(&message_form);
}

static void form_message_button_send_click(void)
{
	network_request(input_message);
	apps_open();
}

static void form_message_key(uint8_t button, uint8_t action)
{
	if(button == BUTTON_B)
	{
		apps_open();
	}
}
