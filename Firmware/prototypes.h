 /*
 * Brief:       List of all the public functions within the project
 *
 *				Public function prototypes for all the .c files within the
 *				/application/source folder. Public variables can be found
 *				in the config.h file.
 *
 * Copyright (C) 2015 Rajesh Nakarja
 * http://www.naklojik.com
 *
 * This software is licenced under the MIT Licence. For full terms visit:
 * http://opensource.org/licenses/MIT
 */

#ifndef __OUTPUTS_H
#define __OUTPUTS_H

	typedef enum { powerBias, frontSlip, rearSlip, gyroGain, steeringTrim, invertSteering, enableWheelSpeedFeedback, magicNumber } ControllerConfigEnum;

/* main.c function prototypes */
	void setControllerConfig(ControllerConfigEnum param, int32_t value);
	void applyControllerConfig(void);
	void loadControllerConfig(void);
	void setControllerDefaults(void);

/* analogue.c Function prototypes */ 
	void initADC(void);

/* imu.c Function prototypes */
	void initIMU(void);
	void IMUGetMotion(void);
	void IMUConfig(int accelrange, int gyrorange);

/* io.c Function prototypes */
	void initIO(void);
	void safeMode(void);

/* serial.c Function prototypes */
	void initSerial();
	void print(char string[]);

/* flash.c Function prototypes */
	uint8_t validateFlash(void);
	void writeToFlash(void);

#endif