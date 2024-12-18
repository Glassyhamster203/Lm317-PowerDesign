/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "struct_typedef.h"
#include "oled.h"
#include "pid.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
  enum
  {
    POWER_STOP = 0,
    CONSTANT_CURRENT,
    CONSTANT_VOLTAGE,
    CONSTANT_POWER,
  } POWER_MODE_ENUM;

#define PID_CC_P 0.00f
#define PID_CC_I 0.00f
#define PID_CC_D 0.00f

#define PID_CV_P 0.00f
#define PID_CV_I 0.00f
#define PID_CV_D 0.00f

#define PID_CP_P 0.00f
#define PID_CP_I 0.00f
#define PID_CP_D 0.00f

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
  extern Power_t PowerStatus;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
  typedef struct
  {
    fp32 I;
    fp32 U;
    fp32 P;
    
    fp32 I_target;
    fp32 U_target;
    fp32 P_target;
    
    fp32 I_Limit;
    fp32 U_Limit;
    fp32 P_Limit;

    pid_type_def POWER_PID;
    uint8_t POWER_MODE;
    uint8_t FallOff_Enable;
    uint32_t PWM_Val;
  } Power_t;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Sample_U_Pin GPIO_PIN_3
#define Sample_U_GPIO_Port GPIOA
#define Sample_I_Pin GPIO_PIN_5
#define Sample_I_GPIO_Port GPIOA
#define PWM_Output_Pin GPIO_PIN_1
#define PWM_Output_GPIO_Port GPIOB
#define FallOff_Pin GPIO_PIN_13
#define FallOff_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
