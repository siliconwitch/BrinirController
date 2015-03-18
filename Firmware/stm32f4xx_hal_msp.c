/*
* Brief:    Peripheral IO and interrupt configuration
*
* Uses:
*
* Datasheet:
*
* Copyright (C) 2015 Rajesh Nakarja
* http://www.naklojik.com
*
* This software is licenced under the MIT Licence. For full terms visit:
* http://opensource.org/licenses/MIT
*/

#include "stm32f4xx_hal.h"
#include "config.h"

/* Handle structs */
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

void HAL_GPIO_MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	/*Configure GPIO input pins */
	GPIO_InitStruct.Pin = REC1PIN | REC2PIN | FB1PIN | FB2PIN | FB3PIN | FB4PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(RECPORT/*same as FBPORT*/, &GPIO_InitStruct);

	/*Configure GPIO output pins */
	GPIO_InitStruct.Pin = AUX1PIN | AUX2PIN | AUX3PIN | AUX4PIN | LEDAPIN | LEDBPIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(AUXPORT/*same as LEDPORT*/, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = MOTOR1PIN | MOTOR2PIN | MOTOR3PIN | MOTOR4PIN | SERVO1PIN | SERVO2PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(MOTORPORT, &GPIO_InitStruct);

	/* NVIC inits for the EXTI pins that get inputs */
	HAL_NVIC_SetPriority(EXTI1_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_GPIO_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_GPIO_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI3_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_GPIO_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	HAL_NVIC_SetPriority(EXTI0_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_GPIO_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_GPIO_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_GPIO_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock enable */
    __ADC1_CLK_ENABLE();
	__DMA2_CLK_ENABLE();

	/* Attach ADC interrupts */
	//HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, ADC_PRIORITY, 0);
	//HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn); /* Something goes wrong when turning this on */

    /* ADC1 GPIO Configuration   */
	GPIO_InitStruct.Pin = ANA1PIN | ANA2PIN | ANA3PIN | ANA4PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ANAPORT, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_adc1);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __ADC1_CLK_DISABLE();
  
	/* Pins deinit */
	HAL_GPIO_DeInit(ANAPORT, ANA1PIN | ANA2PIN | ANA3PIN | ANA4PIN);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock enable */
    __I2C1_CLK_ENABLE();
  
    /* I2C1 GPIO Configuration */
	GPIO_InitStruct.Pin = SCLPIN | SDAPIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(I2CPORT, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
    /* Peripheral clock disable */
    __I2C1_CLK_DISABLE();
  
    /* I2C1 GPIO Configuration */
	HAL_GPIO_DeInit(I2CPORT, SCLPIN | SDAPIN);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  }
}

void HAL_IWDG_MspInit(IWDG_HandleTypeDef* hiwdg)
{

}

void HAL_IWDG_MspDeInit(IWDG_HandleTypeDef* hiwdg)
{

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM2)
  {
    /* Peripheral clock enable */
    __TIM2_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM2_IRQn, PPM_INPUT_PRIORITY, PPM_INPUT_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }
  else if(htim_base->Instance==TIM3)
  {
    /* Peripheral clock enable */
    __TIM3_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM3_IRQn, PPM_OUTPUT_PRIORITY, PPM_OUTPUT_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  }
  else if(htim_base->Instance==TIM4)
  {
    /* Peripheral clock enable */
    __TIM4_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM4_IRQn, RADIO_TIMEOUT_PRIORITY, RADIO_TIMEOUT_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  }
  else if(htim_base->Instance==TIM10)
  {
    /* Peripheral clock enable */
    __TIM10_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, WHEELSPEED_PRIORITY, WHEELSPEED_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(htim_base->Instance==TIM11)
  {
    /* Peripheral clock enable */
    __TIM11_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, WHEELSPEED_PRIORITY, WHEELSPEED_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  }
  else if(htim_base->Instance==TIM13)
  {
    /* Peripheral clock enable */
    __TIM13_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, WHEELSPEED_PRIORITY, WHEELSPEED_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  }
  else if(htim_base->Instance==TIM14)
  {
    /* Peripheral clock enable */
    __TIM14_CLK_ENABLE();

    /* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, WHEELSPEED_PRIORITY, WHEELSPEED_TIM_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM2)
  {
    /* Peripheral clock disable */
    __TIM2_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  }
  else if(htim_base->Instance==TIM3)
  {
    /* Peripheral clock disable */
    __TIM3_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  }
  else if(htim_base->Instance==TIM4)
  {
    /* Peripheral clock disable */
    __TIM4_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
  }
  else if(htim_base->Instance==TIM6)
  {
    /* Peripheral clock disable */
    __TIM6_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  }
  else if(htim_base->Instance==TIM10)
  {
    /* Peripheral clock disable */
    __TIM10_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(htim_base->Instance==TIM11)
  {
    /* Peripheral clock disable */
    __TIM11_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  }
  else if(htim_base->Instance==TIM13)
  {
    /* Peripheral clock disable */
    __TIM13_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_UP_TIM13_IRQn);
  }
  else if(htim_base->Instance==TIM14)
  {
    /* Peripheral clock disable */
    __TIM14_CLK_DISABLE();

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    /* Peripheral clock enable */
	__GPIOA_CLK_ENABLE();
    __USART1_CLK_ENABLE();
	__DMA2_CLK_ENABLE();

    /* USART1 GPIO Configuration */
	GPIO_InitStruct.Pin = TXPIN | RXPIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(UARTPORT, &GPIO_InitStruct);

    /* Peripheral DMA init*/
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_DISABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_usart1_rx);

	hdma_usart1_tx.Instance = DMA2_Stream7;
	hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_tx.Init.Mode = DMA_NORMAL;
	hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_usart1_tx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_usart1_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	HAL_DMA_Init(&hdma_usart1_tx);

    __HAL_LINKDMA(huart, hdmarx, hdma_usart1_rx);
	__HAL_LINKDMA(huart, hdmatx, hdma_usart1_tx);

	/* Attach DMA interrupts */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, UART_PRIORITY, UART_RX_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, UART_PRIORITY, UART_TX_SUBPRIORITY);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    /* Peripheral clock disable */
    __USART1_CLK_DISABLE();
  
    /* USART1 GPIO Configuration */
	HAL_GPIO_DeInit(UARTPORT, TXPIN | RXPIN);

    /* Peripheral DMA DeInit */
    HAL_DMA_DeInit(huart->hdmarx);
	HAL_DMA_DeInit(huart->hdmatx);
  }
}