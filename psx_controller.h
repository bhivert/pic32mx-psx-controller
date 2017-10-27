/* 
 * File:   psx_controller.h
 * Author: bhivert
 *
 * Created on June 20, 2017, 2:13 AM
 */

#ifndef PSX_CONTROLLER_H
# define	PSX_CONTROLLER_H

# include "../cstate-machine/state.h"
# include <stdbool.h>

typedef enum            {
    NONE = -1,
    POLL = 0,
    ENTER_CONF,
    EXIT_CONF,
    SETUP_ANALOG,
    SETUP_PRESSURE,
    SETUP_VIBRATOR
}                       command_id;

typedef struct          command_s {
    unsigned char       header[3];
    unsigned char       params[6];
}                       command_t;

typedef struct          joystick_s {
    unsigned char       rx;
    unsigned char       ry;
    unsigned char       lx;
    unsigned char       ly;
}                       joystick_t;

typedef struct          pressure_s {
    unsigned char       right;
    unsigned char       left;
    unsigned char       up;
    unsigned char       down;
    unsigned char       triangle;
    unsigned char       circle;
    unsigned char       cross;
    unsigned char       square;
    unsigned char       L1;
    unsigned char       R1;
    unsigned char       L2;
    unsigned char       R2;
}                       pressure_t;

typedef struct          values_s {
    unsigned char       digital[2];
    joystick_t          joystick;
    pressure_t          pressure;
}                       values_t;

typedef union           data_u {
    unsigned char       raw[18];
    values_t            values;
}                       data_t;

typedef struct          packet_s {
    unsigned char       size;
    unsigned char       header[3];
    data_t              data;
}                       packet_t;

void                    psx_controller_init(void);
bool                    psx_controller_poll(packet_t *buff);

#endif	/* PSX_CONTROLLER_H */
