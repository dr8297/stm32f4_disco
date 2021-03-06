/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "usb_device.h"


/* USER CODE BEGIN Includes */
// Function prototype
void spi_lis4dh_read (SPI_HandleTypeDef*, uint8_t*, uint8_t*, int);

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s2;
I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2S2_Init(void);
static void MX_I2S3_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//int8_t* spt_lis4dh_read (SPI_HandleTypeDef *,  int8_t *, int8_t *)
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
typedef enum {menu,wait_hal,change_LED} state_mach_t;
typedef enum {line1_print,line2_print} state_line_t;



//PCD_HandleTypeDef hpcd_USB_OTG_FS;
//USBD_HandleTypeDef  hUsbDeviceFS_main;
 //(USBD_HandleTypeDef*) hpcd_USB_OTG_FS->pData;



//USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) pdev->pClassData;


state_mach_t state_var=menu;

uint8_t line1[]="1:Line 1 of the menu \r\n";
uint8_t line2[]="2: Toggle LEDs. Push LED number[1-3]\r\n";
uint8_t line3[]="key 1 has been pressed \r\n";
uint8_t line4[]="key 2 has been pressed \r\n";
uint8_t data[]="aaaaaaaaaaaaaaa \r\n";
uint8_t CTRL1 =0x20;
int size=2;

int16_t  datax=0;
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
uint8_t usb_state=USBD_OK;
state_line_t menu_line=line1_print;
uint8_t data_spi[2]={0x04,0x03};
uint8_t address_spi[3]={0x8F,0x8F,0xa};

char str[1000];
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_I2S2_Init();
  MX_I2S3_Init();
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();

  /* USER CODE BEGIN 2 */
  //HAL_TIM_Base_Start(&htim3);
  HAL_Delay(100);
//  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin,GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	  //uint8_t HiMsg[]="hello\r\n";
	  //CDC_Transmit_FS(HiMsg, strlen(HiMsg));
		if (state_var==menu){
			switch(menu_line){
				case line1_print:
					usb_state=CDC_Transmit_FS(line1, (uint8_t *) strlen(line1));
					if (usb_state==USBD_OK) {
					menu_line=line2_print;
					break;
					} else {
						menu_line=line1_print;
						break;
					}
				case line2_print:
						usb_state=CDC_Transmit_FS(line2,strlen(line2));
						if (usb_state==USBD_OK){
							HAL_GPIO_TogglePin(GPIOD, LD6_Pin);
							HAL_Delay(100);
							state_var=wait_hal;
							menu_line=line1_print;
							break;
						}
						HAL_Delay(100);
						break;
			}

			//CDC_Transmit_FS(line2, strlen(line2)); This line is not printed, maybe the peripheral is busy

			//https://electronics.stackexchange.com/questions/164075/stm32-usb-cdc-how-to-stop-device-transmitting-via-usb-when-not-connected



	} else if(state_var == wait_hal){

			switch(data[0]) {
			case '1':
				HAL_GPIO_TogglePin(GPIOD, LD4_Pin);
				//state_var=change_LED;
				data[0]=0;
				break;
			case '2':
				HAL_GPIO_TogglePin(GPIOD, LD3_Pin);
				//state_var=change_LED;
				data[0]=0;
				break;
			case '3':
					HAL_GPIO_TogglePin(GPIOD, LD5_Pin);
					data[0]=0;
					break;
			case '4':
					state_var = menu;
					data[0]=0;
					//HAL_GPIO_TogglePin(GPIOC,PC6_Pin);
					 //HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
					break;
			case '5':
				state_var = wait_hal;
				for(int i=0; i<256; i++){
				 //setPWM(htim2, TIM_CHANNEL_1, 255, i);
				 HAL_Delay(10);

				}
				break;
			case '6':

				//address_spi=0x0F;
				//data_spi = 0x0;
				address_spi[0]=0x8F;
				address_spi[1]=0x8F;
				size=2;
				spi_lis4dh_read (&hspi1, address_spi, data_spi,size);
				sprintf(str,"The value is data0: %d and data1: %d\r\n",data_spi[0],data_spi[1]);
				usb_state=CDC_Transmit_FS(str, (uint8_t *) strlen(str));
				if (usb_state==USBD_OK) {
				state_var = wait_hal;
				data[0]=0;
				break;
				} else {
					state_var = menu; // I am not sure about this
					data[0]=0;
					break;
				}

			case '7':

				//The read and write protocol is being used
				// Adress _spi is the array with the address and the the data. The data is written by the uC in the data_spi array
				address_spi[0]=0x20;
				address_spi[1]=0x0F; //PD=1 and enable the axis,
				data_spi[0]=0x0;
				data_spi[1]=0x0;
				size=2;
				sprintf(str,"Changing CTRL1 register \r\n");
				 usb_state=CDC_Transmit_FS(str, (uint8_t *) strlen(str));
				while (usb_state!=USBD_OK);
				spi_lis4dh_read (&hspi1, address_spi, data_spi,size);
				sprintf(str,"The value is data0: %d and data1: %d\r\n",data_spi[1],data_spi[2]);
				usb_state=CDC_Transmit_FS(str, (uint8_t *) strlen(str));
				if (usb_state==USBD_OK) {
				state_var = wait_hal;
				data[0]=0;
				break;
				} else {
					state_var = menu; // I am not sure about this
					data[0]=0;
					break;
				}

			case '8':

				//The read and write protocol is being used
				// Adress _spi is the array with the address and the the data. The data is written by the uC in the data_spi array
				address_spi[0]=(0x28 | 0xC0); //OUT_X_L (28h)
				address_spi[1]=(0x29 | 0x80); //PD=1, //OUT_X_H (29h)
				data_spi[0]=0x1;
				data_spi[1]=0x1;
				data_spi[2]=0x1;
				size=3;
				sprintf(str,"Reading data\r\n");
				usb_state=CDC_Transmit_FS(str, (uint8_t *) strlen(str));
				while (usb_state!=USBD_OK);
				//usb_state=CDC_Transmit_FS(str, (uint8_t *) strlen(str));
				while (data[0]!='9'){
					datax=0;
					spi_lis4dh_read (&hspi1, address_spi, data_spi,size);
					//pointer arithmrtic
					datax= (((int16_t)data_spi[2])<<8)+(int16_t)data_spi[1];
					if (datax <=-5000) {
						HAL_GPIO_WritePin(GPIOD, LD5_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOD, LD4_Pin,GPIO_PIN_RESET);
					} else if (datax >=5000) {
						HAL_GPIO_WritePin(GPIOD, LD4_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOD, LD5_Pin,GPIO_PIN_RESET);
					} else {
						HAL_GPIO_WritePin(GPIOD, LD4_Pin,GPIO_PIN_RESET);
						HAL_GPIO_WritePin(GPIOD, LD5_Pin,GPIO_PIN_RESET);
						}
					}
					//sprintf(str,"The value is data0: %d data1: %d data2: %d \r\n",data_spi[0],data_spi[1],data_spi[2]);
					//sprintf(str,"The value is data: %i \r\n",datax);
					//usb_state=CDC_Transmit_FS(str, (uint8_t *) strlen(str));
					//while (usb_state!=USBD_OK);
				//}
				//if (usb_state==USBD_OK) {
				state_var = wait_hal;
				data[0]=0;
				break;
				/*} else {
					state_var = menu; // I am not sure about this
					data[0]=0;
					break;
				} */
			default:
				state_var = wait_hal;
				break;
			}
	}
			else {
				HAL_Delay(200);
			}


		}

  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2S2 init function */
static void MX_I2S2_Init(void)
{

  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_8K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2S3 init function */
static void MX_I2S3_Init(void)
{

  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_96K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC6   ------> S_TIM3_CH1
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE4 PE5 MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin 
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/*void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse)
{
 HAL_TIM_PWM_Stop(&timer, channel); // stop generation of pwm
 TIM_OC_InitTypeDef sConfigOC;
 timer.Init.Period = period; // set the period duration
 HAL_TIM_PWM_Init(&timer); // reinititialise with new period value
 sConfigOC.OCMode = TIM_OCMODE_PWM1;
 sConfigOC.Pulse = pulse; // set the pulse duration
 sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
 sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
 HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, channel);
 HAL_TIM_PWM_Start(&timer, channel); // start pwm generation
} */

void spi_lis4dh_read (SPI_HandleTypeDef* hspi1, uint8_t* address_spi, uint8_t* data_spi, int size) {
	//uint16_t size=2;
	uint32_t timeout=100;
	HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port,CS_I2C_SPI_Pin,GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_SPI_TransmitReceive(hspi1,address_spi,data_spi,(uint16_t) size,timeout);
	while (hspi1->State != HAL_SPI_STATE_READY);
	HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port,CS_I2C_SPI_Pin,GPIO_PIN_SET);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
