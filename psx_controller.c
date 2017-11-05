/* 
 * File:   psx_controller.c
 * Author: bhivert
 *
 * Created on June 20, 2017, 2:13 AM
 */

#include "SPI1.h"
#include "psx_controller.h"
#include <xc.h>

static packet_t             _buff;
static unsigned int         _inc;

static id_t                 _start(id_t id) {
    LATB &= ~(1<<2);            //  Low #SS1
    _inc = 0;
    _buff.size = 0;
    IFS0 &= ~(1<<8);            //  Clear timer 2 interrupt flag
    T2CON |= (1<<15);           //  Timer 2 ON
    return ++id;
}

static command_t            *_active_command;

unsigned char               revb(unsigned char b) {
    return    (b & 0x01) << 7 \
            | (b & 0x02) << 5 \
            | (b & 0x04) << 3 \
            | (b & 0x08) << 1 \
            | (b & 0x10) >> 1 \
            | (b & 0x20) >> 3 \
            | (b & 0x40) >> 5 \
            | (b & 0x80) >> 7;
}

static id_t                 _send_receive(id_t id) {
    if (_buff.size < sizeof(_buff.header)) {
        _buff.header[_buff.size] = revb(SPI1_Transmit(revb(_active_command->header[_buff.size])));
        ++_buff.size;
        return ++id;
    }
    _buff.data.raw[_inc] = revb(SPI1_Transmit((_inc < sizeof(_active_command->params)) ? revb(_active_command->params[_inc]) : 0x00));
    ++_inc;
    ++_buff.size;
    return ++id;
}

static id_t                 _sleep(id_t id) {
    if (_buff.size < sizeof(_buff.header) || _inc < (_buff.header[1] & 0xF) << 1) {
        return --id;
    }
    return ++id;
}

static id_t                 _stop(id_t id) {
    LATB |= (1<<2);             //  High #SS1
    T2CON = 0;                  //  Clear / Stop timer 2
    return ++id;
}

static state_fct_t          *_fcts[] = {
    &_start,
    &_send_receive,
    &_sleep,
    &_stop
};

static state_t              _psx_poll_state;

void                        __attribute__((vector(_TIMER_2_VECTOR), interrupt(IPL7AUTO))) _TIMER2_HANDLER(void) {
    IFS0 &= ~(1<<8);            //  Clear timer 2 interrupt flag
    state_exec(&_psx_poll_state);
}

void                        psx_controller_init(void) {
    state_init(&_psx_poll_state, _fcts, sizeof(_fcts) / sizeof(state_fct_t *));
    SPI1_init();

    // SPI1 set for psx controller
    TRISB &= ~(1<<2);           //  #SS1 set as output
    LATB |= (1<<2);             //  Hight #SS1
    
    // Timer 2 Setting
    T2CON = 0;                  //  Clear / Stop timer 2
    TMR2 = 0;                   //  Set timer counter
    PR2 = 0x2;                  //  Set timer overflow
    IPC2 |= ((0x7<<2) | 0x3);   //  Priority 7 / Sub priority 3
    IEC0 |= (1<<8);             //  Enable timer 2 interruption
}

static command_t            _psx_command[] = {
    {{0x01, 0x42, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, // poll
    {{0x01, 0x43, 0x00}, {0x01, 0x00, 0x00, 0x00, 0x00, 0x00}}, // enter configuration
    {{0x01, 0x43, 0x00}, {0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A}}, // exit configuration
    {{0x01, 0x44, 0x00}, {0x01, 0x03, 0x00, 0x00, 0x00, 0x00}}, // setup analog
    {{0x01, 0x4F, 0x00}, {0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00}}, // setup pressure values
    {{0x01, 0x4D, 0x00}, {0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF}}  // setup vibrator
};

bool                        psx_controller_poll(packet_t *buff) {
    if (state_isstopped(_psx_poll_state)) {
        _active_command = &_psx_command[POLL];
        state_reset(&_psx_poll_state);
        *buff = _buff;
        state_exec(&_psx_poll_state);
        return true;
    }
    return false;
}
