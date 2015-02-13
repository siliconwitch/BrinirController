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
	void initFlash(void);
	void userConfigDefaults(void);
	float getConfigFromFlash(int location);


#endif