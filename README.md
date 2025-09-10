**MAX7219** 
===
This is a library to control the MAX7219 and MAX7221 with a LED matrix, using a STM32f4. 

**UPDATES** 
-------
Support for casscading and for 7-segments LED will be added at a later date.

**INSTALLING** 
-------
Download the Src file and the Inc file, then drag them into their respective folder in your STM32 project.

**INSTRUCTION**
-------
Assuming the user is using STM32cubeIDE or STM32cubeMX. \
### **SETUP** 
- Go to your .ioc file, pick a pin (eg. PA4), and set it to GPIO_Output. \
- Now go to connectivity, click on SPI1, use Full-Duplex Master mode, no hardware NSS signal. \
- In configuration, leave the default option, make sure the baudrate is < 10MBits/s, if not then change your prescaler. 
### **USAGE**
- in main.c, include "STM32_max7219.h".
