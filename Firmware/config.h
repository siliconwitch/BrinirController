 /*
 * Brief:       Main config, pin mappings and global variables.
 *
 *				From here we can see all the major variables shared	between the 
 *				vehicle control system as well as all the peripherals. There are
 *				also many config values that can be tweaked for different setups.
 *
 * Copyright (C) 2015 Rajesh Nakarja
 * http://www.naklojik.com
 *
 * This software is licenced under the MIT Licence. For full terms visit:
 * http://opensource.org/licenses/MIT
 */

#ifndef __PINMAPPINGS_H
#define __PINMAPPINGS_H
 
/*------------------------*/
/* INITIAL CONFIGURATION  */
/*------------------------*/

	/* Motor related */
	#define MOTORPOLES	14

	/* ADC related */
	#define ADCSAMPLES	ADC_SAMPLETIME_15CYCLES

	/* Serial related */
	#define BAUDRATE 	9600

	/* Receiver related */
 	#define PPMTIMEOUTVALUE		50000
 	#define PPMTIMEOUTPRESCALER	42

 	#define PPMMEASUREPRESCALER	4
 	#define RECEIVERMINSIGNAL	16000
 	#define RECEIVERMIDSIGNAL	25000
 	#define RECEIVERMAXSIGNAL	34000

	/* IMU related */
	#define DEFAULTACCELRANGE	4
	#define DEFAULTGYRORANGE	500
 	#define TIMEOUT 			1000
	
	/* Misc */
	#define MAGICNUMBER 66 /* If this isn't in the flash, it means the flash has been wiped and we need to restore defaults */
	#define WATCHDOG_EN /* Comment out to disable watchdog, NOT RECOMENDED */

/*------------------------*/
/* PIN MAPPINGS FOR BOARD */
/*------------------------*/

	/* This is just a reference, don't change these pins without 
	   checking where they are used. Each pin also have
	   interrupts tied to them, so they'd also need changing */

	/* LED pins */
	#define LEDAPIN		GPIO_PIN_0
	#define LEDBPIN		GPIO_PIN_1
	#define LEDPORT		GPIOB

	/* Pins and port for motor outputs */
	#define MOTOR1PIN	GPIO_PIN_9		/* Wheel BR*/
	#define MOTOR2PIN	GPIO_PIN_8		/* Wheel BL*/
	#define MOTOR3PIN	GPIO_PIN_7		/* Wheel FR*/
	#define MOTOR4PIN	GPIO_PIN_6		/* Wheel FL*/
	#define SERVO1PIN	GPIO_PIN_11
	#define SERVO2PIN	GPIO_PIN_10
	#define MOTORPORT	GPIOC

	/* Pins and port for AUX outputs */
	#define AUX1PIN		GPIO_PIN_12
	#define AUX2PIN		GPIO_PIN_13
	#define AUX3PIN		GPIO_PIN_14
	#define AUX4PIN		GPIO_PIN_15
	#define AUXPORT		GPIOB

	/* Serial port pins */
	#define TXPIN		GPIO_PIN_9
	#define RXPIN		GPIO_PIN_10
	#define BTSTATPIN	GPIO_PIN_8	// TODO implement this pin
	#define UARTPORT	GPIOA

	/* I2C port pins*/
	#define SCLPIN		GPIO_PIN_8
	#define SDAPIN		GPIO_PIN_9
	#define I2CPORT		GPIOB

	/* Hallsensor feedback input pins */
	#define FB1PIN		GPIO_PIN_4
	#define FB2PIN		GPIO_PIN_5
	#define FB3PIN		GPIO_PIN_6
	#define FB4PIN		GPIO_PIN_7
	#define FBPORT		GPIOA

	/* Receiver input channel pins */
	#define REC1PIN		GPIO_PIN_0
	#define REC2PIN		GPIO_PIN_1
	#define RECPORT		GPIOA

	/* Analogue input pins */
	#define ANA1PIN		GPIO_PIN_0
	#define ANA2PIN		GPIO_PIN_1
	#define ANA3PIN		GPIO_PIN_2
	#define ANA4PIN		GPIO_PIN_3
	#define ANAPORT		GPIOC

/*------------------------*/
/*  INTERRUPT PRIORITES   */
/*------------------------*/

	/* If using group 3 priorities, max can be 8 for main and 1 for sub*/
	/* Low number means higher priority*/
	#define PPM_INPUT_PRIORITY 1
	#define PPM_INPUT_GPIO_SUBPRIORITY 0
	#define PPM_INPUT_TIM_SUBPRIORITY 1

	#define PPM_OUTPUT_PRIORITY 0
	#define PPM_OUTPUT_GPIO_SUBPRIORITY 0
	#define PPM_OUTPUT_TIM_SUBPRIORITY 1

	#define RADIO_TIMEOUT_PRIORITY 2
	#define RADIO_TIMEOUT_GPIO_SUBPRIORITY 0
	#define RADIO_TIMEOUT_TIM_SUBPRIORITY 1

	#define WHEELSPEED_PRIORITY 3
	#define WHEELSPEED_GPIO_SUBPRIORITY 0
	#define WHEELSPEED_TIM_SUBPRIORITY 1

	#define ADC_PRIORITY 5

	#define I2C_PRIORITY 4

	#define UART_PRIORITY 6
	#define UART_RX_SUBPRIORITY 0
	#define UART_TX_SUBPRIORITY 1


/*------------------------*/
/*    GLOBAL VARIABLES    */
/*------------------------*/
	
	typedef struct { int32_t powerBias, frontSlip, rearSlip, gyroGain, steeringTrim, invertSteering, enableWheelSpeedFeedback, magicNumber } ControllerConfigStruct;

	typedef struct { double steering, throttle, aux1, aux2; } RCRadio;

	typedef struct { double MOT1, MOT2, MOT3, MOT4, SER1, SER2, AUX1, AUX2, AUX3, AUX4; } PPMOutputs;

	typedef struct { double FL, FR, BL, BR; } WheelRPM;

	typedef struct { uint16_t ch1,  ch2,  ch3,  ch4; } AnalogueOutput; //currently unused

	typedef struct { float x, y, z, roll, pitch, yaw, temp, valid; } IMUMotion;

	uint8_t telemetryFlag;

	uint8_t FlashWritePendingFlag; /* Flag is 1 if flash write pending */

	uint8_t ResetPendingFlag; /* Flag is 1 if reset pending */

#endif