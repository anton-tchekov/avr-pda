static volatile uint16_t ms;

static void tick(void);
static void wait_ms(uint16_t n);
static void mstimer_init(void);

static void mstimer_init(void)
{
	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS21) | (1 << CS20);
	OCR2A = 250;
	TIMSK2 = (1 << OCIE2A);
}

static void mstimer_disable(void)
{
	TCCR2B &= ~((1 << CS21) | (1 << CS20));
	cli();
}

static void mstimer_enable(void)
{
	TCCR2B |= (1 << CS21) | (1 << CS20);
	sei();
}

void wait_ms(uint16_t n)
{
	uint16_t target = ms + n;
	while(ms != target) ;
}

ISR(TIMER2_COMPA_vect)
{
	++ms;
	tick();
}
