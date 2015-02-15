# BrinirController
=====================

An embedded control system targeted to RC model cars which feature individual wheel drive. This controller allows for software differentials and power delivery with the aid of various sensors on the vehicle. Currently the system makes use of wheel speed sensors and a 6 axis IMU for real time processing.

Overview Video: http://youtu.be/ET1HEyqEQJQ

The controller communicates to a steering servo and four brushless DC motor controllers. It also listens to a standard RC radio receiver for user input, as well as a serial port for telemetry over wireless modules such as xbee or bluetooth.

It's hoped that this project allows for advanced features such as traction control, under/over steer detection, adaptive power delivery and real-time telemetry.

The system runs on Cortex M4 STM32F4 devices by ST microelectronics. To build this project, you will need the ARM-GCC compiler found here:

 Useful links
--------------
ARM GCC C Complier https://launchpad.net/gcc-arm-embedded

As well as the STLink driver for uploading to a development board such as the STM32F4Discovery board:

ST-Link v2 tool: http://www.st.com/web/en/catalog/tools/FM146/CL1984/SC724/SS1677/PF251168

GDB debugging on stm32: http://www.emb4fun.de/archive/stlink/index.html

The project is developed in Visual Studio 2014 Communitity with VisualGDB.

Control System
--------------
The control system for the project is designed in Simulink and can be exported as C code using the embedded coder module. Granted this is the only part of the project that requires a non free software, the exported code however is still in readable C. Useful resource: http://www.mathworks.co.uk/hardware-support/arm-cortex-m-cmsis.html and http://www.mathworks.co.uk/hardware-support/st-discovery-board.html

Copyright
---------

Copyright (C) 2015 Rajesh Nakarja. All rights reserved
http://www.naklojik.com

Licenced under the MIT licence