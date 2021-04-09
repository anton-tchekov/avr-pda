static uint8_t connected;
static char _response[1024];
static uint16_t _response_len;
static char _ip[32];

static void network_init(void)
{
	uint8_t i;
	char c;
	cli();
	i = 0;
	for(;;)
	{
		c = uart_rx();
		if(c >= 32)
		{
			_ip[i++] = c;
		}
		else if(c == '\n')
		{
			_ip[i] = '\0';
			i = 0;
			if(connected == 0xFF)
			{
				connected = 1;
				break;
			}

			if(_ip[0] == 'S')
			{
				connected = 0xFF;
			}
			else if(_ip[0] == 'F')
			{
				break;
			}
		}
	}

	sei();
}

void network_request(char *name)
{
	uint16_t i = 0;
	char c;

	if(!name[0])
	{
		goto end;
	}

	uart_flush();
	uart_tx_str(name);
	uart_tx('\n');
	while((c = uart_rx()) != '~')
	{
		_response[i++] = c;
	}

end:
	_response[i] = '\0';
	_response_len = i;
}
