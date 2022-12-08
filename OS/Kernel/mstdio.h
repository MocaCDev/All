#ifndef STDIO
#define STDIO
#include "util.h"

void PutC(uint8 c, uint32 color1, uint32 color2)
{
	uint8 *val = (uint8 *)(FONT + ((c * 16) - 16));
	uint32 *fb = (uint32 *)FB;

	fb += (tc.cursor_x * 8) + (tc.cursor_y * 16 * WIDTH);

	for(uint8 i = 0; i < 16; i++)
	{
		for(int8 b = 7; b >= 0; b--)
		{
			*fb = (val[i] & (1 << b)) ? color1 : color2;
			fb++;
		}
		fb += (WIDTH - 8);
	}

	tc.cursor_x++;
}

void Print(uint8 *str, uint32 fg, uint32 bg)
{
	while(*str != 0)
	{
		switch(*str)
		{
			case '\n':
			{
				tc.cursor_y++;
				tc.cursor_x = 0;
				str++;
				update_screen();
				break;
			}
			case '\t':
			{
				tc.cursor_x += 4;
				str++;
				break;
			}
			default:
			{
				PutC(*str, fg, bg);
				str++;
				break;
			}
		}
	}
}

void Err(uint8 *str)
{
	Print(str, MakeColor(255, 0, 0), BLACK);

	//__asm("hlt");
}

#define PANIC(msg) \
	Err((uint8 *)msg); \
	__asm("hlt;");

static void PrintHex(uint32 number)
{

	if(number <= 0)
	{
		uint8 *val = (uint8 *)"0x0";
		Print((uint8 *)val, WHITE, BLACK);
		return;
	}

	uint8 hex[80];
	uint8 numbers[17] = "0123456789ABCDEF";
	uint8 i = 0, j = 0;

	while(number > 0)
	{
		hex[i] = numbers[(uint8)number & 0x0F];
		number >>= 4;
		i++;
	}


	hex[i++] = 'x';
	hex[i++] = '0';
	hex[i] = '\0';
	i--;

	for(j=0; j < i; j++, i--)
	{
		uint8 temp = hex[j];
		hex[j] = hex[i];
		hex[i] = temp;
	}
	Print((uint8 *)hex, WHITE, BLACK);
}

void PrintNum(uint32 _number)
{
	uint8 dec[80];
	uint8 i = 0, j, temp;

	while(_number > 0)
	{
		dec[i] = (_number % 10) + '0';
		_number /= 10;
		i++;
	}

	dec[i] = '\0';
	i--;

	for(j = 0; j < i; j++, i--)
	{
		temp = dec[j];
		dec[j] = dec[i];
		dec[i] = temp;
	}

	Print((uint8 *)dec, WHITE, BLACK);
}

#endif