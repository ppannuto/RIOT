/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     xtimer_examples
 * @{
 *
 * @file
 * @brief       example application for setting a periodic wakeup
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 *
 * @}
 */

//#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "board.h"
#include "periph_conf.h"

#include <fxos8700.h>
#include <tmp006.h>
#include "periph/i2c.h"
#include "periph/gpio.h"

/* set interval to 1 second */
#define INTERVAL (2U * US_PER_SEC)

/* Whoo! RIOT's finally worth something! */
//fxos8700_t fxos8700;
// Nope. Nevermind.

// Tryin again
//tmp006_t tmp006;
// And another bucket of nope.


void kill_all_sensors(void) {
    // We have a Franken-Hamilton:
    // https://github.com/hamilton-mote/hw/issues/2
    // Okay, or maybe a 3C?

    // Generic init's
    i2c_acquire(I2C_0);
    i2c_init_master(I2C_0, I2C_SPEED_NORMAL);



    // 1 | Low-cost temperature sensor | AT30TS74 |
    // top left

    // "16 bit" reg is actually only 8, conf reg addr 1
    // in conf reg, shutdown is LSB & active high
    i2c_write_reg(I2C_0, 0x49, 0x1, 0x01);



    // 4 | Light sensor | APDS-9007 |
    // top right ish
    gpio_init(GPIO_PIN(0, 28), GPIO_OUT);
    gpio_write(GPIO_PIN(0, 28), 1); // high -> shutdown



    // 8 | High-precision 3-axis accel/magnetometer | FXOS8700 |
    // left of the SAMR21
    /*
    fxos8700_params_t fxos8700_params = {
        .i2c = I2C_0,
        .addr = 0x1E,
    };
    fxos8700_init(&fxos8700, &fxos8700_params);
    */
    // https://www.nxp.com/files-static/sensors/doc/data_sheet/FXOS8700CQ.pdf
    // 13.4, p26
    i2c_write_reg(I2C_0, 0x1E, 0x2A, 0x00);
    // Sigh, after all this, I've convinced myself that this too
    // starts in standby mode, p31: default value of 0x2A is 0x00



    // 10 | Temp / Humidity sensor | HDC1080 |
    // On right edge of board, isolated
    // http://www.ti.com/lit/ds/symlink/hdc1080.pdf
    // 8.4, p9: power-on -> sleep mode automatically



    // 20 | Temp / Radiant sensor | TMP006 |
    // http://www.ti.com/lit/ds/symlink/tmp006.pdf
    // 8.5.3, p20
    uint8_t reg[2];
    i2c_read_regs(I2C_0, 0x44, 0x02, reg, 2);
      // clear bits 14:12
    reg[0] &= ~0x70;
    i2c_write_regs(I2C_0, 0x44, 0x02, reg, 2);




    i2c_release(I2C_0);
}

int main(void)
{
    kill_all_sensors();

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while(1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        //printf("slept until %" PRIu32 "\n", xtimer_usec_from_ticks(xtimer_now()));
        LED_TOGGLE;
    }

    return 0;
}
