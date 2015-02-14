/*
* Brief:    
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
#include "prototypes.h"
#include "config.h"

/* Watchdog handle struct */
IWDG_HandleTypeDef hiwdg;

/* Globals */
ControllerConfigStruct ControllerConfig;

/* Memory allocation for user data */
__attribute__((__section__(".user_data"))) const int32_t userConfig[32];

/* Private function prototypes */
void hardFlashFault(void);

/* Tests flash */
uint8_t validateFlash(void)
{
	if (userConfig[magicNumber] != MAGICNUMBER)
	{
		print("\r\n[ERROR] Flash data is invalid");
		return 0;
	}
	
	print("\r\n[OK] Flash okay");
	return 1;
}
 
/* Writes settings to flash for user config */
void writeToFlash()
{
	HAL_FLASH_Unlock();

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
	FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
	
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[powerBias], ControllerConfig.powerBias);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[frontSlip], ControllerConfig.frontSlip);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[rearSlip], ControllerConfig.rearSlip);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[gyroGain], ControllerConfig.gyroGain);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[steeringTrim], ControllerConfig.steeringTrim);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[invertSteering], ControllerConfig.invertSteering);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[enableWheelSpeedFeedback], ControllerConfig.enableWheelSpeedFeedback);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[magicNumber], ControllerConfig.magicNumber);
	
	HAL_FLASH_Lock();

	if (userConfig[magicNumber] != MAGICNUMBER) hardFlashFault();
}

/* If the expected magic number wasn't read back 
   correctly, that means theres a fault with the
   flash operation. Don't start the controller
   as a result and sit in this loop
*/
void hardFlashFault()
{
	/* We cannot let the platform start with invalid settings */
	print("\r\n[CRITICAL] Defaults could not be restored. Cannot continue");

	/* We must not let the platform fall into a reset loop else we'll kill the flash */
	while (1)
	{
		/* Stay in safemode */
		safeMode();
		#ifdef WATCHDOG_EN
			HAL_IWDG_Refresh(&hiwdg);
		#endif
	}
}