/*
 * lage.c
 *
 *  Created on: Apr 24, 2024
 *      Author: Tobias Ellermeyer
 */

#include <stdint.h>
#include "main.h"
#include "bno055.h"
#include "math.h"
#include "bldc.h"
#include "globals.h"
#include "adc.h"
#include "uart.h"

#define _USE_MATH_DEFINES
#include <math.h>

typedef struct
{
	float k1;
	float k2;
	float k3;
	float gyro_smooth;
	float gyro_amount;
	float offset_gyroZ;
	float offset_angle;
	float angle;
	float gyro;
	float pwm;
} filter_t;

#define LOOP_TIME_MS   10   // Loop time in milliseconds
#define RAD2DEG    180. / M_PI
#define UMIN2DEGS  360. / 60.

uint8_t cnt=0;


/********************************************************
 * Calibrate the gyro offset as well as the start angle
 ********************************************************/
void angle_calibrate(filter_t *p)
{
	int32_t i;
	bno055_vector_t gyro;
	bno055_vector_t acc;

	printf("Calibrating BNO ...\r\n");
	for (i=0;i<100;i++)
	{
		if (i%10==0) printf(".");
		gyro = bno055_getVectorGyroscope();
		acc  = bno055_getVectorAccelerometer();
		p->offset_gyroZ += gyro.z;
		p->offset_angle += atan2(acc.x,acc.y);

		HAL_Delay(20);
	}
	p->offset_gyroZ /= 100.;
	p->offset_angle /= 100.;
	p->offset_angle *= RAD2DEG;
	printf("\r\nOffset Gyro: %6.4f Angle: %6.4f\r\n", p->offset_gyroZ, p->offset_angle);
}

/*************************************************
 * Calculate current angle
 *************************************************/
void angle_calculate(filter_t *p)
{
  float acc_angle;
  static float gyro_angle = 0.;
  static uint8_t init = 0;


  bno055_vector_t gyro = bno055_getVectorGyroscope();
  bno055_vector_t accel = bno055_getVectorAccelerometer();

  gyro.z -= p->offset_gyroZ;

  // Integrate gyro go get angle
  // TODO: Initialize if started from side
  gyro_angle += gyro.z * LOOP_TIME_MS/1000.0;
  if (!init)
  {
	  init = 1;
	  gyro_angle+=p->offset_angle;
  }

  // Filter gyro value
  p->gyro = p->gyro_smooth * gyro.z  + (1 - p->gyro_smooth) * p->gyro;

  // caltulate angle from Acceleration
  acc_angle = atan2(accel.x, accel.y) * RAD2DEG;

  // Calculate prisma angle
  p->angle = p->gyro_amount * gyro_angle + (1.0 - p->gyro_amount) * acc_angle;
}

/***********************************************************
 * Update params by user input
 ***********************************************************/
void param_adjust(filter_t *p)
{
	switch(uart_usb_getc())
	{
		case 'a': p->k1 +=0.1; break;
		case 'y': p->k1 -=0.1; break;
		case 's': p->k2 +=0.02; break;
		case 'x': p->k2 -=0.02; break;
		case 'd': p->k3 +=0.0002; break;
		case 'c': p->k3 -=0.0002; break;
		default: break;
	}

}

void lage()
{
	// Set up the params
	filter_t prisma={
			0.0,	// k1
			0.0,	// k2
			0.0,	// k3
			0.5,	// gyro_smooth smoothing for gyro_filt
			0.8,	// gyro_amount for angle
			0.0,	// offset_gyroZ
			0.0,		// offset_angle;
			0.0,
			0.0,
			0.0
	};
	uint8_t trigger_old = 0;

	angle_calibrate(&prisma);  // Calibrate

//	bldc_mode(BLDC_FREERUN);
	bldc_mode(BLDC_DRIVE);
	float pwm_s = 0.;
	float moment = 0.;
	float mX3;

	int pwm;

	//**** LOOP ****
	while(1)
	{
		param_adjust(&prisma);
		angle_calculate(&prisma);

		// Wait for next cycle
		while(trigger_old==trigger_g);
		trigger_old=trigger_g;

		// Only update if prisma is in upright position
		if ((prisma.angle<35.0)&&(prisma.angle>-35.0))
		{
			mX3 = prisma.k3 * rpma_g * UMIN2DEGS;

			mX3 = constrain(mX3,-20.,20.);

			moment = prisma.k1 * prisma.angle + prisma.k2 * prisma.gyro + mX3;

			pwm_s = constrain(moment,-100.,100.);

			// Leave some margin for no pwm at all, but remove dead band
			pwm = pwm_s;
			if (pwm < -3) pwm -=14.;
			if (pwm > +3) pwm +=14.;

			if ( (pwm_s>-3) && (pwm_s<3) )
			{
				bldc_mode(BLDC_FREERUN);

			}
			else
			{
				bldc_mode(BLDC_DRIVE);
			}
			pwm = constrain(pwm,-100,100);

			if (!cnt) printf("pwm: %6.2f %4i M %6.2f  ang: %5.2f  gyro %5.2f rpm %6.2f kpar %5.2f %5.2f %5.3f\r\n",
					pwm_s, pwm, moment, prisma.angle, prisma.gyro, rpma_g, prisma.k1,prisma.k2,prisma.k3);
			if (sgn(pwm)!=sgn(rpma_g))
			{
				bldc_brake(0);
			}
			else
			{	bldc_brake(0);
			}
			bldc_torque(-pwm);
		}
		else
		{
			bldc_torque(0);
		}
		cnt = (cnt+1)%50;
		if (!cnt)
		{
			adc_get_and_restart();
			if ( (vbat1_g<3.8) || (vbat1_g<3.8) || (vbat1_g<3.8) )  // Battery LOW
			printf("ADC %5.3f  %5.3f  %5.3f  %5.3f\r\n", imot_g, vbat1_g, vbat2_g, vbat3_g);
		}
	}
}
