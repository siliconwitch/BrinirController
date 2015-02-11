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
#include "clicommands.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Public variables */
uint8_t telemetryFlag = 0;

/* Private variables */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
uint8_t rxBuffer = '\000';
uint8_t rxString[MAXCLISTRING];
int rxindex = 0;
int TXBusy = 0;

/* Private function prototypes */
void executeSerialCommand(uint8_t string[]);
uint8_t compareCommand(uint8_t inString[], uint8_t compString[], float *numArg);

/* Main serial init function. Run this before sending any serial */
void initSerial()
{
	__DMA2_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	HAL_UART_MspInit(&huart1);
	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);
	print("\r\n[OK] Serial started");

	/* Start the receiver */
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxBuffer, 1);
}

/* Prints the supplied string to uart */
void print(char string[])
{
	//HAL_UART_Transmit_DMA(&huart1, (uint8_t*)string, strlen(string));
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 100); // Working transmit function
}

/* UART TX complete callback */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}

/* UART RX complete callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1); // Clear the buffer to prevent overrun

	int i = 0;

	print(&rxBuffer); // Echo the inputed character

	if (rxBuffer == 8 || rxBuffer == 127) // If Backspace or del
	{
		print(" \b"); // Properly clear the character
		rxindex--; 
		if (rxindex < 0) rxindex = 0;
	}

	else if (rxBuffer == '\n' || rxBuffer == '\r') // If Enter
	{
		executeSerialCommand(rxString);
		rxString[rxindex] = 0;
		rxindex = 0;
		for (i = 0; i < MAXCLISTRING; i++) rxString[i] = 0; // Clear the string buffer
	}

	else
	{
		rxString[rxindex] = rxBuffer;
		rxindex++;
		if (rxindex > MAXCLISTRING)
		{
			rxindex = 0;
			for (i = 0; i < MAXCLISTRING; i++) rxString[i] = 0; // Clear the string buffer
			print("\r\nBrinir> ");
		}
	}
}

/* Callback if error has occured in the serial */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	print("\r\n[ERROR] Serial error");
}

/* When enter is pressed, the command is excecuted here */
void executeSerialCommand(uint8_t string[])
{
	float numArg = 0;

	if (compareCommand(string, CMD_HELP, &numArg))
	{
		print("\r\nHelp screen result:");
	}

	if (compareCommand(string, CMD_ABOUT, &numArg))
	{
		print("\r\nLol screen result:");
	}

	if (compareCommand(string, CMD_REBOOT, &numArg))
	{
		safeMode();
		while (1){ } // Will timeout though watchdog
	}

	print("\r\nBrinir> ");
}

/* Compared the command with acceptable strings. Also gets number numerical arg if present */
uint8_t compareCommand(uint8_t inString[], uint8_t compString[], float *numArg)
{
	int i = 0;
	int j = 0;
	*numArg = 0.0;
	uint8_t numString[10]; // Max 10 character numerical string

	int len = strlen(compString); // For length of expected command
	for (i = 0; i < len; i++)
	{
		if (compString[i] != inString[i]) return 0; // Compares expected to actual, return if false

		if (i == len - 1) // If we've checked the entire expected command
		{
			for (i = 0; i <= len + 10; i++) // Check a bit more into the inString
			{
				if (inString[i] == ':') // If we see a : it means we have an arg coming up
				{
					i++; // We dont want the : character in the num string
					for (j = 0; j < 10; j++) { numString[j] = inString[i + j]; } // Copy over the numerical arg
					*numArg = strtof(numString, NULL);
				}
			}
			return 1; // command worked
		}
	}
}
