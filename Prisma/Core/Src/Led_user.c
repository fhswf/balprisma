/*
 * Led_user.c
 *
 *  Created on: Apr 4, 2022
 *      Author: https://controllerstech.com/interface-ws2812-with-stm32/
 *
 */

#include <stdio.h>
#include "main.h"
#include "Led_user.h"




extern TIM_HandleTypeDef htim2;			//Timer 2 -> LED-Ring
#define MAX_LED 24						//Maximale Anzahl der LEDs auf dem Ring
#define PI 3.14159265					//PI definiert

volatile int datasentflag;				//Variable: wird 1 wenn daten gesendet wurden -> nur einmalige Sendung der Daten
uint8_t LED_Data[MAX_LED][4];			//Array in dem die Nummern und Farbdaten der 24 LEDs gespeichert werden
uint16_t pwmData[(24*MAX_LED)+50];		//Array in dem alle Bytes die gesendet werden gespeichert sind


//Funktion mit der im main_user programm die Farbwerte einer bestimmten LED gespeichert werden können
void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;		//Nummer der LED
	LED_Data[LEDnum][1] = Green;		//Grüner Farbwert als uint8_t -> 0...255
	LED_Data[LEDnum][2] = Red;			//Roter  Farbwert als uint8_t -> 0...255
	LED_Data[LEDnum][3] = Blue;			//Blauer Farbwert als uint8_t -> 0...255
}


//Funktion mit der die Bytes an den LED-Ring gesendet werden
void WS2812_Send (void)
{
	uint32_t indx=0;		//Index -> welche LED gerade angesprochen wird
	uint32_t color;			//Farbe ->


	for (int i= 0; i<MAX_LED; i++) //Alle LEDs werden der Reihe nach durchgegangen
	{

		//Alle 3 Farben Bytes der momentanen LED (je nach index) werden in einer uint32_t Variable gespeichert
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));


		//Das pwmData Array in dem alle zu Sendenden Bits gespeichert werden wird Beschrieben
		for (int i=23; i>=0; i--) 		//Alle 24 Bits werden einzeln durchgegangen-> 3 Byte -> 3 Farben einer LED
		{
			if (color&(1<<i))			//Ist das Betrachtete Bit 1 soll (später) eine 1 gesendet werden
			{
				pwmData[indx] = 60;  	//-> High Anteil über 50% -> von LED IC als 1 erkannt
			}

			else pwmData[indx] = 30;  	//-> High Anteil unter 50% -> von LED IC als 0 erkannt

			indx++;						//Bit-Wert gespeichert -> nächster Wert soll in nächsten Arrayplatz geschrieben werden
		}

	}



	for (int i=0; i<50; i++) //Nach der eigentlichen Übertragung wird noch ein reset Code gesendet
	{
		pwmData[indx] = 0;	//Momentanes Byte wird auf 0 gesetzt
		indx++;				//Index wird um 1 erhöht -> nächstes Byte
		//Vorgang wird 50 mal wiederholt
	}

	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_3, (uint32_t *)pwmData, indx); 		//PWM wird gestartet
	while (!datasentflag){};														//es wird gewartet bis die Daten gesendet wurden (datasentflag=1;)
	datasentflag = 0;																//datasentflag wird für die nächste Übertragung zurückgesetzt
}


//Funktion die nach senden der Bytes aufgerufen wird
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim2)
{
	HAL_TIM_PWM_Stop_DMA(htim2, TIM_CHANNEL_3); 	//Senden/PWM wird angehalten
	datasentflag=1;									//auf 1 gesetzt -> Nur einmalige sendung der Daten

}



