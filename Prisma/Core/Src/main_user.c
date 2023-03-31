/* USER CODE BEGIN Header */
/*
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
*/
#include <stdio.h>
#include "main.h"

#include "Led_user.h"				//Ansteuerung des Led-Rings
#include "bno055_stm32.h"			//Auslesen des Lagesensors
#include "EXTI_Callback_user.h"		//Interrupt bei Änderung der Hall-Sensoren
#include "Spulenschaltung_user.h"	//Schaltung der EN1, 2, 3 und der PWM Chanel 1, 2, 3
#include "PID_Regler_user.h"		//Regelung der Motorgeschwindigkeit mittels Sensordaten

extern TIM_HandleTypeDef htim4; 	//Timer 4 Handle für den Interrupt des PID-Reglers
extern I2C_HandleTypeDef hi2c1;		//I2C Kommunikation mit Lagesensor
extern int8_t motor_moment;			//motor_moment aus Callback Funktion -> Ausgabe des momentanen Wertes mit printf möglich
extern int pid_output;				//pid_output aus PID_Regler_user -> Ausgabe des momentanen Wertes mit printf möglich

volatile double winkel;				//Variable: Winkel von Lagesensor




extern ADC_HandleTypeDef hadc1;



void main_user(void)
{

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

	HAL_Delay(500);

	uint16_t value[4];

	/*
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)value, 4); // start adc in DMA mode
	while(1)
	{
		//HAL_ADC_PollForConversion(&hadc1, 1);
		printf("%10u %10u %10u %10u\r\n",value[0],value[1],value[2],value[3]);
		printf("%7.3lf %7.3lf %7.3lf %7.3lf\r\n",
				3.3*value[0]/4095.0,
				3.3*value[1]/4095.0,
				3.3*value[2]/4095.0,
				3.3*value[3]/4095.0
				);
		HAL_Delay(200);
	}
	*/


	//Kommunikation mit Lagesensor
	printf("setup...\r\n");						//Ausgabe: Setup des Lagesensors hat begonnen
	bno055_assignI2C(&hi2c1);					//Kommunikation
	bno055_setup();								//Setup
	bno055_setOperationMode(8);					//Operationsmodus: IMU;
	printf("setup complete\r\n");				//Ausgabe: Setup des Lagesensors ist abgeschlossen


	//Variablen für Farbanimation
	int r, g, b; 								//Variablen Farben
	int relativpos;								//relative Pos. des aktuellen Pixels zum Zeigerpixel
	int realpos;								//reale Pos. des aktuell betrachteten Pixels (0...23)
	int zeigerpixel;							//reale Pos. des Zeigerpixels -> bestimmt mit Lagesensor (0° bis 360°)

	//Variablen für Helligkeitsanimation
	int hell [13]; 								//Array speichert die Helligkeit der LED Positionen
	int lauflichtpos = 1;						//Pos. des Lauflichts aktell
	int lauflichtup = 1;						//Pos. des Lauflichts vorher
	uint32_t count = 0;							//Zählvariable zum weiterschalten des Lauflichts

	// Timer 4 wird gestartet
	HAL_TIM_Base_Start_IT(&htim4);				//PID-Regler Interrupt wird gestartet





	//Reset der Motorsperre (wenn ein Netzteil genutzt wird, statt der vorgesehenen Lipo Akkus)
	/*
	HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_SET);		//Pin setzen
	HAL_Delay(1000);																//1 s warten
	HAL_GPIO_WritePin(Motor_Enable_GPIO_Port, Motor_Enable_Pin, GPIO_PIN_RESET);	//Pin rücksetzen
	HAL_Delay(1000);																//1 s warten
	*/





	while (1){



		//Abfrage des Operationsmodus und sensor kalibierung zur kontrolle
		int j;

		if (j<=5000)													//status ca. alle 5s
			{
				j += 1;
			}
		else
		{
			bno055_opmode_t operationsmodus;											//Variable vom typ bno055_opmode_t
			bno055_calibration_state_t calstate;										//Variable vom typ bno055_calibration_state_t
			operationsmodus = bno055_getOperationMode();								//Operationsmodus auslesen
			calstate = bno055_getCalibrationState();    								//calibration status auslesen

			//Operationsmodus (8: IMU) und status der calibrierung (0: nicht calibriert, 3: calibriert)
			printf("OPmode: %x  ACC_cal: %x  GYR_cal: %x\n\r", operationsmodus, calstate.accel, calstate.gyro);

			j = 0;
		}



		//Winkel erfassen
		bno055_vector_t v = bno055_getVectorEuler();				//Variable vom typ bno055_vector_t erstellen und Lagedaten reinschreiben
		winkel = v.x;												//Übergabe des relevanten winkels an die Variable winkel



		//oben definieren

		bno055_vector_t gravity;
		gravity = bno055_getVectorGravity();
		//printf("grav_test: w %.2lf  x %.2lf  y %.2lf  z %.2lf | ", gravity.w, gravity.x, gravity.y, gravity.z);	//testausgabe erdbeschleunigung
		if (gravity.x <= 0)
		{
			zeigerpixel = 0;
		}
		else
		{
			zeigerpixel = 3;
		}


		/*
		//Zeigerpixel mittels Winkel ermitteln
		for (int i = 0; i < 24; i++){											//Kreis in 24 x15° Schritte aufteilen
			if  (((360-winkel) >= i * 15) && ((360-winkel) <= (i+1) * 15 )){ 	//Überprüfen ob der Winkel innerhalb des momentan betrachteten Bereichs liegt
				//printf("H: %.2f I: %i\n\r", v.x, i);							//Momentane Lage und Nummer des Zeigerpixels ausgeben
				zeigerpixel = i;												//Pos. des Zeigerpixels in zeigerpixel schreiben
			}
		}
		*/


		//Helligkeitslauflicht
		for (int i = 1; i <= 12; i++){ hell[i] = 1;}				//Die Helligkeit aller Pixel (bis auf Zeigerpixel) auf 1 setzen
		hell[0] = 2;												//Die Helligkeit des Zeigerpixels wird auf 2 gesetzt
		hell[lauflichtpos] = 4;										//Die Helligkeit der beiden momentanen Lauflichtpixel wird auf 4 gesetzt

		if (lauflichtpos == 12){lauflichtup = 0;}					//Wenn lauflichtpos bis 12 hochgezählt wurde soll wieder runtergezählt werden
		if (lauflichtpos == 0){lauflichtup = 1;}					//Wenn lauflichtpos bis 0 runtergezählt hat soll wieder hochgezählt werden

		if ((lauflichtup == 1) & (count >= 10)){					//Wenn lauflichtup auf 1 gesetzt wurde und die while Schleife 10 mal durchlaufen wurde...
			lauflichtpos = lauflichtpos + 1;						//wird die lauflichtposition um 1 erhöht...
			count = 0;												//und der Zähler zurückgesetzt
		}
		if ((lauflichtup == 0) & (count >= 10)){					//Wenn lauflichtup auf 0 gesetzt wurde und die while Schleife 10 mal durchlaufen wurde...
			lauflichtpos = lauflichtpos - 1;						//wird die lauflichtposition um 1 verringert...
			count = 0;												//und der Zähler zurückgesetzt
		}
		count = count + 1;											//Pro while Schleifen durchlauf wird der Zähler um 1 erhöht





		//Alle Pixel einfärben - Da das Muster gespiegelt ist gibt es statt 24 nur 12 relativpositionen
		for (relativpos = 0; relativpos <= 12; relativpos++){					//alle relativpositionen werden der reihe nach Farbwerte zugewiesen

			if (relativpos > 3){  g = 1; } else{ g = 0; }						//Grün-Wert je nach relativer Pos. zu Zeigerpixel festlegen
			if (relativpos < 7){  b = 1; } else{ b = 0; }						//Blau-Wert je nach relativer Pos. zu Zeigerpixel festlegen
			if (relativpos > 9){  r = 1; } else{ r = 0; }						//Rot--Wert je nach relativer Pos. zu Zeigerpixel festlegen
			if (relativpos==0 ){  r = 5; }										//Zeigerbit wird heller Dargestellt


			realpos = zeigerpixel - relativpos; 								//relative Pos. zu realer Pos. umrechnen (rechts von Zeigerpixel)
			if (realpos < 0){realpos = 24 + realpos;}							//Unterlauf berücksichtigen

			//Farbwerte übergeben, berücksichtigung der (vom Lauflichtteil) gewünschten Helligkeit
			Set_LED( realpos, r*4*hell[relativpos], g*4*hell[relativpos], b*4*hell[relativpos]);

			realpos = zeigerpixel + relativpos; 								//relative Pos. zu realer Pos. umrechnen (links von Zeigerpixel)
			if (realpos > 23){realpos = realpos - 24;}							//umrechnen bei Überlauf

			//Farbwerte übergeben, berücksichtigung der (vom Lauflichtteil) gewünschten Helligkeit
			Set_LED( realpos, r*4*hell[relativpos], g*4*hell[relativpos], b*4*hell[relativpos]);
		}


		//prisma aufrichten






		//Farbwerte an LED-Ring Senden lassen
        WS2812_Send();

        //1 ms warten
        HAL_Delay(1);

        //Ausgabe von motor_moment, winkel, pid_output zur kontrolle
        printf("M: %d | W: %lf | PID: %i \n\r", motor_moment, winkel, pid_output);

        //Callbackfunktion aufrufen da (ohne Änderung an den Hall-Sensoren) der Motor sonst nicht alleine starten kann
        HAL_GPIO_EXTI_Callback(0);

  }
}
