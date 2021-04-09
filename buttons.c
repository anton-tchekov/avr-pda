enum HW_BUTTON
{
	BUTTON_LEFT,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_CENTER,
	BUTTON_A,
	BUTTON_B
};

#define BUTTON_PRESS          0
#define BUTTON_RELEASE        1
#define BUTTON_REPEAT         2
#define BUTTON_PRESS_TICKS  150
#define BUTTON_REPEAT_TICKS 800

static const volatile uint8_t *button_ports[7] = { &PIND, &PIND, &PIND, &PIND, &PIND, &PINC, &PIND };
static const uint8_t button_pins[7] = { (1 << 2), (1 << 4), (1 << 3), (1 << 6), (1 << 5), (1 << 0), (1 << 7) };
static uint16_t button_ticks[7];

#ifdef DEBUG

static const char *strings0[7] = { "Up", "Down", "Left", "Right", "Center", "A", "B" };
static const char *strings1[3] = { "Press", "Release", "Repeat" };

#endif

static void event_button(uint8_t button, uint8_t action);

static void buttons_init(void)
{
	PORTD = (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
	PORTC |= (1 << 0);
}

static void buttons_update(void)
{
	uint8_t i;
	for(i = 0; i < 7; ++i)
	{
		if(!(*button_ports[i] & button_pins[i]))
		{
			if(button_ticks[i] < BUTTON_PRESS_TICKS)
			{
				++button_ticks[i];
			}
			else if(button_ticks[i] == BUTTON_PRESS_TICKS)
			{
				button_ticks[i] = BUTTON_PRESS_TICKS + 1;
				event_button(i, BUTTON_PRESS);
			}
			else if(button_ticks[i] < BUTTON_PRESS_TICKS + BUTTON_REPEAT_TICKS)
			{
				++button_ticks[i];
			}
			else
			{
				button_ticks[i] = BUTTON_PRESS_TICKS + 1;
				event_button(i, BUTTON_REPEAT);
			}
		}
		else if(button_ticks[i] >= BUTTON_PRESS_TICKS)
		{
			button_ticks[i] = 0;
			event_button(i, BUTTON_RELEASE);
		}
		else
		{
			button_ticks[i] = 0;
		}
	}
}

