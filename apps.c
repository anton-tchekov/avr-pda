static void table_open(void);
static void plot_open(void);
static void request_open(void);
static void message_open(void);
static void clock_open(void);

static Element apps_elements[] =
{
	/* Calculator */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 5,
			.W = 230,
			.H = 20,
			.Text = { .Flash = calculator },
			.Click = table_open
		}}
	},

	/* Function Plotter */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 30,
			.W = 230,
			.H = 20,
			.Text = { .Flash = function_plotter },
			.Click = plot_open
		}}
	},

	/* Clock */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 55,
			.W = 230,
			.H = 20,
			.Text = { .Flash = clock },
			.Click = clock_open
		}}
	},

	/* Request Resource */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 80,
			.W = 230,
			.H = 20,
			.Text = { .Flash = request_resource },
			.Click = request_open
		}}
	},

	/* Send Message */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 105,
			.W = 230,
			.H = 20,
			.Text = { .Flash = send_message },
			.Click = message_open
		}}
	},

	/* About Label */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 205,
			.Text = { .Flash = about }
		}}
	},
};

static Form form_apps =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = applications },
	.Elements = apps_elements,
	.Count = arrlen(apps_elements),
	.OnKey = NULL
};

static void apps_open(void)
{
	if(!connected)
	{
		form_apps.Count = 2;
	}

	form_open(&form_apps);
}
