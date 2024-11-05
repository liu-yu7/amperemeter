/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mygui.h"
#include "st7735.h"
#include "ina226.h"
#include "stm32g4xx_hal_dma.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi1_tx;
extern TIM_HandleTypeDef htim6;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SET_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
  static uint32_t last_time = 0;
  if(HAL_GPIO_ReadPin(SET_GPIO_Port, SET_Pin) == GPIO_PIN_RESET)
  {
    last_time = HAL_GetTick();
  }
  else if(HAL_GetTick() - last_time > 500)
  {
    Button_Callback(Button_SET_l);
  }
  else
  {
    Button_Callback(Button_SET_s);
  }
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */

  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(DOWN_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */
  Button_Callback(Button_DOWN);
  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
  if(__HAL_DMA_GET_FLAG(&hdma_spi1_tx, DMA_FLAG_TC1))
  {
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET); //spi�����������Ƭ�?
  }
  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  
  
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(UP_Pin);
  HAL_GPIO_EXTI_IRQHandler(ALERT_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */
  if(HAL_GPIO_ReadPin(ALERT_GPIO_Port, ALERT_Pin) == GPIO_PIN_RESET)
  {
    HAL_GPIO_WritePin(SW_IO_GPIO_Port, SW_IO_Pin, GPIO_PIN_RESET);
    measure.output = 0;
  }
  else if(HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin) == GPIO_PIN_RESET)
  {
    Button_Callback(Button_UP);
  }

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC3 channel underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
  static uint8_t t = 0;
  if (t++ > 99) 
  {
    t = 0;
    measure.time++; 
  }

  if(t%10 == 0)
  {
    while(measure.current > measure.current_reso)
    {
      measure.current_reso *= 2;
      for(uint8_t i = 0; i < 35; i++)measure.current_buf[i] /= 2;
    }
    measure.current_buf[measure.current_buf_index++] = measure.current/measure.current_reso*60.0f;

    if(measure.current_buf_index == 36)measure.current_buf_index = 0; 

    if(measure.current_reso > 0.125f)
    {
      uint8_t num = 0;
      for(uint8_t i = 0; i < 35; i++)
      {
        if(measure.current_buf[i] > 30)
        {
          break;
        }
        num++;
      }
      if(num == 35)
      {
        measure.current_reso /= 2;
        for(uint8_t i = 0; i < 35; i++)measure.current_buf[i] *= 2;
      }
    }
  }

  measure.current = INA226_GetCurrent();
  //电流补偿
  if(measure.current < 1.0f)
  {
    measure.current = measure.current + usr_config.current_offset[0]*measure.current/1.0f;
  }
  else if(measure.current < 2.0f)
  {
    measure.current = measure.current + usr_config.current_offset[1]*measure.current/2.0f;
  }
  else if(measure.current < 5.0f)
  {
    measure.current = measure.current + usr_config.current_offset[2]*measure.current/5.0f;
  }
  else if(measure.current < 10.0f)
  {
    measure.current = measure.current + usr_config.current_offset[3]*measure.current/10.0f;
  }
  else if(measure.current < 20.0f)
  {
    measure.current = measure.current + usr_config.current_offset[4]*measure.current/20.0f;
  }
  else if(measure.current < 50.0f)
  {
    measure.current = measure.current + usr_config.current_offset[5]*measure.current/50.0f;
  }
  measure.voltage = INA226_GetBusV();
  //电压补偿
  if(measure.voltage < 5.0f)
  {
    measure.voltage = measure.voltage + usr_config.voltage_offset[0]*measure.voltage/5.0f;
  }
  else if(measure.voltage < 10.0f)
  {
    measure.voltage = measure.voltage + usr_config.voltage_offset[1]*measure.voltage/10.0f;
  }
  else if(measure.voltage < 15.0f)
  {
    measure.voltage = measure.voltage + usr_config.voltage_offset[2]*measure.voltage/15.0f;
  }
  else if(measure.voltage < 20.0f)
  {
    measure.voltage = measure.voltage + usr_config.voltage_offset[3]*measure.voltage/20.0f;
  }
  else if (measure.voltage < 25.0f)
  {
    measure.voltage = measure.voltage + usr_config.voltage_offset[4]*measure.voltage/25.0f;
  }
  else if(measure.voltage < 30.0f)
  {
    measure.voltage = measure.voltage + usr_config.voltage_offset[5]*measure.voltage/30.0f;
  }
  measure.power = INA226_GetPower();
  measure.energy += measure.power/100/60/60;

  if(measure.current > measure.max_current)measure.max_current = measure.current;
  if(measure.current < measure.min_current)measure.min_current = measure.voltage;

  if(t%2 == 1)ST7735_Draw();
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
