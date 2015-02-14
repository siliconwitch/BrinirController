/*
* Brief:       Here are defined all the CLI commands and their strings
*
* Copyright (C) 2015 Rajesh Nakarja
* http://www.naklojik.com
*
* This software is licenced under the MIT Licence. For full terms visit:
* http://opensource.org/licenses/MIT
*/

#ifndef __CLICOMMANDS_H
#define __CLICOMMANDS_H

	// Max string size to allocate for
	#define MAXCLISTRING 32

	// Basic commands
	#define CMD_HELP "help"
	#define CMD_PRINT_CONFIG "print config"
	#define CMD_REBOOT "reboot"

	// Controller related commands
	#define CMD_DEFAULTS "defaults"
	#define CMD_SET_POWERBIAS "power bias"
	#define CMD_SET_FRONTSLIP "front slip"
	#define CMD_SET_REARSLIP "rear slip"
	#define CMD_SET_GYROGAIN "gyro gain"
	#define CMD_SET_STEERINGTRIM "steering trim"
	#define CMD_INVERT_STEERING "invert steering"
	#define CMD_WHEELFEEDBACK "wheel feedback"

#endif // !__CLICOMMANDS_H
