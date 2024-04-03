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
 * main_user.c
 *
 *  Created on: Oct 27, 2023
 *      Author: tobi
 */


/***** TIMER Usage *****
 *
 * TIM1  - bldc.c  -> PWM on ENx for Motor
 * TIM2  - ledring.c  -> WS2812 data
 * TIM3  - bldc.c  -> PWM on INx for Motor
 * TIM4  - ledring.c  -> Periodic update of LED display
 * TIM5  - bldc.c  -> 32bit counter for RPM measurement
 * TIM7  - ctrl.c  -> Timer for 100 Hz control loop
 */
#include <leds.h>
#include "main.h"
#include "uart.h"
#include "bluetooth.h"
#include "bldc.h"
#include "adc.h"
#include <stdio.h>

#define MAIN_C
#include "globals.h"

extern TIM_HandleTypeDef htim7;

color_t blue=RGB(0,0,32);

void test_ledring_status();

void main_user(void)
{
	int d=1;
	int cnt=0;
	uart_init();
	printf("UART Initialized\r\n");
	ledring_init();
	ledring_welcome();
	// Init Bluetooth
	bt_init();

	// Test LEDRING_Status
	//test_ledring_status();
	ledring_welcome();

	uart_printf_to_bt(1);

	adc_init();

	printf("ADC startet\r\n");
	bldc_init();
	HAL_TIM_Base_Start_IT(&htim7);
	printf("TIM7\r\n");
	//while(1)
	{
		bldc_mode(d);
		d=1-d;

		for (int f=0;f<=100;f+=10)
		{
			bldc_torque(-f);
			printf("Hall cnt: %li   rpm: %f  %f\r\n",hallcnt_g,rpm_g,rpma_g);
			HAL_Delay(200);
		}
		/*
		for (int f=100;f>=-100;f-=10)
		{
			bldc_torque(f);
			HAL_Delay(200);
		}
		for (int f=-100;f<=0;f+=10)
		{
			bldc_torque(f);
			HAL_Delay(200);
		}
*/
		bldc_torque(-40);
		while(1)
		{
			adc_get_and_restart();
			printf("Vbat1: %6.2lf Vbat2: %6.2lf Vbat3: %6.3lf  Imot: %6.3lf\r\n",
					vbat1_g, vbat2_g, vbat3_g, imot_g);
			printf("Hall cnt: %li   rpm: %f  %f\r\n",hallcnt_g,rpm_g,rpma_g);
			HAL_Delay(200);
			if (cnt==50) bldc_torque(0);
			if (cnt==100) bldc_torque(100);
			cnt++;
		}
	}
}



void test_ledring_status()
{
	int16_t i;
	for (i=-100;i<=100;i+=10)
	{
		force_accel_g = i;
		ledring_status();
		HAL_Delay(100);
	}
	force_accel_g = 0;
	for (i=-100;i<=100;i+=10)
	{
		force_brake_g = i;
		ledring_status();
		HAL_Delay(100);
	}
	vbat_mv_g=8000;

	for (i=-45;i<=45;i+=5)
	{
		angle_g=i;
		ledring_status();
		HAL_Delay(100);
	}
	for (i=45;i>=-45;i-=5)
	{
		angle_g=i;
		ledring_status();
		HAL_Delay(100);
	}
}
