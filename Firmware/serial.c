 /*
 * Brief:    UART Serial to communicate with Xbee or bluetooth module
 *
 * Uses:     Uses the PB6 and PB7 pins
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
#include "prototypes.h"
#include "config.h"
#include "clicommands.h"
#include <string.h>
#include <stdlib.h>

/* Globals */
uint8_t telemetryFlag = 0;
ControllerConfigStruct ControllerConfig;

/* Private variables */
uint8_t rxBuffer = '\000';
uint8_t rxString[MAXCLISTRING];
int rxindex = 0;
int TXBusy = 0;

/* Private function prototypes */
void executeSerialCommand(uint8_t string[]);
uint8_t compareCommand(uint8_t inString[], uint8_t compString[], float *numArg);

/* Handle structs */
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
IWDG_HandleTypeDef hiwdg;

/* Main serial init function. Run this before sending any serial */
void initSerial()
{
	/* Init UART */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = BAUDRATE;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

	/* Print OK */
	print("\r\n[OK] Serial started");

	/* Start the receiver */
	__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive_DMA(&huart1, &rxBuffer, 1);
}

/* Prints the supplied string to uart */
void print(char string[])
{
	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 5);
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
	uint8_t tempBool = 0;

	/* Big help screen */
	if (compareCommand(string, CMD_HELP, &numArg))
	{
		print("\r\n Format is:");
		print("\r\n   command:number");
		print("\r\n number must be between:");
		print("\r\n   -1000 and 1000 for (permilli) commands");
		print("\r\n   1 or 0 for (boolean) commands\n");
		/* Because serial can be time consuming */
		#ifdef WATCHDOG_EN
				HAL_IWDG_Refresh(&hiwdg);
		#endif
		print("\r\n defaults - Sets defaults (no args)");
		print("\r\n print config - Prints out the current config (no args)");
		print("\r\n power bias - Sets front to rear power bias (permilli)");
		print("\r\n front slip - Sets front diff slip (permilli)");
		/* Because serial can be time consuming */
		#ifdef WATCHDOG_EN
				HAL_IWDG_Refresh(&hiwdg);
		#endif
		print("\r\n rear slip - Sets rear diff slip (permilli)");
		print("\r\n gyro gain - Sets steering correction gain (permilli)");
		print("\r\n steering trim - Trims the steering (permilli)");
		/* Because serial can be time consuming */
		#ifdef WATCHDOG_EN
				HAL_IWDG_Refresh(&hiwdg);
		#endif
		print("\r\n invert steering - Inverts the steering (boolean)");
		print("\r\n wheel feedback - Enables the wheel speed feedback (boolean)\n");

	}

	/* Prints current config */
	if (compareCommand(string, CMD_PRINT_CONFIG, &numArg))
	{
		uint8_t intString[10];

		print("\r\n Power bias: ");
		itoa(ControllerConfig.powerBias, &intString, 10);
		print(&intString);

		print("\r\n Front slip: ");
		itoa(ControllerConfig.frontSlip, &intString, 10);
		print(&intString);

		print("\r\n Rear slip: ");
		itoa(ControllerConfig.rearSlip, &intString, 10);
		print(&intString);

		print("\r\n Gyro gain: ");
		itoa(ControllerConfig.gyroGain, &intString, 10);
		print(&intString);

		print("\r\n Steering trim: ");
		itoa(ControllerConfig.steeringTrim, &intString, 10);
		print(&intString);

		print("\r\n Invert steering: ");
		itoa(ControllerConfig.invertSteering, &intString, 10);
		print(&intString);

		print("\r\n Wheel speed feedback: ");
		itoa(ControllerConfig.enableWheelSpeedFeedback, &intString, 10);
		print(&intString);
		print("\r\n");
	}

	/* Setting power bias etc */
	if (compareCommand(string, CMD_SET_POWERBIAS, &numArg))
	{
		print("\r\n Power bias ");
		setControllerConfig(powerBias, (int32_t)numArg);
	}

	if (compareCommand(string, CMD_SET_FRONTSLIP, &numArg))
	{
		print("\r\n Front slip ");
		setControllerConfig(frontSlip, (int32_t)numArg);
	}

	if (compareCommand(string, CMD_SET_REARSLIP, &numArg))
	{
		setControllerConfig(rearSlip, (int32_t)numArg);
		print("\r\n Rear slip ");
	}

	if (compareCommand(string, CMD_SET_GYROGAIN, &numArg))
	{
		setControllerConfig(gyroGain, (int32_t)numArg);
		print("\r\n Gyro gain ");
	}

	if (compareCommand(string, CMD_SET_STEERINGTRIM, &numArg))
	{
		setControllerConfig(steeringTrim, (int32_t)numArg);
		print("\r\n Steering trim ");
	}

	if (compareCommand(string, CMD_INVERT_STEERING, &numArg))
	{
		setControllerConfig(invertSteering, (int32_t)numArg);
		print("\r\n Steering invert ");
	}

	if (compareCommand(string, CMD_WHEELFEEDBACK, &numArg))
	{
		setControllerConfig(enableWheelSpeedFeedback, (int32_t)numArg);
		print("\r\n Wheel speed feedback ");
	}

	/* Restores default settings */
	if (compareCommand(string, CMD_DEFAULTS, &numArg))
	{
		/* Set defaults */
		setControllerDefaults();
	}

	/* Reboots CPU */
	if (compareCommand(string, CMD_REBOOT, &numArg))
	{
		/* Flag for reset */
		ResetPendingFlag = 1;
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

/* Converts int to string */
char* itoa(int value, char* result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}