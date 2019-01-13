#include "common.h"
#include <propeller.h>
#include <stdio.h>

static const uint32_t XI             = 20000000;
static const uint32_t INPUT_DIVIDER  = 1;
static const uint32_t VCO_MULTIPLIER = 4;
static const uint32_t FINAL_DIVIDER  = 1;
uint32_t              CLOCK_FREQ;

ISR(my_fancy_isr) {
    io_asm(drvnot, 56);
    interrupt_return(1);
}

void do_magic () {
    int32_t i;
    while (1) {
        i = 20;
        do {
            drive_invert(58);
            waitx(CLOCK_FREQ / 20);
        } while (--i);
        drive_invert(57);
        trigger_interrupt(1);
    }
}

void main () {
    int i;

    // Wait for things to start
    set_clock_rcfast();
    waitx(RCFAST_FREQ);
    CLOCK_FREQ = compute_clock(XI, INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);
    const error_t err = set_clock_pll(INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    if (err) {
        printf("Error! %d\n", err);
    } else {
        printf("Running at %d Hz\n", CLOCK_FREQ);
        set_isr(1, my_fancy_isr);

        direction_low(59);
        smartpin_control_t mode;
        mode.fields.mode = SPM_TRANSITION_OUTPUT;
        mode.fields.tt = 0b01;
        printf("Setting smart pin mode to 0x%08x\n", mode.raw);
        set_smartpin_mode(59, mode);
        set_smartpin_x_reg(59, UINT16_MAX);
        set_smartpin_y_reg(59, -1);
        direction_high(59);

        do_magic();
    }
}

