#include "common.h"
#include <propeller.h>

void interruptable_wait (const uint32_t clocks) {
    uint32_t ct;
    getct(ct);
    const uint32_t until = ct + clocks;
    while (abs(until - ct) > 2) getct(ct);
}

void interruptable_wait1 (const uint32_t clocks) {
    uint32_t timer;
    __asm__ volatile (
    "           getct %[_timer]                         \n\t"
    "           addct1 %[_timer], %[_clocks]            \n\t"
    "interruptable_wait1_start%=:                       \n\t"
    "           jnct1 #interruptable_wait1_start%=:     \n\t"
    :[_timer] "+r"(timer)
    :[_clocks] "r"(clocks)
    );
}

void interruptable_wait2 (const uint32_t clocks) {
    uint32_t timer;
    __asm__ volatile (
    "           getct %[_timer]                         \n\t"
    "           addct2 %[_timer], %[_clocks]            \n\t"
    "interruptable_wait2_start%=:                       \n\t"
    "           jnct2 #interruptable_wait2_start%=:     \n\t"
    :[_timer] "+r"(timer)
    :[_clocks] "r"(clocks)
    );
}

void interruptable_wait3 (const uint32_t clocks) {
    uint32_t timer;
    __asm__ volatile (
    "           getct %[_timer]                         \n\t"
    "           addct3 %[_timer], %[_clocks]            \n\t"
    "interruptable_wait3_start%=:                       \n\t"
    "           jnct3 #interruptable_wait3_start%=:     \n\t"
    :[_timer] "+r"(timer)
    :[_clocks] "r"(clocks)
    );
}

error_t set_clock_mode (const bool enablePll, uint32_t inputDivider, uint32_t vcoMultiplier, uint32_t finalDivider,
                        const xi_status_t xiStatus, const clock_source_t clockSource) {
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
