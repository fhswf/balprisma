/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOT_SENSE_Pin GPIO_PIN_0
#define MOT_SENSE_GPIO_Port GPIOC
#define BAT1_SENSE_Pin GPIO_PIN_1
#define BAT1_SENSE_GPIO_Port GPIOC
#define BAT2_SENSE_Pin GPIO_PIN_2
#define BAT2_SENSE_GPIO_Port GPIOC
#define BAT3_SENSE_Pin GPIO_PIN_3
#define BAT3_SENSE_GPIO_Port GPIOC
#define USB_TX_Pin GPIO_PIN_2
#define USB_TX_GPIO_Port GPIOA
#define USB_RX_Pin GPIO_PIN_3
#define USB_RX_GPIO_Port GPIOA
#define BT_EN_Pin GPIO_PIN_4
#define BT_EN_GPIO_Port GPIOA
#define BT_PWR_N_Pin GPIO_PIN_5
#define BT_PWR_N_GPIO_Port GPIOA
#define MOT1_PWM_Pin GPIO_PIN_6
#define MOT1_PWM_GPIO_Port GPIOA
#define MOT2_PWM_Pin GPIO_PIN_7
#define MOT2_PWM_GPIO_Port GPIOA
#define MOT3_PWM_Pin GPIO_PIN_0
#define MOT3_PWM_GPIO_Port GPIOB
#define NEOPIXEL_Pin GPIO_PIN_10
#define NEOPIXEL_GPIO_Port GPIOB
#define HALL1_Pin GPIO_PIN_12
#define HALL1_GPIO_Port GPIOB
#define HALL1_EXTI_IRQn EXTI15_10_IRQn
#define HALL2_Pin GPIO_PIN_13
#define HALL2_GPIO_Port GPIOB
#define HALL2_EXTI_IRQn EXTI15_10_IRQn
#define HALL3_Pin GPIO_PIN_14
#define HALL3_GPIO_Port GPIOB
#define HALL3_EXTI_IRQn EXTI15_10_IRQn
#define MOT1_EN_Pin GPIO_PIN_8
#define MOT1_EN_GPIO_Port GPIOA
#define MOT2_EN_Pin GPIO_PIN_9
#define MOT2_EN_GPIO_Port GPIOA
#define MOT3_EN_Pin GPIO_PIN_10
#define MOT3_EN_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_11
#define LED_GPIO_Port GPIOC
#define BT_TX_Pin GPIO_PIN_6
#define BT_TX_GPIO_Port GPIOB
#define BT_RX_Pin GPIO_PIN_7
#define BT_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
