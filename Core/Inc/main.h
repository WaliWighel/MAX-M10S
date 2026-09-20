/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32h7rsxx_hal.h"

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
typedef enum {
    EVENT_NONE,

    EVENT_SPI_TX_DONE,

    EVENT_SPI_RX_DONE,

    EVENT_SPI_TX_RX_DONE,

    EVENT_START_CYCLE,

    EVENT_MESSAGE_RECIEVED,

    EVENT_USART_RX,

    EVENT_I2C_RX_DONE,

	EVENT_DATA_RECIEVED,
} Event_t;

typedef enum {
    TS_END_WITH_SUCCESS    = 0x00,
    TS_ERROR               = 0x01, 
    TS_IN_PROGRESS         = 0x02,
} TS_Return_t;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_6
#define LED4_GPIO_Port GPIOE
#define GPS_EXTI_Pin GPIO_PIN_12
#define GPS_EXTI_GPIO_Port GPIOM
#define GPS_RESET_Pin GPIO_PIN_11
#define GPS_RESET_GPIO_Port GPIOM
#define GPS_SAFEBOOT_Pin GPIO_PIN_14
#define GPS_SAFEBOOT_GPIO_Port GPIOM

/* USER CODE BEGIN Private defines */
#define LED1_ON  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED1_OFF HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)

// Lights when executing control loop
#define LED2_ON  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED2_OFF HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)

// Lights when interacting with sensors
#define LED3_ON  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED3_OFF HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)

// Lights while sending Debug data
#define LED4_ON  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET)
#define LED4_OFF HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET)

// Lights up at boot, off if error
#define LED5_ON  HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET)
#define LED5_OFF HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET)

#define NC_RAM   __attribute__((section(".noncacheable_buffer")))
#define RAM1     __attribute__((section (".RAM1")))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
