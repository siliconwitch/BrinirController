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

/* Public variables */
uint8_t telemetryFlag = 0;

/* Private variables */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
uint8_t rxBuffer = '\000';
uint8_t rxString[100];
int rxindex = 0;

/* Private function prototypes */
void executeSerialCommand(uint8_t string[], int length);


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
	sendSerialString("[OK] Serial started..\n");

	/* Start the receiver */
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxBuffer, 1);
}

void sendSerialString(char string[])
{
	int lentest = 0;
	while((string[lentest] != 10) && lentest < 100){  lentest++;  } /* Determines size of string by looking for a \n or ASCII 'NL'. Cuts off after 100 chars */
	HAL_UART_Transmit(&huart1, (uint8_t*)string, lentest+1, 100);
}

void echo(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, 1, 100);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	
	echo(&rxBuffer);

	rxString[rxindex] = rxBuffer;
	rxindex++;

	if (rxindex > 20) rxindex = 0;

	if (rxBuffer == '5' ) sendSerialString(rxString);


}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	sendSerialString("[ERROR] Serial error\n");
}

void executeSerialCommand(uint8_t string[], int length)
{
	if(string[0] == '-') /* All commands start with a - */
	{ 
		switch(string[1])
		{
			case 'h': /* Help command */
				sendSerialString("display help\n");
				break;

			case 'c':
				sendSerialString("calibrate\n");
				break;

			case 't':
				if (telemetryFlag == 0)
				{
					telemetryFlag = 1;
					sendSerialString("Telemetry started\n");
				}

				else
				{
					telemetryFlag = 0;
					sendSerialString("Telemetry stopped\n");
				}
				break;
		}
	}
}

