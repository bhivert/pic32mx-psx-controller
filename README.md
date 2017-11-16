Play Station Analog Controller (Ps1&2) firmware designed with/for PIC32MX340F512H MCU  
  
Precaution of use:  
When controller where initialised or polled, SPI 1 interface and Timer 2 are used.  
  
Requirements:  
\#include "../cstate-machine/state.h"  
<https://github.com/bhivert/cstate-machine>

The module is set for about 30Hz polling and 125hz spi baudrate

\#pragma config FNOSC =	PRIPLL  
\#pragma config FSOSCEN =	ON  
\#pragma config POSCMOD =	XT  
\#pragma config OSCIOFNC =	ON  
\#pragma config FPBDIV =	DIV_4  

\#pragma config FPLLIDIV =	DIV_2  
\#pragma config FPLLMUL =	MUL_16  
\#pragma config FPLLODIV =	DIV_16  
\#pragma config UPLLEN =	ON
