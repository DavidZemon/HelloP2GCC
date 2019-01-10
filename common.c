#include "common.h"
#include <propeller.h>

void waitx (const uint32_t clockCycles) {
    __asm__ __volatile ("waitx %0" : : "r" (clockCycles));
}

static errot_t set_clock_mode (const bool enablePll, uint32_t inputDivider, uint32_t vcoMultiplier,
                               uint32_t finalDivider, const xi_status_t xiStatus, const clock_source_t clockSource) {
    __asm__ __volatile("hubset #0");

    uint32_t configuration = 0;

    if (enablePll) {
        configuration = 1 << 24;
    }

    if (64 >= inputDivider) {
        --inputDivider;
        inputDivider &= 0b11111;
        configuration |= inputDivider << 18;
    } else {
        return INVALID_INPUT_DIVIDER;
    }

    if (1024 >= vcoMultiplier) {
        --vcoMultiplier;
        vcoMultiplier &= 0b1111111111;
        configuration |= vcoMultiplier << 8;
    } else {
        return INVALID_VCO_MULTIPLIER;
    }

    if ((finalDivider % 2 && 1 != finalDivider) || 30 < finalDivider) {
        return INVALID_FINAL_DIVIDER;
    } else {
        finalDivider = (finalDivider >> 1) + 15;
        configuration |= (finalDivider & 0b1111) << 4;
    }

    configuration |= xiStatus << 2;

    // enable crystal+PLL, stay in 20MHz+ mode
    __asm__ __volatile("hubset %[_configuration]" : :[_configuration] "r"(configuration));

    // wait ~10ms for crystal+PLL to stabilize
    waitx(RCFAST_FREQ / 100);

    // now switch to PLL
    configuration |= clockSource;
    __asm__ __volatile("hubset %[_configuration]" : :[_configuration] "r"(configuration));

    return NO_ERROR;
}

errot_t set_clock_pll (uint32_t inputDivider, uint32_t vcoMultiplier, uint32_t finalDivider) {
    return set_clock_mode(true, inputDivider, vcoMultiplier, finalDivider, XI_15PF, CLK_SRC_PLL);
}

uint32_t compute_clock (const uint32_t xi, const uint32_t inputDivider, const uint32_t vcoMultiplier,
                        const uint32_t finalDivider) {
    const uint32_t frequency = xi * vcoMultiplier / inputDivider;
    if (finalDivider) {
        return frequency / finalDivider;
    } else {
        return frequency;
    }
}

void set_clock_rcfast () {
    set_clock_mode(false, 0, 0, 0, XI_IGNORED, CLK_SRC_RC_FAST);
}

void set_clock_rcslow () {
    set_clock_mode(false, 0, 0, 0, XI_IGNORED, CLK_SRC_RC_SLOW);
}

void set_clock_xi (const xi_status_t xiStatus) {
    set_clock_mode(false, 0, 0, 0, XI_IGNORED, CLK_SRC_XI);
}

void reboot () {
    __asm__ __volatile__ ("hubset %0" : : "r" (0x10000000));
}

void direction_input (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_output (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_not_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_not_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_random (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void direction_invert (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("dirnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_low (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_high (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_not_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_not_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_random (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void output_invert (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("outnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_low (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_high (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("flth %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_not_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_not_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_random (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void float_invert (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("fltnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_low (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_high (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_not_c (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_not_z (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_random (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

void drive_invert (const uint_fast8_t pinNumber) {
    __asm__ __volatile__ ("drvnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}
