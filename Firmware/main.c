 /*
 * Brief:       Main file for Project Aelith running on STM32F4 devices
 *
 * Datasheet:   http://www.st.com/web/en/resource/technical/document/datasheet/DM00037051.pdf
 *
 * Ref Manual:  http://www.st.com/st-web-ui/static/active/en/resource/technical/document/reference_manual/DM00031020.pdf
 *
 * This software is licenced under the MIT Licence
 *
 * Copyright (c) 2015 Rajesh Nakarja
 * http://www.naklojik.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://opensource.org/licenses/MIT
 */

#include <stm32f4xx_hal.h>
#include "prototypes.h"
#include "config.h"
#include "../Controller/fuzzyController_ert_rtw/fuzzyController.h"
#include "../Controller/fuzzyController_ert_rtw/rtwtypes.h"

/* Public variables */
extern PPMOutputs PPMOutputStructure;
extern RCRadio RCRadioStructure;
extern IMUMotion Motion;
extern ControllerConfigStruct ControllerConfig;

extern const int32_t userConfig[32];

extern uint8_t telemetryFlag;
extern uint8_t FlashWritePendingFlag = 0;


/* Private variables */
IWDG_HandleTypeDef hiwdg;

/* Private function prototypes */
void SystemClock_Config(void);

/* Main function */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Global priority group to use */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3); /* GRP 3 means max 8 possibilities for pre-empt and 2 for sub */

	/* System tick interrupt init*/
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

	/* Init vehicle functions */
	initSerial();
	initIO();
	initADC(); 
	initIMU();

  	/* Configure and start IWDG */
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
	hiwdg.Init.Reload = 2047;
	HAL_IWDG_Init(&hiwdg);
	#ifdef WATCHDOG_EN
		HAL_IWDG_Start(&hiwdg);
		print("\r\n\[OK] Watchdog started");
	#else
		print("\r\n\[WARNING] Watchdog not enabled");
	#endif

	/* Start controller and set initial parameters */
	fuzzyController_initialize();

	/* Loads the controller config registers into RAM */
	loadControllerConfig();

	/* If magic number is invalid, reset to defaults*/
	if (!validateFlash()) setControllerDefaults();

	/* Apply set parameters */
	applyControllerConfig();

	/* Intro message */
	print("\r\n\nBrinir Controller Ready");
	print("\r\n(C) NakLojik 2015");
	print("\r\nFor help type 'help'");
	print("\r\nEnjoy your drive!");
	print("\r\n\nBrinir> ");
	
	/* Main loop */
	while(!FlashWritePendingFlag && !ResetPendingFlag)
	{
		IMUGetMotion();

		/* Send inputs into the controller */
		fuzzyController_U.gyroYaw = Motion.yaw/DEFAULTGYRORANGE;          
		fuzzyController_U.steeringSignal = RCRadioStructure.steering;   
		fuzzyController_U.throttleSignal = RCRadioStructure.throttle;   

		/* Run the controller */ 
		fuzzyController_step();

		/* Apply outputs from controller */
		PPMOutputStructure.SER1 = fuzzyController_Y.steeringOutput;
		PPMOutputStructure.MOT4 = fuzzyController_Y.FLWheelOutput;
		PPMOutputStructure.MOT3 = fuzzyController_Y.FRWheelOutput;
		PPMOutputStructure.MOT2 = fuzzyController_Y.BLWheelOutput;
		PPMOutputStructure.MOT1 = fuzzyController_Y.BRWheelOutput;      
        
		/* Send telemetery */
		if (telemetryFlag)
		{
			//print("idfs\n");
		}

		/* Reset Watchdog */
		#ifdef WATCHDOG_EN
			HAL_IWDG_Refresh(&hiwdg);
		#endif
	}

	/* Left main loop so go into safe mode */
	safeMode();

	/* Extend the watchdog to allow for flash write */
	#ifdef WATCHDOG_EN
		hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
		hiwdg.Init.Reload = 2047;
		HAL_IWDG_Init(&hiwdg);
		HAL_IWDG_Refresh(&hiwdg);
	#endif

	if (FlashWritePendingFlag)
	{
		print("\r\n Writing data to flash, do not power off");
		writeToFlash();
	}

	print("\r\n Restarting system\r\n");
	NVIC_SystemReset();

	return 0;
}

/* Sets controller parameters to defaults */
void setControllerDefaults()
{
	print("\r\n Setting controller defaults");
	ControllerConfig.powerBias = 0;
	ControllerConfig.frontSlip = 950;
	ControllerConfig.rearSlip = 100;
	ControllerConfig.gyroGain = 1000;
	ControllerConfig.steeringTrim = 150;
	ControllerConfig.invertSteering = 0;
	ControllerConfig.enableWheelSpeedFeedback = 0;
	ControllerConfig.magicNumber = MAGICNUMBER;

	FlashWritePendingFlag = 1;
}

/* Loads existing data from controller config memory */
void loadControllerConfig()
{
	ControllerConfig.powerBias = userConfig[powerBias];
	ControllerConfig.frontSlip = userConfig[frontSlip];
	ControllerConfig.rearSlip = userConfig[rearSlip];
	ControllerConfig.gyroGain = userConfig[gyroGain];
	ControllerConfig.steeringTrim = userConfig[steeringTrim];
	ControllerConfig.invertSteering = userConfig[invertSteering];
	ControllerConfig.enableWheelSpeedFeedback = userConfig[enableWheelSpeedFeedback];
	ControllerConfig.magicNumber = userConfig[magicNumber];
}

/* Sets parameter in the controller config */
void setControllerConfig(ControllerConfigEnum param, int32_t value)
{
	switch (param)
	{
	case powerBias:
		if(value < 5000 && value > -5000) ControllerConfig.powerBias = value;
		else print("\r\n Input out of range");
		break;

	case frontSlip:
		if (value < 5000 && value > -5000) ControllerConfig.frontSlip = value;
		else print("\r\n Input out of range");
		break;

	case rearSlip:
		if (value < 5000 && value > -5000) ControllerConfig.rearSlip = value;
		else print("\r\n Input out of range");
		break;

	case gyroGain:
		if (value < 5000 && value > -5000) ControllerConfig.gyroGain = value;
		else print("\r\n Input out of range");
		break;

	case steeringTrim:
		if (value < 5000 && value > -5000) ControllerConfig.steeringTrim = value;
		else print("\r\n Input out of range");
		break;

	case invertSteering:
		if (value <= 1 && value >= 0) ControllerConfig.invertSteering = value;
		else print("\r\n Input out of range");
		break;

	case enableWheelSpeedFeedback:
		if (value <= 1 && value >= 0) ControllerConfig.enableWheelSpeedFeedback = value;
		else print("\r\n Input out of range");
		break;

	default:
		print("[ERROR] Incorrect parameter chosen");
		break;
	}

	FlashWritePendingFlag = 1;
}

/* Applies constants to controller */
void applyControllerConfig()
{
	//fuzzyController_U.wheelFeedbackEnable =			ControllerConfig.enableWheelSpeedFeedback;
	fuzzyController_U.reverseSteering =				ControllerConfig.invertSteering;
	fuzzyController_U.steeringTrim =		(float) ControllerConfig.steeringTrim	/ 1000;
	fuzzyController_U.gyroGain =			(float) ControllerConfig.gyroGain		/ 1000;
	fuzzyController_U.frontDifferential =	(float) ControllerConfig.frontSlip		/ 1000;
	fuzzyController_U.rearDifferential =	(float) ControllerConfig.rearSlip		/ 1000;
	fuzzyController_U.powerBias =			(float) ControllerConfig.powerBias		/ 1000;
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __GPIOH_CLK_ENABLE(); /* Enable clock pins */

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
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