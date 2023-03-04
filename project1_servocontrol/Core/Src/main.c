/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
int8_t maxabs(int8_t a[],int8_t len);
int8_t myabs(int8_t a);
int8_t IS_ACCEPT_RIGHT(uint8_t * RXBUFFER,int length);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}ARM;

ARM ARM_ARRAY[6];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int TIM_COUNT = 0;
int ARM_DEGREE[6] = {75,75,75,75,75,75};  //
int ARM_DEGREE_LAST[6] = {75,75,75,75,75,75};
char high[6] ={0};
uint8_t RXBUFFER[9]={0,0,0,0,0,0,0,0,0};
int RXFLAG=0;
uint8_t test=1;
uint8_t test2=2;
uint8_t mode=1; //0:default(fast) 1:middle 2:slow

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void ARM_INIT();
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
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  ARM_INIT();//Init ARM Struct
  __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);//!!!!!!!!!!!!!!!!!!!PSC72	ARR10
  HAL_TIM_Base_Start_IT(&htim3);

  HAL_Delay(3000);
  ARM_DEGREE[5] = 25;   //open gripper

  while(HAL_UART_Receive_IT(&huart1, RXBUFFER, 9) != HAL_OK);
  while(HAL_UART_Transmit_IT(&huart1, RXBUFFER, 9)!= HAL_OK);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(RXFLAG==1)
	  	  {
		  	  HAL_UART_Transmit_IT(&huart1, &test, 1);
	  		  IS_ACCEPT_RIGHT(RXBUFFER,9);
	  		  RXFLAG=0;
	  	  }

//	  int step=1;
//	  ARM_DEGREE[4]+=step;
//	  HAL_Delay(100);  //  36deg/s
//	  if(ARM_DEGREE[4]>125)
//	  {
//		  ARM_DEGREE[4]=75;
//	  }



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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 71;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 19;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB5 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  //25-125  -> -90 -- +90  1.8deg/int
{
	static int i=0;
	if (htim == &htim3)
	{
		TIM_COUNT++;
		for(i=0;i<6;i++)
		{
			if(TIM_COUNT<=ARM_DEGREE[i])
				{
						if(high[i]==0)
							{
							HAL_GPIO_WritePin(ARM_ARRAY[i].GPIOx,ARM_ARRAY[i].GPIO_Pin,GPIO_PIN_SET);
							high[i]=1;
							}

				}
			else
				{
					if(high[i]==1)
					{
						HAL_GPIO_TogglePin(ARM_ARRAY[i].GPIOx,ARM_ARRAY[i].GPIO_Pin);
						high[i]=0;
					}
				}
		}


		if(TIM_COUNT>=1001)
			{
				TIM_COUNT = 1;
			}
	}
	__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);



}
void ARM_INIT()
{
	ARM_ARRAY[0].GPIOx = GPIOB;
	ARM_ARRAY[0].GPIO_Pin = GPIO_PIN_8;
	ARM_ARRAY[1].GPIOx = GPIOB;
	ARM_ARRAY[1].GPIO_Pin = GPIO_PIN_5;
	ARM_ARRAY[2].GPIOx = GPIOD;
	ARM_ARRAY[2].GPIO_Pin = GPIO_PIN_2;
	ARM_ARRAY[3].GPIOx = GPIOC;
	ARM_ARRAY[3].GPIO_Pin = GPIO_PIN_12;
	ARM_ARRAY[4].GPIOx = GPIOC;
	ARM_ARRAY[4].GPIO_Pin = GPIO_PIN_11;
	ARM_ARRAY[5].GPIOx = GPIOC;
	ARM_ARRAY[5].GPIO_Pin = GPIO_PIN_10;

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	RXFLAG=1;
	while(HAL_UART_Receive_IT(&huart1, RXBUFFER, 9) != HAL_OK);
	//while(HAL_UART_Transmit_IT(&huart1, RXBUFFER, 9)!= HAL_OK);

}
int8_t IS_ACCEPT_RIGHT(uint8_t * RXBUFFER,int length)
{
	int8_t i=0;
	int8_t j=0;
	int8_t step_counter=0;
//	float total_steps = 40;  //2 seconds
	uint8_t position[6]={0,0,0,0,0,0};
	int8_t delta[6]={0,0,0,0,0,0};

	int8_t step[6]={1,1,1,1,1,1};
//	int arm_index=0;  //1-6
	if((RXBUFFER[0])==0xaa&&RXBUFFER[8]==0xff)
	{
		for(i=0;i<6;i++)
		{
			position[i]=(RXBUFFER[i+1]);

		}

	}
	if(mode==0)
	{
		for(i=0;i<6;i++)
		{
			ARM_DEGREE[i]=25+100*position[i]/180;
		}


	}
	if(mode==1)
	{
		for(i=0;i<6;i++)
		{
			delta[i]=position[i]-(uint8_t)ARM_DEGREE[i];
			if(delta[i]<0) {step[i]=-1;}
		}

//maxabs(delta,6)
			for(i=0;i<maxabs(delta,6);i++)
					{
						for(j=0;j<6;j++)
						{	if(ARM_DEGREE[j]==(int)position[j])
							{
							}
							else ARM_DEGREE[j]+=step[j];
						}
						HAL_Delay(10);   //5S 180DEG

					}

//			while(HAL_UART_Receive_IT(&huart1, RXBUFFER, 9) != HAL_OK)
//			{
//				HAL_UART_Transmit_IT(&huart1, &test, 1);
//			}


//			for(i=0;i<(-delta);i++)
//					{
//						ARM_DEGREE[arm_index-1]+=(-step);
//						HAL_Delay(50);   //5S 180DEG
//					}


	}
	HAL_UART_Transmit_IT(&huart1, &test2, 1);
}

int8_t maxabs(int8_t a[],int8_t len)
{
	int8_t copy[len];
	int8_t i=0;
    for(i=0;i<len;i++)
    {
        copy[i]=a[i];
    }

    int8_t temp;
	for(i=0;i<len-1;i++)
	{
		if(myabs(copy[i+1])<myabs(copy[i]))
		{
			temp=copy[i];
			copy[i]=copy[i+1];
			copy[i+1]=temp;
		}
	}
	return myabs(copy[len-1]);
}

int8_t myabs(int8_t a)
{
	if(a>=0) return a;
	else return -a;
	}
//HAL_GPIO_WritePin()
//void PWM_CONTROL(GPIO_TypeDef GPIOX,uint16_t GPIO_Pin,int degree)//-90__+90 -> 50__250
//{
//	;
//}
//int * count()
//{
//
//}
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
