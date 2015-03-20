# BrinirController
=====================

Brinir is an embedded control platform for RC scale model cars, trucks and buggys. It features:

- Independent four wheel drive control (PPM)
- Independent four way suspension control (PPM)
- Two servo outputs (PPM)
- Two radio inputs (PPM)
- Four analogue inputs (Suspension throw)
- Four digital inputs (Wheel tacho speed)
- Command line configuration over any form of UART
- Bootloading over UART
- Model based control from Matlab/Simulink

Overview Video: Coming soon...

The firmware currently runs on an STM32F405RG device but can be scaled to run on any STM32 processor depending on processing power required.

The code is built using Visual Studio 2013 Community edition and VisualGDB

 Useful links
--------------

Visual Studio 2013 Community (Free): https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx

VisualGDB (Free trial) (~£70 1yr licence) (~£35 Students): http://visualgdb.com/


Building
--------------

Once you have VS and VisualGDB installed. Create a new embedded project from the new project menu and follow the setup and install:

GNU GCC Compiler for ARM
STM32 support packages
STLink v2 debugger drivers

This can all be done from within the new project menus. There is also an option to test the settings. If you have trouble, use the tutorial on the VisualGDB website.

Once the tools are setup. The Brinir project file, "BrinirController.sln" can be launched and built. It should build and load onto the STM32 without any changes.


Handy tools if not using Visual GDB:
--------------

GNU GCC C Complier for ARM: https://launchpad.net/gcc-arm-embedded

ST-Link v2 tool: http://www.st.com/web/en/catalog/tools/FM146/CL1984/SC724/SS1677/PF251168

GNU GDB debugging guide for stm32: http://www.emb4fun.de/archive/stlink/index.html


Control System
--------------
The control system for the project is designed in Simulink and can be exported as C code using the embedded coder module. Granted this is the only part of the project that requires a non free software, the exported code however is still in readable C. Useful resource: http://www.mathworks.co.uk/hardware-support/arm-cortex-m-cmsis.html and http://www.mathworks.co.uk/hardware-support/st-discovery-board.html

Copyright
---------

Copyright (C) 2015 Rajesh Nakarja. All rights reserved
http://www.naklojik.com

Licenced under the MIT licence