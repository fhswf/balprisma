/*
 * EXTI_Callback_user.c
 *
 *  Created on: May 27, 2022
 *      Author: Jonathan Lennhoff
 */

#include "main.h"
#include <stdio.h>
#include "Spulenschaltung_user.h"				//Spulenschaltung_user einbinden da die Funktion Interrupt_nach_Hall(); genutzt wird


volatile int direction = 0;						//Variable für die Richtung in die der Motor drehen soll
volatile int8_t motor_moment;					//Variable für das ARR der Spulen-PWM -> Motormoment / Motorgeschwindigkeit
volatile uint8_t Zustaende[2][8] = 		{		//Array in dem die je nach Hallsensorwerten folgende Spulenschaltung gespeichert ist
{0, 23, 31, 21, 12, 13, 32, 0},					//CW Clockwise
{0, 21, 32, 31, 13, 23, 12, 0}					//CCW Counter Clockwise
										};



// Callback-Funktion für Hall Sensoren | Übergabe des gewünschten Motormoments
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


	uint8_t Hall_Sensor;							//Zahlenwert der zusammengerechneten Hallsensoren
	uint8_t moment_uebergabe;						//Übergabevariable für motor_moment

	//Zusammenrechnung der Hall-Sensor-Werte
	Hall_Sensor  = HAL_GPIO_ReadPin( Hall1_GPIO_Port, Hall1_Pin );
	Hall_Sensor |= HAL_GPIO_ReadPin( Hall2_GPIO_Port, Hall2_Pin )<<1;
	Hall_Sensor |= HAL_GPIO_ReadPin( Hall3_GPIO_Port, Hall3_Pin )<<2;



	if (motor_moment < 0){							//Ist das Motormoment negativ...
		moment_uebergabe = motor_moment * (-1);		//wird der Übergabewert für das ARR positiv...
		direction = 0;								//und die Variable für die Drehrichtung wird angepasst
	}

	else {											//Ist das Motormoment positiv...
		moment_uebergabe = motor_moment;			//Wird an dem Übergabewert keine änderung vorgenommen...
		direction = 1;								//und die Variable für die Drehrichtung wird angepasst
	}


	//Limitierung des maximal übergebenen Wertes auf 100
	if (moment_uebergabe > 100) moment_uebergabe = 100;

	//Aufrufen der Funktion zum Schalten der Spulen / die gwünschte Spulenschaltung und die Spulen-PWM werden übergeben
	//Interrupt_nach_Hall(Zustaende[direction][Hall_Sensor], moment_uebergabe);

}






