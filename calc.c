#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

enum CHAR
{
	CHAR_X = 'x',
	CHAR_DP = '.',
	CHAR_LP = '(',
	CHAR_RP = ')',
	CHAR_ADD = '+',
	CHAR_SUB = '-',
	CHAR_MUL = '*',
	CHAR_DIV = '/',
	CHAR_POW = '^',
};

enum PGM_STRING
{
	ERROR_NOERROR,
	ERROR_SYNTAX,
	ERROR_MATH,
	ERROR_NOMEM,
	ERROR_RANGE,
	STR_SIN,
	STR_COS,
	STR_TAN,
	STR_ASIN,
	STR_ACOS,
	STR_ATAN,
	STR_START,
	STR_STEP,
	STR_ERROR,
};

enum TOKEN_TYPE
{
	/* Infix */
	TT_NULL,
	TT_NUMBER,
	TT_X,
	TT_LP,
	TT_RP,

	/* Postfix */
	/* Unary */
	TT_UNARY_MINUS,
	TT_LOG,
	TT_SIN,
	TT_COS,
	TT_TAN,
	TT_ASIN,
	TT_ACOS,
	TT_ATAN,

	/* Binary */
	TT_ADD,
	TT_SUB,
	TT_MUL,
	TT_DIV,
	TT_POW,
};

#define NUMBER_STACK_SIZE      32
#define OPERATOR_STACK_SIZE    32
#define TOKEN_LIST_SIZE        32
#define OUTPUT_PRECISION        4
#define MODE_TABLE_STEP_BIG    10
#define TERM_MAX_LEN          256

#define UNSHIFT(key)             (key & ~(1 << 4))
#define RAD_TO_DEG(rad)          ((rad) * (180.0 / M_PI))
#define DEG_TO_RAD(deg)          ((deg) * M_PI / 180.0)
#define SIND(x)                  (sin(DEG_TO_RAD((float)(x))))
#define COSD(x)                  (cos(DEG_TO_RAD((float)(x))))
#define TAND(x)                  (tan(DEG_TO_RAD((float)(x))))
#define ASIND(x)                 (RAD_TO_DEG(asin((float)(x))))
#define ACOSD(x)                 (RAD_TO_DEG(acos((float)(x))))
#define ATAND(x)                 (RAD_TO_DEG(atan((float)(x))))
#define FORMAT_NUMBER(v, s, n) \
	(uint8_t *)dtostrf(v, n, OUTPUT_PRECISION, (char *)s)

static uint8_t tok_cnt;
static uint8_t op_stack[OPERATOR_STACK_SIZE];
static float num_stack[NUMBER_STACK_SIZE];
static uint8_t tok_type_list[TOKEN_LIST_SIZE];
static float tok_num_list[TOKEN_LIST_SIZE];

static uint8_t calc_prepare(char *term);
static uint8_t calc_solve(float x, float *y);
static uint8_t asin_acos_range(float n);
static uint8_t get_precedence(uint8_t tt);

/* Calculation */
static uint8_t calc_prepare(char *term)
{
	uint8_t c, cur_type, isop, top_stack, top_num;
	cur_type = TT_NULL;
	tok_cnt = 0;
	top_num = 0;
	top_stack = 0;
	while((c = *term))
	{
		isop = 1;

		/* Tokenizer */
		if(isdigit(c))
		{
			/* Numbers */
			char *begin, dps;
			float n, power;

			/* Find the end of the float */
			for(dps = 0, begin = term; (c = *term); ++term)
			{
				if(c == CHAR_DP)
				{
					if(++dps > 1)
					{
						/* Return a syntax error if there
						is more than one decimal point */
						return ERROR_SYNTAX;
					}
				}
				else if(!isdigit(c))
				{
					/* Break when the end of the number
					(non digit character) is reached */
					break;
				}
			}

			/* Digits before the decimal point */
			for(n = 0.0; begin < term; ++begin)
			{
				if((c = *begin) == CHAR_DP)
				{
					/* Skip the decimal point, if present */
					++begin;
					break;
				}

				n = n * 10.0 + c - '0';
			}

			/* Digits after the decimal point */
			for(power = 1.0; begin < term; ++begin)
			{
				n = n * 10.0 + *begin - '0';
				power *= 10.0;
			}

			if(tok_cnt >= TOKEN_LIST_SIZE - 1)
			{
				return ERROR_NOMEM;
			}

			tok_type_list[tok_cnt++] = cur_type = TT_NUMBER;
			tok_num_list[top_num++] = n / power;
			isop = 0;
		}
		else
		{
			/* Translate characters to tokens */
			switch(c)
			{
			case CHAR_SUB:
				switch(cur_type)
				{
				case TT_NULL:
				case TT_ADD:
				case TT_SUB:
				case TT_MUL:
				case TT_DIV:
				case TT_POW:
				case TT_LP:
					cur_type = TT_UNARY_MINUS;
					break;

				case TT_NUMBER:
				case TT_X:
				case TT_RP:
					cur_type = TT_SUB;
					break;
				}
				break;

			case CHAR_X:
				if(tok_cnt >= TOKEN_LIST_SIZE - 1)
				{
					return ERROR_NOMEM;
				}

				tok_type_list[tok_cnt++] =
					cur_type = TT_X;
				isop = 0;
				break;

			/* Parenthesis */
			case CHAR_LP:
				/* Push onto the operator stack */
				if(top_stack >= OPERATOR_STACK_SIZE - 1)
				{
					return ERROR_NOMEM;
				}

				op_stack[top_stack++] = cur_type = TT_LP;
				isop = 0;
				break;

			case CHAR_RP:
			{
				/* Pop all operators from the stack
				until the opening bracket is found */
				uint8_t t, i;
				cur_type = TT_RP;
				i = 1;
				while(i)
				{
					if(top_stack == 0)
					{
						/* Missing opening bracket */
						return ERROR_SYNTAX;
					}

					if((t = op_stack[top_stack - 1]) == TT_LP)
					{
						i = 0;
					}
					else
					{
						if(tok_cnt >= TOKEN_LIST_SIZE - 1)
						{
							return ERROR_NOMEM;
						}

						tok_type_list[tok_cnt++] = t;
					}

					--top_stack;
				}

				isop = 0;
				break;
			}

			/* Operators */
			case CHAR_ADD:
				cur_type = TT_ADD;
				break;

			case CHAR_MUL:
				cur_type = TT_MUL;
				break;

			case CHAR_DIV:
				cur_type = TT_DIV;
				break;

			case CHAR_POW:
				cur_type = TT_POW;
				break;

			/* Logarithm */
			case 'l':
				cur_type = TT_LOG;
				goto add2;

			/* Trigonometric functions */
			case 'a':
				switch(*(++term))
				{
				case 's':
					cur_type = TT_ASIN;
					break;

				case 'c':
					cur_type = TT_ACOS;
					break;

				case 't':
					cur_type = TT_ATAN;
					break;
				}

				goto add2;

			case 's':
				cur_type = TT_SIN;
				goto add2;

			case 'c':
				cur_type = TT_COS;
				goto add2;

			case 't':
				cur_type = TT_TAN;

			add2:
				term += 2;
			}

			++term;
		}

		/* RPN converter using the
		shunting yard algorithm */
		if(isop)
		{
			/* Any operator: Pop all operators from
			the stack that have a lower precedence */
			uint8_t precedence, tmp;
			precedence = get_precedence(cur_type);
			while(top_stack > 0)
			{
				tmp = op_stack[top_stack - 1];
				if((get_precedence(tmp) > precedence) ||
					(tmp == TT_LP))
				{
					break;
				}

				--top_stack;
				if(tok_cnt >= TOKEN_LIST_SIZE - 1)
				{
					return ERROR_NOMEM;
				}

				tok_type_list[tok_cnt++] = tmp;
			}

			if(top_stack >= OPERATOR_STACK_SIZE - 1)
			{
				return ERROR_NOMEM;
			}

			op_stack[top_stack++] = cur_type;
		}
	}

	/* Pop all remaining operators from the stack */
	while(top_stack > 0)
	{
		if(tok_cnt >= TOKEN_LIST_SIZE - 1)
		{
			return ERROR_NOMEM;
		}

		tok_type_list[tok_cnt++] =
			op_stack[--top_stack];
	}

	return 0;
}

static uint8_t calc_solve(float x, float *y)
{
	float op_left, op_right;
	uint8_t tok_type_i, tok_num_i, top_num;
	tok_type_i = 0;
	tok_num_i = 0;
	top_num = 0;
	for(; tok_type_i < tok_cnt; ++tok_type_i)
	{
		switch(tok_type_list[tok_type_i])
		{
		case TT_NUMBER:
			if(top_num >= NUMBER_STACK_SIZE - 1)
			{
				return ERROR_NOMEM;
			}

			num_stack[top_num++] =
				tok_num_list[tok_num_i++];
			break;

		case TT_X:
			if(top_num >= NUMBER_STACK_SIZE - 1)
			{
				return ERROR_NOMEM;
			}

			num_stack[top_num++] = x;
			break;

		default:
			op_right = 0;
			if(tok_type_list[tok_type_i] < TT_ADD)
			{
				if(top_num == 0)
				{
					/* Buffer underflow */
					return ERROR_SYNTAX;
				}
			}
			else
			{
				if(top_num <= 1)
				{
					/* Buffer underflow */
					return ERROR_SYNTAX;
				}

				op_right = num_stack[--top_num];
			}

			op_left = num_stack[--top_num];
			switch(tok_type_list[tok_type_i])
			{
			case TT_UNARY_MINUS:
				op_left = -op_left;
				break;

			case TT_ADD:
				op_left += op_right;
				break;

			case TT_SUB:
				op_left -= op_right;
				break;

			case TT_MUL:
				op_left *= op_right;
				break;

			case TT_DIV:
				if(op_right == 0.0)
				{
					/* Division by zero */
					return ERROR_MATH;
				}

				op_left /= op_right;
				break;

			case TT_LOG:
				op_left = log(op_left);
				break;

			case TT_SIN:
				op_left = SIND(op_left);
				break;

			case TT_COS:
				op_left = COSD(op_left);
				break;

			case TT_TAN:
				op_left = TAND(op_left);
				break;

			case TT_ASIN:
				if(!asin_acos_range(op_left))
				{
					return ERROR_RANGE;
				}
				op_left = ASIND(op_left);
				break;

			case TT_ACOS:
				if(!asin_acos_range(op_left))
				{
					return ERROR_RANGE;
				}
				op_left = ACOSD(op_left);
				break;

			case TT_ATAN:
				op_left = ATAND(op_left);
				break;

			case TT_POW:
				op_left = pow(op_left, op_right);
				break;

			default:
				continue;
			}

			if(top_num >= NUMBER_STACK_SIZE - 1)
			{
				return ERROR_NOMEM;
			}

			num_stack[top_num++] = op_left;
			break;
		}
	}

	if(top_num != 1)
	{
		return ERROR_SYNTAX;
	}

	*y = num_stack[--top_num];
	return 0;
}

static uint8_t asin_acos_range(float n)
{
	return n >= -1 && n <= 1;
}

static uint8_t get_precedence(uint8_t tt)
{
	switch(tt)
	{
	case TT_ADD:
	case TT_SUB:
		return 3;

	case TT_MUL:
	case TT_DIV:
		return 2;

	case TT_POW:
		return 1;
	}

	return 0;
}

