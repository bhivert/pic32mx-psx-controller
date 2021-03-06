/* 
 * File:   SPI1.c
 * Author: bhivert
 *
 * Created on June 19, 2017, 2:19 AM
 */

#include <xc.h>

void            SPI1_init(void) {
    unsigned char   c;
    
    SPI1CON = 0;                                    //  SPI1CON Clear / Stop
    SPI1BRG = 0x3;
    TRISF |= (1<<2);                                //  SDI1 set as intput for pull up set
    c = SPI1BUF;                                    //  Clear SPI1BUF
    SPI1CON = (1<<4) | (1<<5) | (1<<6) | (1<<15);   //  SPI1 Setup / Start
}

unsigned char   SPI1_Transmit(unsigned char c) {
    SPI1BUF = c;                            //  Write c in SPI1BUF
    while (SPI1STAT & (1<<11))              //  Wait while is busy
        ;
    return SPI1BUF;                         //  Read SPI1BUF
}
