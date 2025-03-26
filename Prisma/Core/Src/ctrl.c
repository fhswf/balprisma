/************************************************************************************
 *
 *      oooooooooooooo          **
 *      o   .---.    o         ****   ***
 *      o   | * |   oo         ****   ***
 *       oo '---' oo           ****
 *         oo   oo              **           Fachhochschule Suedwestfalen
 *           ooo                **           Mechatronik / Mikrocomputer / EmbSys
 *            o                              (c) Prof. Tobias Ellermeyer
 *    ==================        **
 *     BALANCING PRISMA         **
 *
 *
 * ctrl.c
 *
 *  Created on: Nov 21, 2023
 *      Author: tobi
 */

#include "main.h"
#include "globals.h"
#include "bldc.h"
extern TIM_HandleTypeDef htim7;

static int32_t cnt_old=0;
static int16_t stall_cnt = 1;		// Number of IRQ until stall is detected



void TIM7_IRQHandler(void)
{
	uint32_t hall;

	// cnt / (Polepairs * Commutations)  -> Rotations (in 1/100 sec)
	// `100 -> Rotations per sec. *60 per min
    __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);

    hall=hallcnt_g;	// hallcnt_g gets reset in bldc_calc_rpm
    bldc_calc_rpm();

    if (cnt_old != hall)
    {
    	stall_cnt = 1;
    }
    else
    {
    	if (stall_cnt<10)
    	{
    		stall_cnt++;
    	}
    	else
    	{
    		rpm_g = 0.;
    	}

    }
	rpma_g = 0.9*rpma_g + 0.1*rpm_g;
	//rpm_g = (float)(hallcnt_g-cnt_old)*6000.;
	cnt_old = hall;
	trigger_g++;

	return;
}
