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

/* Memory allocation for user data */
__attribute__((__section__("user_data"))) const float userConfig[64];

/* Watchdog handle struct */
IWDG_HandleTypeDef hiwdg;

/* Private function prototypes */
void flashFault(void);

/* Tests flash is set to defaults, if not, it will set them */
void initFlash()
{
	if (userConfig[MAGICNUMBER_LOC] != MAGICNUMBER)
	{
		print("\r\n[ERROR] Flash data invalid, setting defaults");
		userConfigDefaults();
	}
	
	print("\r\n[OK] Flash okay");
}
 
/* Restores default settings for user config */
void userConfigDefaults()
{
	HAL_FLASH_Unlock();

	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[POWERBIAS_LOC], POWERBIAS);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[FRONTSLIP_LOC], FRONTSLIP);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[REARSLIP_LOC], REARSLIP);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[GYROGAIN_LOC], GYROGAIN);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[INVERTSTEERING_LOC], INVERTSTEERING);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[STEERINGTRIM_LOC], STEERINGTRIM);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[WHEELFEEDBACKENABLE_LOC], WHEELFEEDBACKENABLE);
	HAL_FLASH_Program(TYPEPROGRAM_WORD, &userConfig[MAGICNUMBER_LOC], MAGICNUMBER);
	
	HAL_FLASH_Lock();

	if (userConfig[MAGICNUMBER_LOC] != MAGICNUMBER)
	{
		print("\r\n[CRITICAL] Defaults could not be restored. Cannot continue");
		flashFault();
	}

	print("\r\n[OK] Defaults restored");
}

float getConfigFromFlash(int location)
{
	return userConfig[location];
}

void flashFault()
{
	/* We cannot let the platform start with invalid settings */
	/* We must not let the platform fall into a reset loop else we'll kill the flash */
	while (1)
	{
		/* Stay in safemode */
		safeMode();
		HAL_IWDG_Refresh(&hiwdg);
	}
}