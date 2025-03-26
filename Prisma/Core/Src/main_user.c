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
#include "bno055_stm32.h"			//Auslesen des Lagesensors
#include <stdio.h>

#define MAIN_C
#include "globals.h"

extern TIM_HandleTypeDef htim7;

color_t blue=RGB(0,0,32);
extern I2C_HandleTypeDef hi2c1;		//I2C Kommunikation mit Lagesensor

void test_ledring_status();
void lage();

void main_user(void)
{
	int d=1;
	int cnt=0;
	uart_init();
	printf("UART Initialized\r\n");
	ledring_init(DMA_NON_BLOCKING);
	ledring_welcome();
	// Init Bluetooth
	bt_init();

	// Test LEDRING_Status
	//test_ledring_status();
	ledring_welcome();

	uart_printf_to_bt(1);

	adc_init();

	printf("ADC startet\r\n");
	//Kommunikation mit Lagesensor
	printf("BNO setup...\r\n");						//Ausgabe: Setup des Lagesensors hat begonnen
	bno055_assignI2C(&hi2c1);					//Kommunikation
	bno055_setup();								//Setup
	bno055_setOperationMode(8);					//Operationsmodus: IMU;

	bldc_init();
	HAL_TIM_Base_Start_IT(&htim7);
	printf("TIM7\r\n");

	bldc_mode(BLDC_FREERUN);
	int sgn=-1;
/*	while(1)
	{
		printf("Gas (sgn: %2i)!\r\n",sgn);
		bldc_torque(sgn*100);
		HAL_Delay(1000);
		printf("Weniger Gas\r\n");
		bldc_torque(sgn*50);
		HAL_Delay(500);
		bldc_torque(sgn*50);
		HAL_Delay(500);
		printf("Vollbremsung\r\n");
		bldc_torque(sgn*(-100));
		HAL_Delay(1000);
		bldc_torque(0);
		printf("Ausrollen\r\n");
		HAL_Delay(5000);
		sgn=-sgn;

	}*/
	lage();
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
