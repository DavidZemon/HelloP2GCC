#include "common.h"
#include <propeller.h>
#include <stdio.h>

static const uint32_t XI             = 20000000;
static const uint32_t INPUT_DIVIDER  = 1;
static const uint32_t VCO_MULTIPLIER = 4;
static const uint32_t FINAL_DIVIDER  = 1;
uint32_t              CLOCK_FREQ;

void do_magic ();

__attribute__ ((naked)) void my_fancy_isr (void);

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

        set_isr(ISR_1, &my_fancy_isr);
        do_magic();
    }
}

void do_magic () {
    while (1) {
        drive_invert(58);
        waitx(CLOCK_FREQ / 20);
    }
}

__attribute__ ((naked)) void my_fancy_isr (void) {
    drive_invert(58);
}
