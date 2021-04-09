#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#define arrlen(A) (sizeof(A) / sizeof(*A))

/* Hardware Layer */
#include "buttons.c"
#include "mstimer.c"
#include "graphics.c"
#include "uart.c"

/* Library Layer */
#include "calc.c"

/* Middle Layer */
#include "form.c"
#include "network.c"
#include "keyboard.c"

/* Data */
#include "strings.c"

/* Application Layer */
#include "init.c"
#include "apps.c"
#include "plot.c"
#include "table.c"
#include "request.c"
#include "message.c"
#include "clock.c"

static void event_key(uint8_t key, uint8_t ascii);
static void event_button(uint8_t button, uint8_t action);

int main(void)
{
	form_init_open();
	render_keyboard();
	apps_open();
	for(;;)
	{

	}

	return 0;
}

static void tick(void)
{
	mstimer_disable();
	clock_update();
	buttons_update();
	mstimer_enable();
}

static void event_button(uint8_t button, uint8_t action)
{
	if(any_key)
	{
		init_key();
		return;
	}

	if(kbd_mode == 2)
	{
		form_event_button(button, action);
	}

	keyboard_key(button, action);
}

static void event_key(uint8_t key, uint8_t ascii)
{
	form_event_key(key, ascii);
}
