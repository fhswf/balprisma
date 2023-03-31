/*
 * main_user.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Tobias Ellermeyer
 */


#include "main.h"

void main_user()
{
	while(1)
	{
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(200);
	}
}
