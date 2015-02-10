 /*
 * Brief:    UART Serial to communicate with Xbee or btooth module
 *
 * Uses:     Uses the PB6 and PB7 pins
 *
 * Datasheet: 
 *
 * Copyright (C) 2014 Rajesh Nakarja. All rights reserved
 * http://www.naklojik.com
 *
 * Copyright (C) 2015 Rajesh Nakarja
 * http://www.naklojik.com
 *
 * This software is licenced under the MIT Licence. For full terms visit:
 * http://opensource.org/licenses/MIT
 */

#include "stm32f4xx_hal.h"
#include "prototypes.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Public variables */
uint8_t telemetryFlag = 0;

/* Private variables */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
uint8_t rxBuffer = '\000';
uint8_t rxString[10];
int rxindex = 0;

/* Private function prototypes */
void executeSerialCommand(uint8_t string[]);
uint8_t compareCommand(uint8_t inString[], uint8_t compString[]);
int16_t parseInt(uint8_t string[]);

void initSerial()
{
	__DMA2_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	//HAL_UART_MspInit(&huart1);
	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);
	print("[OK] Serial started\n");

	/* Start the receiver */
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxBuffer, 1);
}

void print(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 100);
}

void echo(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, 1, 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1);

	int i = 0;

	echo(&rxBuffer);

	if (rxBuffer == '\b') // If Backspace
	{
		rxindex--; 
		if (rxindex < 0) rxindex = 0;
	}

	else if (rxBuffer == '\n' || rxBuffer == '\r') // If Enter
	{
		executeSerialCommand(rxString);
		rxString[rxindex] = 0;
		rxindex = 0;
		for (i = 0; i < 10; i++) rxString[i] = 0;
	}

	else
	{
		rxString[rxindex] = rxBuffer;
		rxindex++;
		if (rxindex > 9)
		{
			rxindex = 0;
			for (i = 0; i < 10; i++) rxString[i] = '\000';
			print("\nBrinir> ");
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	print("[ERROR] Serial error");
}

void executeSerialCommand(uint8_t string[])
{
	if (compareCommand(string, "help"))
	{
		print("Help screen:\n");
	}

	if (compareCommand(string, "lol"))
	{
		print("Lol screen:\n");
	}

	print("Brinir> ");
}

uint8_t compareCommand(uint8_t inString[], uint8_t compString[])
{
	int i = 0;
	int len = strlen(inString);
	for (i = 0; i <= len; i++)
	{
		if (compString[i] != inString[i]) break;
		if (i = len) return 1;
	}
	return 0;
}

int16_t parseInt(uint8_t string[])
{
	return strtof(string, NULL);
}

