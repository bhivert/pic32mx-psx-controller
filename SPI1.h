/* 
 * File:   SPI1.h
 * Author: bhivert
 *
 * Created on June 19, 2017, 3:00 AM
 */

#ifndef SPI1_H
# define SPI1_H

void            SPI1_init(void);
unsigned char   SPI1_Transmit(unsigned char c);

#endif	/* SPI1_H */

/*
1. Disable the SPI interrupts in the respective IECx register.
2. Stop and reset the SPI module by clearing the ON bit.
3. Clear the receive buffer.
4. Clear the ENHBUF bit (SPIxCON<16>) if using Standard Buffer mode or set the bit if using
Enhanced Buffer mode.
5. If SPI interrupts are not going to be used, skip this step and continue to step 5. Otherwise
the following additional steps are performed:
a) Clear the SPIx interrupt flags/events in the respective IFSx register.
b) Write the SPIx interrupt priority and subpriority bits in the respective IPCx register.
c) Set the SPIx interrupt enable bits in the respective IECx register.
6. Write the Baud Rate register, SPIxBRG.
7. Clear the SPIROV bit (SPIxSTAT<6>).
8. Write the desired settings to the SPIxCON register with MSTEN (SPIxCON<5>) = 1.
9. Enable SPI operation by setting the ON bit (SPIxCON<15>).
10. Write the data to be transmitted to the SPIxBUF register. Transmission (and reception) will
start as soon as data is written to the SPIxBUF register.
*/