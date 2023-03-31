/*
 * PID_Regler_user.c
 *
 *  Created on: May 27, 2022
 *      Author: Jonathan Lennhoff
 */

#include "main.h"
#include "Led_user.h"
#include <stdio.h>
#include "EXTI_Callback_user.h"





extern TIM_HandleTypeDef htim4; 		//Timer 4 Handle | Interrupt für PID-Regler
extern double winkel;					//Winkel von Lagesensor aus main_user
extern int8_t motor_moment;				//motor_moment aus EXTI_Callback_user


int pid_fehler;							//Fehler -> Abweichung des Systems vom Sollwert
int pid_fehler_alt;						//Fehler des letzten durchlaufs
int pid_P_output;						//Output des P-Reglers
int pid_I_output;						//Output des I-Reglers
int pid_D_output;						//Output des D-Reglers
int pid_output;							//Output des PID-Reglers | Gesamtoutput

// Werte welche noch auf das System eingestellt werden müssen
#define pid_W 180						//Sollzustand des Systems
#define pid_P_gain 1					//Gain Wert des P-Reglers
#define pid_I_gain 1					//Gain Wert des I-Reglers
#define pid_I_max 0						//Maximaler Wert des I-Reglers
#define pid_D_gain 1					//Gain Wert des D-Reglers
#define pid_max 100						//Maximaler Gesamtoutput des PID-Reglers

int cnt = 0;							//Zähler für die Durchläufe der Funktion





//Interrupt wird alle 0.2 Millisekunden ausgeführt | für PID-Regler sollten auch 2-4 ms reichen
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	pid_fehler = pid_W - winkel;											//Berechnung der Abweichung
	pid_P_output = pid_P_gain * pid_fehler;									//P-Glied

	pid_I_output = pid_I_output + pid_I_gain * pid_fehler;					//I-Glied
	if (pid_I_output > pid_I_max) pid_I_output = pid_I_max;					//I-Glied nach oben begrenzen
	else if (pid_I_output < -pid_I_max) pid_I_output = -pid_I_max;			//I-Glied nach unten begrenzen

	pid_D_output = pid_D_gain * (pid_fehler - pid_fehler_alt);				//D-Glied
	pid_fehler_alt = pid_fehler;											//Neuen Fehler als den jetzt alten Fehler speichern

	pid_output = pid_P_output + pid_I_output + pid_D_output;				//Output des PID-Controlers berechnen
	if(pid_output > pid_max)pid_output = pid_max;                       	//Begrenzung des Outputs nach oben
	else if(pid_output < -pid_max)pid_output = -pid_max;					//Begrenzung des Outputs nach unten

	//motor_moment =  motor_moment + pid_output ;							//Steuerung der Motorgeschwindigkeit (momentan noch nicht genutzt wegen alternativer Steuerung)




	//Zum Testen des Gesamtsystems wurde eine alternative Steuerung des Motors programmiert

	cnt = cnt + 1;										//Zähler um 1 erhöhen

	//Alle 200 ms
	if ((180 -winkel >=50) & (cnt >= 1000)){			//Wenn der Sensor geneigt ist wird alle 200ms...
		motor_moment =  motor_moment + 1 ;				//das Motormoment und damit die Geschwindigkeit erhöht
		cnt = 0;										//Der Zähler für die 200 ms wird zurückgesetzt
	}
	if ((180 - winkel <=-50) & (cnt >= 1000)){			//Wenn der Sensor in die andere Richtung geneigt ist wird alle 200ms...
		motor_moment =  motor_moment -1 ;				//das Motormoment und damit die Geschwindigkeit erhöht
		cnt = 0;										//Der Zähler für die 200 ms wird zurückgesetzt
	}


	if (motor_moment < -100) motor_moment = -100;		//Begrenzung des minimalen motor_moment auf -100
	if (motor_moment > 100) motor_moment = 100;			//Begrenzung des maximalen motor_moment auf 100

}












