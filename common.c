#include "common.h"
#include <propeller.h>

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

void set_isr (const isr_number_t isrNumber, const isr_t isr) {
    switch (isrNumber) {
        case ISR_1:
            __asm__ volatile ("mov IJMP1, %0" : : "r" (isr));
            break;
        case ISR_2:
            __asm__ volatile ("mov IJMP2, %0" : : "r" (isr));
            break;
        case ISR_3:
            __asm__ volatile ("mov IJMP3, %0" : : "r" (isr));
            break;
    }
}

void set_smartpin_mode (const uint_fast8_t pinNumber, const uint_fast8_t inputSelectorA,
                        const uint_fast8_t inputSelectorB, const uint_fast8_t inputLogicOrFilter,
                        const uint_fast16_t lowLevelControl, const uint_fast8_t dirOutControl,
                        const uint_fast8_t mode) {
    uint32_t value = 0;
    value |= (inputSelectorA & 0b1111) << 28;
    value |= (inputSelectorB & 0b1111) << 24;
    value |= (inputLogicOrFilter & 0b111) << 21;
    value |= (lowLevelControl & 0b1111111111111) << 7;
    value |= (dirOutControl & 0b11) << 5;
    value |= (mode & 0b1111) << 1;
    __asm__ volatile ("wrpin %0, %1" : : "r" (pinNumber), "r" (value));
}
