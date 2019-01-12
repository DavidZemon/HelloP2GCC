#include "common.h"
#include <propeller.h>
#include <stdio.h>

static const uint32_t XI             = 20000000;
static const uint32_t INPUT_DIVIDER  = 1;
static const uint32_t VCO_MULTIPLIER = 4;
static const uint32_t FINAL_DIVIDER  = 1;
uint32_t              CLOCK_FREQ;

void do_magic ();

ISR(my_fancy_isr);

void main () {
    int i;

    // Wait for things to start
    waitx(RCFAST_FREQ);
    CLOCK_FREQ = compute_clock(XI, INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);
    printf("Attempting to set clock for %d Hz\n", CLOCK_FREQ);

    const error_t err = set_clock_pll(INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    if (err) {
        printf("Error! %d\n", err);
    } else {
        printf("Running at %d Hz\n", CLOCK_FREQ);

        set_isr(1, my_fancy_isr);
        assign_int_event_src(1, EVT_SRC_SE1);
        do_magic();
    }
}

void do_magic () {
    int32_t i;
    while (1) {
        for (i = 0; i < 20; ++i) {
            drive_invert(58);
            waitx(CLOCK_FREQ / 20);
        }
        trigger_interrupt(1);
    }
}

ISR(my_fancy_isr) {
    io_asm(drvnot, 57);
    interrupt_return(1);
}
