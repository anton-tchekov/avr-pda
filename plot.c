static char input_fx_buffer[32] = "x*x";
static char input_gx_buffer[32] = "1/x";
static char input_step_buffer[16] = "0.001";
static char input_center_x_buffer[16] = "0.0";
static char input_center_y_buffer[16] = "0.0";
static char input_scale_x_buffer[16] = "20.0";
static char input_scale_y_buffer[16] = "20.0";
static char input_grid_x_buffer[16] = "1.0";
static char input_grid_y_buffer[16] = "1.0";

/* CenterX, CenterX, ScaleX, ScaleY, GridX, GridY, Step */
static float pcx, pcy, psx, psy, pgx, pgy, pstep;

static void form_plot_input_button_ok_click(void);
static void form_plot_input_key(uint8_t button, uint8_t action);
static void form_plot_output_key(uint8_t button, uint8_t action);

static Element calc_input_elements[] =
{
	/* F(X) = */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 5,
			.Text = { .Flash = fx }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 5,
			.Y = 20,
			.W = 230,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_fx_buffer),
			.Text = input_fx_buffer
		}}
	},

	/* G(X) = */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 45,
			.Text = { .Flash = gx }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 5,
			.Y = 60,
			.W = 230,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_gx_buffer),
			.Text = input_gx_buffer
		}}
	},

	/* Center X */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 85,
			.Text = { .Flash = center_x }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 5,
			.Y = 100,
			.W = 50,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_center_x_buffer),
			.Text = input_center_x_buffer
		}}
	},

	/* Center Y */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 65,
			.Y = 85,
			.Text = { .Flash = center_y }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 65,
			.Y = 100,
			.W = 50,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_center_y_buffer),
			.Text = input_center_y_buffer
		}}
	},

	/* Scale X */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 125,
			.Y = 85,
			.Text = { .Flash = scale_x }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 125,
			.Y = 100,
			.W = 50,
			.Position = 4,
			.Length = 4,
			.Size = sizeof(input_scale_x_buffer),
			.Text = input_scale_x_buffer
		}}
	},

	/* Scale Y */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 185,
			.Y = 85,
			.Text = { .Flash = scale_y }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 185,
			.Y = 100,
			.W = 50,
			.Position = 4,
			.Length = 4,
			.Size = sizeof(input_scale_y_buffer),
			.Text = input_scale_y_buffer
		}}
	},


	/* Grid X */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 5,
			.Y = 125,
			.Text = { .Flash = grid_x }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 5,
			.Y = 140,
			.W = 50,
			.Position = 3,
			.Length = 3,
			.Size = 32,
			.Text = input_grid_x_buffer
		}}
	},

	/* Grid Y */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 65,
			.Y = 125,
			.Text = { .Flash = grid_y }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 65,
			.Y = 140,
			.W = 50,
			.Position = 3,
			.Length = 3,
			.Size = sizeof(input_grid_y_buffer),
			.Text = input_grid_y_buffer
		}}
	},

	/* Step */
	{
		.Type = ELEMENT_TYPE_LABEL,
		.E = { .L =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 125,
			.Y = 125,
			.Text = { .Flash = step }
		}}
	},
	{
		.Type = ELEMENT_TYPE_INPUT,
		.E = { .I =
		{
			.Flags = 0,
			.X = 125,
			.Y = 140,
			.W = 110,
			.Position = 5,
			.Length = 5,
			.Size = sizeof(input_step_buffer),
			.Text = input_step_buffer
		}}
	},

	/* OK */
	{
		.Type = ELEMENT_TYPE_BUTTON,
		.E = { .B =
		{
			.Flags = FLAG_TEXT_PROGMEM,
			.X = 155,
			.Y = 195,
			.W = 80,
			.H = 20,
			.Text = { .Flash = ok },
			.Click = form_plot_input_button_ok_click
		}}
	}
};

static Form plot_form_input =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = function_plotter },
	.Elements = calc_input_elements,
	.Count = arrlen(calc_input_elements),
	.OnKey = form_plot_input_key
};

static Form plot_form_output =
{
	.Flags = FLAG_TEXT_PROGMEM,
	.Title = { .Flash = function_plotter },
	.Elements = NULL,
	.Count = 0,
	.OnKey = form_plot_output_key
};

static float map(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void plot_open(void)
{
	form_open(&plot_form_input);
}

static void plot_render(void)
{
	/* MinX, MaxY */
	float min_x, max_x, min_y, max_y;

	/* X/Y */
	float x, y;

	/* Mapped X/Y */
	int16_t mx, my;

	/* Clear */
	lcd_color(LCD_WHITE);
	lcd_rect(0, 20, 240, 220);

	/* Calculate MinX/MaxX */
	{
		float f = 120.0f / psx;
		min_x = pcx - f;
		max_x = pcx + f;
	}

	/* Calculate MinY/MaxY */
	{
		float f = 110.0f / psy;
		min_y = pcy - f;
		max_y = pcy + f;
	}

	/* Draw Grid */
	lcd_color(0xAD55);

	/* X: Vertical */
	{
		float f;
		f = ceil(min_x / pgx) * pgx;
		for(; f < max_x; f += pgx)
		{
			mx = (int16_t)map(f, min_x, max_x, 0.0f, 240.0f);
			lcd_vline(mx, 20, 220);
		}
	}

	/* Y: Horizontal */
	{
		float f;
		f = ceil(min_y / pgy) * pgy;
		for(; f < max_y; f += pgy)
		{
			my = (int16_t)map(f, min_y, max_y, 240.0f, 20.0f);
			lcd_hline(0, my, 240);
		}
	}

	/* Draw X/Y Axes */
	lcd_color(LCD_BLACK);
	mx = (int16_t)map(0.0f, min_x, max_x, 0.0f, 240.0f);
	if(mx < 0 || mx >= 240)
	{
		goto dy;
	}

	lcd_vline(mx, 20, 220);

dy:
	my = (int16_t)map(0.0f, min_y, max_y, 240.0f, 20.0f);
	if(my < 20 || my >= 240)
	{
		goto dt;
	}

	lcd_hline(0, my, 240);

dt:
	/* Draw Ticks and Numbers */


fx:
	/* Plot f(x) */
	if(calc_prepare(input_fx_buffer))
	{
		goto gx;
	}

	lcd_color(LCD_RED);
	for(x = min_x; x < max_x; x += pstep)
	{
		if(calc_solve(x, &y))
		{
			continue;
		}

		mx = (int16_t)map(x, min_x, max_x, 0.0f, 240.0f);
		if(mx < 0 || mx >= 240)
		{
			continue;
		}

		my = (int16_t)map(y, min_y, max_y, 240.0f, 20.0f);
		if(my < 20 || my >= 240)
		{
			continue;
		}

		lcd_pixel(mx, my);
	}

gx:
	/* Plot g(x) */
	if(calc_prepare(input_gx_buffer))
	{
		return;
	}

	lcd_color(LCD_GREEN);
	for(x = min_x; x < max_x; x += pstep)
	{
		if(calc_solve(x, &y))
		{
			continue;
		}

		mx = (int16_t)map(x, min_x, max_x, 0.0f, 240.0f);
		if(mx < 0 || mx >= 240)
		{
			continue;
		}

		my = (int16_t)map(y, min_y, max_y, 240.0f, 20.0f);
		if(my < 20 || my >= 240)
		{
			continue;
		}

		lcd_pixel(mx, my);
	}
}

static void form_plot_input_button_ok_click(void)
{
	form_open(&plot_form_output);

	/* Get values */
	pcx = atof(input_center_x_buffer);
	pcy = atof(input_center_y_buffer);
	psx = atof(input_scale_x_buffer);
	psy = atof(input_scale_y_buffer);
	pgx = atof(input_grid_x_buffer);
	pgy = atof(input_grid_y_buffer);
	pstep = atof(input_step_buffer);

	if(psx <= 0.0f)
	{
		psx = 20.0f;
	}

	if(psy <= 0.0f)
	{
		psy = 20.0f;
	}

	if(pgx <= 0.0f)
	{
		pgx = 1.0f;
	}

	if(pgy <= 0.0f)
	{
		pgy = 1.0f;
	}

	if(pstep < 0.00001f)
	{
		pstep = 0.001f;
	}

	plot_render();
}

static void form_plot_input_key(uint8_t button, uint8_t action)
{
	if(button == BUTTON_B && action == BUTTON_PRESS)
	{
		apps_open();
		return;
	}
}

static void form_plot_output_key(uint8_t button, uint8_t action)
{
	if(action == BUTTON_PRESS)
	{
		return;
	}

	if(button == BUTTON_B)
	{
		form_open(&plot_form_input);
		return;
	}

	switch(button)
	{
		case BUTTON_UP:
			pcy += 20.0f / psy;
			plot_render();
			break;

		case BUTTON_DOWN:
			pcy -= 20.0f / psy;
			plot_render();
			break;

		case BUTTON_LEFT:
			pcx -= 20.0f / psx;
			plot_render();
			break;

		case BUTTON_RIGHT:
			pcx += 20.0f / psx;
			plot_render();
			break;
	}
}

