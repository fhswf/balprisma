/*
 * usb_printf.c
 *
 *  Created on: 09.09.2020
 *      Author: Tobias Ellermeyer
 */

#include "main.h"

extern UART_HandleTypeDef huart2;

void uart_init()
{
	return;
}

int __io_putchar(int ch)
{
 uint8_t c[1];
 c[0] = ch & 0x00FF;
 HAL_UART_Transmit(&huart2, &*c, 1, 0xFFFF);
 return ch;
}

int _write(int file,char *ptr, int len)
{
 int DataIdx;
 __io_putchar('w');
 for(DataIdx= 0; DataIdx< len; DataIdx++)
 {
 __io_putchar(*(ptr++) );
 }
return len;
}
