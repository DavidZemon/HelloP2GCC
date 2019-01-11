#include "common.h"
#include <propeller.h>
#include <stdio.h>

static const uint32_t XI             = 20000000;
static const uint32_t INPUT_DIVIDER  = 1;
static const uint32_t VCO_MULTIPLIER = 4;
static const uint32_t FINAL_DIVIDER  = 1;

void main () {
    int            i;
    const uint32_t CLOCK_FREQ = compute_clock(XI, INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    // Wait for things to start
    waitx(RCFAST_FREQ);
    printf("Attempting to set clock for %d Hz\n", CLOCK_FREQ);

    const errot_t err = set_clock_pll(INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    if (err) {
        printf("Error! %d\n", err);
        set_clock_rcfast();
        while (1) {
            switch (err) {
                case INVALID_INPUT_DIVIDER:
                    drive_invert(59);
                    break;
                case INVALID_VCO_MULTIPLIER:
                    drive_invert(60);
                    break;
                case INVALID_FINAL_DIVIDER:
                    drive_invert(61);
                    break;
            }
            waitx(RCFAST_FREQ / 20);
        }
    } else
        printf("Running at %d Hz\n", CLOCK_FREQ);

    while (1) {
        drive_invert(58);
        waitx(CLOCK_FREQ / 20);
    }
}
