/*
 * Spulenschaltung_user.c
 *
 *  Created on: May 27, 2022
 *      Author: Jonathan Lennhoff
 */


#include "main.h"
#include <stdio.h>

//Funktion die von dem Interrupt der Hall-Sensor-Wert-Änderung aufgerufen wird
void Interrupt_nach_Hall (uint8_t Schalten, uint8_t Moment){


    extern TIM_HandleTypeDef htim3;					//Timer 3 -> PWM für die 3 Spulen

	HAL_TIM_Base_Start(&htim3);						//PWM starten
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);		//PWM Chanel 1 -> Spule 1 starten
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);		//PWM Chanel 2 -> Spule 2 starten
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);		//PWM Chanel 3 -> Spule 3 starten

	//Switch case Anweisung -> Schaltung von EN1, EN2, EN3 und IN1, IN2, IN3 je nach übergebenem Wert
	switch (Schalten)
		{

		case 12:  // Strom läuft von Spule 1 durch Spule 2

			//Setzen der Enable Pins (nach Tabelle 6 & 7 der Hausarbeit)
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);

			//Schalten der PWM -> Einstellung des ARR nach übergabewert der Callback Funktion
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Moment);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);

			break;

		case 13:   // Strom läuft von Spule 1 durch Spule 3

			//Setzen der Enable Pins (nach Tabelle 6 & 7 der Hausarbeit)
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

			//Schalten der PWM -> Einstellung des ARR nach übergabewert der Callback Funktion
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Moment);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			break;

		case 21:  // Strom läuft von Spule 2 durch Spule 1

			//Setzen der Enable Pins (nach Tabelle 6 & 7 der Hausarbeit)
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);

			//Schalten der PWM -> Einstellung des ARR nach übergabewert der Callback Funktion
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, Moment);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			break;

		case 23: // Strom läuft von Spule 2 durch Spule 3

			//Setzen der Enable Pins (nach Tabelle 6 & 7 der Hausarbeit)
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

			//Schalten der PWM -> Einstellung des ARR nach übergabewert der Callback Funktion
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, Moment);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
			break;

		case 31: // Strom läuft von Spule 3 durch Spule 1

			//Setzen der Enable Pins (nach Tabelle 6 & 7 der Hausarbeit)
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

			//Schalten der PWM -> Einstellung des ARR nach übergabewert der Callback Funktion
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, Moment);
			break;

		case 32:  // Strom läuft von Spule 3 durch Spule 2

			//Setzen der Enable Pins (nach Tabelle 6 & 7 der Hausarbeit)
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

			//Schalten der PWM -> Einstellung des ARR nach übergabewert der Callback Funktion
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, Moment);
			break;
		}

}




