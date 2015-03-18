 /*
 * Brief:     Manages all the interrupts into the system from external pins
 *			  i.e. all signals from RC radio and hall effect sensors. Outputs 
 *			  to motor drivers (ESCs) and servo
 *
 *			  Decodes PPM signals from RC radio with TIM2 and EXTI0, 1, 6, 7
 *			  Hall effect sensors used are the AH3761.
 *
 * Uses:      Uses PORT A pins 0 - 7, TIM2, TIM3, TIM4, TIM10, TIM11, TIM13, TIM14, EXTI 0 - 7
 *
 * Datasheet: http://diodes.com/datasheets/AH3761.pdf
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
RCRadio volatile RCRadioStructure = {0,0,0,0};
WheelRPM volatile WheelRPMStructure = {0,0,0,0};
PPMOutputs volatile PPMOutputStructure = {0,0,0,0,0,0,0,0};

/* Private variables */
int pulseIndex = 0;

/* Private helper function declartions */
double CalculateWheelRPM(uint32_t Count);	/* Calculates the RPM of the wheels from times between feedback pulses */
double normaliseSignal(uint32_t input);		/* Turns the Timer count value of the input, into a -1 to 1 float */
uint32_t denormaliseSignal(double input);	/* Turns the float speed value back into a timer count value */

/* Private struct declarations */
TIM_HandleTypeDef htim2;					/* PPM length timer for radio input */
TIM_HandleTypeDef htim3;					/* PPM length timer for motor outputs */
TIM_HandleTypeDef htim4;					/* Radio input signal lost timeout timer */
TIM_HandleTypeDef htim10;					/* wheel feedback count timer */
TIM_HandleTypeDef htim11;					/* wheel feedback count timer */
TIM_HandleTypeDef htim13;					/* wheel feedback count timer */
TIM_HandleTypeDef htim14;					/* wheel feedback count timer */
GPIO_InitTypeDef GPIO_InitStruct;
TIM_ClockConfigTypeDef sClockSourceConfig;
TIM_MasterConfigTypeDef sMasterConfig;

void initIO()
{
	/* Init GPIO and EXTI interrupts */
	HAL_GPIO_MspInit();
	
	/* TIM2 Init - PPM Inputs */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = PPMMEASUREPRESCALER;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 65535;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim2);
	
	/* TIM3 Init - PPM Outputs */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = PPMMEASUREPRESCALER;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim3);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim3);

	/* TIM4 Init - Lost signal timer */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = PPMTIMEOUTPRESCALER;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = PPMTIMEOUTVALUE;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim4);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

	HAL_TIM_Base_Start_IT(&htim4);
		
	/* Wheel speed feedback timers */

	/* TIM10 Init */
	htim10.Instance = TIM10;
	htim10.Init.Prescaler = 1600;
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = 10000;
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim10);
	HAL_TIM_Base_Start_IT(&htim10);
	
	/* TIM11 Init */
	htim11.Instance = TIM11;
	htim11.Init.Prescaler = 1600;
	htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim11.Init.Period = 10000;
	htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim11);
	HAL_TIM_Base_Start_IT(&htim11);
	
	/* TIM13 Init */
	htim13.Instance = TIM13;
	htim13.Init.Prescaler = 840;
	htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim13.Init.Period = 10000;
	htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim13);
	HAL_TIM_Base_Start_IT(&htim13);
	
	/* TIM14 Init */
	htim14.Instance = TIM14;
	htim14.Init.Prescaler = 840;
	htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim14.Init.Period = 10000;
	htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim14);
	HAL_TIM_Base_Start_IT(&htim14);
	
	/* Print OK */
	print("\r\n[OK] IO and timers ready");
}

/* EXTI Interrupts */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
		case REC1PIN: /* Steering pin */
		    if(HAL_GPIO_ReadPin(RECPORT, REC1PIN) == GPIO_PIN_RESET){    RCRadioStructure.steering = normaliseSignal(TIM2->CNT);   }  
    		TIM2->CNT = 0;
    		TIM4->CNT = 0; /* Reset Lost signal timer */
    		break;

    	case REC2PIN: /* Throttle pin */
			if(HAL_GPIO_ReadPin(RECPORT, REC2PIN) == GPIO_PIN_RESET){    RCRadioStructure.throttle = normaliseSignal(TIM2->CNT);   }
   			TIM2->CNT = 0;
			break;

		case FB1PIN: /* Feedback pin */
			WheelRPMStructure.FL = CalculateWheelRPM(TIM10->CNT);
			TIM10->CNT = 0;
			break;
		
		case FB2PIN: /* Feedback pin */
			WheelRPMStructure.FR = CalculateWheelRPM(TIM11->CNT);
			TIM11->CNT = 0;
			break;

		case FB3PIN: /* Feedback pin */
			WheelRPMStructure.BL = CalculateWheelRPM(TIM13->CNT);
			TIM13->CNT = 0;
			break;

		case FB4PIN: /* Feedback pin */
			WheelRPMStructure.BR = CalculateWheelRPM(TIM14->CNT);
			TIM14->CNT = 0;
			break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM4){ /* Timer for detecting lost signal, overflows if steering signal lost */
		RCRadioStructure.steering = 0;
		RCRadioStructure.throttle = 0;
	}

	if (htim->Instance == TIM10){ /* Timer for zeroing feedback pin */
		WheelRPMStructure.FL = 0;
	}

	if (htim->Instance == TIM11){ /* Timer for zeroing feedback pin */
		WheelRPMStructure.FR = 0;
	}

	if (htim->Instance == TIM13){ /* Timer for zeroing feedback pin */
		WheelRPMStructure.BL = 0;
	}

	if (htim->Instance == TIM14){ /* Timer for zeroing feedback pin */
		WheelRPMStructure.BR = 0;
	}

	if (htim->Instance == TIM3){ /* Timer outputing PPM signals to motors & servos */
		switch(pulseIndex){
		    case 0:
		        TIM3->ARR = denormaliseSignal(PPMOutputStructure.MOT1);
		        HAL_GPIO_WritePin(MOTORPORT, AUX4PIN, GPIO_PIN_RESET);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR1PIN, GPIO_PIN_SET);
		        pulseIndex++;
		        break;

		    case 1:
		        TIM3->ARR = denormaliseSignal(PPMOutputStructure.MOT2);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR1PIN, GPIO_PIN_RESET);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR2PIN, GPIO_PIN_SET);
		        pulseIndex++;
		        break;

		    case 2:
		        TIM3->ARR = denormaliseSignal(PPMOutputStructure.MOT3);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR2PIN, GPIO_PIN_RESET);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR3PIN, GPIO_PIN_SET);
		        pulseIndex++;
		        break;

		    case 3:
		        TIM3->ARR = denormaliseSignal(PPMOutputStructure.MOT4);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR3PIN, GPIO_PIN_RESET);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR4PIN, GPIO_PIN_SET);
		        pulseIndex++;
		        break;

		    case 4:
		        TIM3->ARR = denormaliseSignal(PPMOutputStructure.SER1);
		        HAL_GPIO_WritePin(MOTORPORT, MOTOR4PIN, GPIO_PIN_RESET);
		        HAL_GPIO_WritePin(MOTORPORT, SERVO1PIN, GPIO_PIN_SET);
		        pulseIndex++;
		        break;

			case 5:
				TIM3->ARR = denormaliseSignal(PPMOutputStructure.SER2);
				HAL_GPIO_WritePin(MOTORPORT, SERVO1PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MOTORPORT, SERVO2PIN, GPIO_PIN_SET);
				pulseIndex++;
				break;

			case 6:
				TIM3->ARR = denormaliseSignal(PPMOutputStructure.AUX1);
				HAL_GPIO_WritePin(MOTORPORT, SERVO2PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MOTORPORT, AUX1PIN, GPIO_PIN_SET);
				pulseIndex++;
				break;

			case 7:
				TIM3->ARR = denormaliseSignal(PPMOutputStructure.AUX2);
				HAL_GPIO_WritePin(MOTORPORT, AUX1PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MOTORPORT, AUX2PIN, GPIO_PIN_SET);
				pulseIndex++;
				break;

			case 8:
				TIM3->ARR = denormaliseSignal(PPMOutputStructure.AUX3);
				HAL_GPIO_WritePin(MOTORPORT, AUX2PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MOTORPORT, AUX3PIN, GPIO_PIN_SET);
				pulseIndex++;
				break;

			case 9:
				TIM3->ARR = denormaliseSignal(PPMOutputStructure.AUX4);
				HAL_GPIO_WritePin(MOTORPORT, AUX3PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(MOTORPORT, AUX4PIN, GPIO_PIN_SET);
				pulseIndex = 0;
				break;
		    }
	}
}

double CalculateWheelRPM(uint32_t Count)
{
    return 60.0 / ((Count/100000.0) * MOTORPOLES);
}

double normaliseSignal(uint32_t input){
    if(input > RECEIVERMINSIGNAL && input < RECEIVERMAXSIGNAL){
        return (((double)input-RECEIVERMINSIGNAL) / ((RECEIVERMAXSIGNAL-RECEIVERMINSIGNAL)/2)) - 1;
    }

    else{
        return 0.0;
    }
}

uint32_t denormaliseSignal(double input)
{
    if(input >  1.0){input =  1.0;}
    if(input < -1.0){input = -1.0;}
    return (uint32_t) (( (input+1) * ((RECEIVERMAXSIGNAL-RECEIVERMINSIGNAL)/2) ) + RECEIVERMINSIGNAL);
}

/* Halt the vehicle */
void safeMode(void)
{
    PPMOutputStructure.MOT1 = 0;
    PPMOutputStructure.MOT2 = 0;
    PPMOutputStructure.MOT3 = 0;
    PPMOutputStructure.MOT4 = 0;
	PPMOutputStructure.SER1 = 0;
	PPMOutputStructure.SER2 = 0;
	PPMOutputStructure.AUX1 = 0;
	PPMOutputStructure.AUX2 = 0;
	PPMOutputStructure.AUX3 = 0;
	PPMOutputStructure.AUX4 = 0;
}