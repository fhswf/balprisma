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
 * adc.c
 *
 *  Created on: Dec 7, 2023
 *      Author: tobi
 */


#include "main.h"
#include "globals.h"

#define ADC_CHANNELS 	4
#define SCALE_VBAT1     ((3.3/4095.0)*2.0)
#define SCALE_VBAT2     SCALE_VBAT1
#define SCALE_VBAT3     SCALE_VBAT1
#define SCALE_IMOT      ((3.3/4095.0))

static volatile uint8_t adc_conv_complete_flag=0;
static uint16_t adc_vals[ADC_CHANNELS];
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;


void adc_init()
{
	// Do an ADC calibration
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	// First run
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_vals, ADC_CHANNELS); // start adc in DMA mode
}

void adc_get_and_restart()
{
	// Get and scale values
	imot_g  = adc_vals[0] * SCALE_IMOT;
	vbat1_g = adc_vals[1] * SCALE_VBAT1;
	vbat2_g = adc_vals[2] * SCALE_VBAT2;
	vbat3_g = adc_vals[3] * SCALE_VBAT3;

	// Restart ADC
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_vals, ADC_CHANNELS); // start adc in DMA mode
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	// I set adc_conv_complete_flag variable to 1 when,
	// HAL_ADC_ConvCpltCallback function is call.
	adc_conv_complete_flag++;
}
