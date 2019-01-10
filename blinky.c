#include "common.h"
#include <propeller.h>
#include <stdio.h>

static const uint32_t XI             = 20000000;
static const uint32_t INPUT_DIVIDER  = 4;
static const uint32_t VCO_MULTIPLIER = 72;
static const uint32_t FINAL_DIVIDER  = 2;

void main () {
    int            i;
    const uint32_t CLOCK_FREQ = compute_clock(XI, INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    printf("Attempting to set clock for %d Hz\n", CLOCK_FREQ);

    waitx(CLOCK_FREQ);

    const errot_t err = set_clock_pll(INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    if (err)
        printf("Error! %d\n", err);
    else
        printf("Running at %d\n", CLOCK_FREQ);

    while (1) {
        drive_invert(58);
        waitx(CLOCK_FREQ / 2);
    }
}
