/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int counter_button = 0;											// A variable that counts the times that the button was pressed
char counter_button_string[2];						// A string variable for the times that the button was pressed 
bool print_button = 0;												// A flag to show if the button was pressed, to print the appropriate message via UART
bool state_changed = 0;										// A flag to show if the state of the LED was changed 
bool led_state = 0;														// A variable that keeps the state of the LED (1-> LED on, 0-> LED off)
uint8_t uart_data[5];													// An array to save the receiving data via UART
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	// UART initializations
  HAL_UART_Receive_IT(&huart2, uart_data, 5); 									// Initialize the first interrupt of the UART receive message
	
	// Send via UART a message to the user
	HAL_UART_Transmit_IT(&huart2, (uint8_t *)"\nGive me your AEM or press a button\n", strlen("\nGive me your AEM or press a button\n"));				
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		// If the state of the LED was changed from any of the callbacks, print a message for this change via UART
		if(state_changed == 1)
		{
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)"\nLED CHANGED\n", strlen("\nLED CHANGED\n"));
			HAL_Delay(50);
			state_changed = 0;								// Reset the state_changed flag (To be ready for use on the next iteration)
		}
		
		// If the button was pressed, print via UART the times that the button was pressed
		if(print_button == 1)
		{
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)"\nTimes pressed: ", strlen("\nTimes pressed: "));					// Transmit via UART the message about the times that the button was pressed
			HAL_Delay(50);
			sprintf(counter_button_string, "%d", counter_button); // Convert the counter_button integer variable to string in order to be sent via UART
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)counter_button_string, 2);										// Send the counter_button_string via UART
			HAL_Delay(50);
			print_button = 0;									// Reset the print_button flag (To be ready for use on the next iteration)
		}
		
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUTTON_EXT_Pin */
  GPIO_InitStruct.Pin = BUTTON_EXT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_EXT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

// ISR callback for UART receiving data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* The program receives a 4 digit number (The AEM)
			If the last digit is even number, the ISR will close the LED
			If the last digit is odd number the ISR will light up the LED
	*/
	if(huart -> Instance == USART2)					// Check if the data that were received, came from the UART channel 2
	{
		if(uart_data[3] % 2 == 0) 									// Check if the uart_data[3] (the last digit of the AEM), is even number 
		{
			// Detect a change on the LED's state	
			if(led_state == 1)														// Check the previous state of the led (if the led was lighten)
			{
				state_changed = 1;											// If the LED was open, then detect that a change will become on its state
				led_state = 0;															// Make the state LOW (LED off)
			}
			HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);				// If the last digit is even number, make the LED pin LOW (Turn off the LED)
		}
		else{																								// Else the last number will be odd number. Therefore the LED must light up
			// Detect a change on the LED's state	
			if(led_state == 0)													// Check if the LED state before the change was closed 
				{
					state_changed = 1;									// If the LED was closed, then detect that a change will become on its state
					led_state = 1;													// Make the state HIGH (LED on)
				}
				HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);					//  If the last digit is odd number, make the LED pin HIGH (Turn onthe LED)
			}
	}
	
	HAL_UART_Receive_IT(&huart2, uart_data, 5);									// Enable the interrupt for a next UART receive
	
}

// ISR callback for the external interrupt (For the button)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// If the user will press the button, then this function will change the LED's state
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		state_changed = 1;									 //  Detect that the LED changed its state
		print_button = 1;											 // Make the print_button flag 1, in order to run the appopriate block on the main function
		counter_button += 1;								// Increase the counter, that counts the times that the LED was pressed, by 1
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
