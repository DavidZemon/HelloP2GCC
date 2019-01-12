#include <stdint.h>
#include <stdbool.h>

/* Sleep functions */

static inline void waitx (const uint32_t clockCycles) {
    __asm__ __volatile ("waitx %0" : : "r" (clockCycles));
}

/* HUB Configuration */

#define RCFAST_FREQ 22600000

typedef enum {
    XI_IGNORED,
    XI_NO_LOADING_CAPS,
    XI_15PF,
    XI_30PF
} xi_status_t;

typedef enum {
    CLK_SRC_RC_FAST,
    CLK_SRC_RC_SLOW,
    CLK_SRC_XI,
    CLK_SRC_PLL
} clock_source_t;

typedef enum {
    NO_ERROR,
    INVALID_INPUT_DIVIDER,
    INVALID_VCO_MULTIPLIER,
    INVALID_FINAL_DIVIDER
} error_t;

error_t set_clock_mode (const bool enablePll, uint32_t inputDivider, uint32_t vcoMultiplier, uint32_t finalDivider,
                        const xi_status_t xiStatus, const clock_source_t clockSource);

/**
 * @brief       Set the clock based on the external crystal with the PLL
 *
 * Resulting clock frequency will be XI * vcoMultiplier / (inputDivider * finalDivider)
 *
 * @param[in]   inputDivider
 * @param[in]   vcoMultiplier
 * @param[in]   finalDivider
 *
 * @return      0 upon success, error code otherwise
 */
static inline error_t set_clock_pll (uint32_t inputDivider, uint32_t vcoMultiplier, uint32_t finalDivider) {
    return set_clock_mode(true, inputDivider, vcoMultiplier, finalDivider, XI_15PF, CLK_SRC_PLL);
}

static inline uint32_t compute_clock (const uint32_t xi, const uint32_t inputDivider,
                                      const uint32_t vcoMultiplier, const uint32_t finalDivider) {
    return xi * vcoMultiplier / inputDivider / finalDivider;
}

static inline void set_clock_rcfast () {
    set_clock_mode(false, 0, 0, 0, XI_IGNORED, CLK_SRC_RC_FAST);
}

static inline void set_clock_rcslow () {
    set_clock_mode(false, 0, 0, 0, XI_IGNORED, CLK_SRC_RC_SLOW);
}

static inline void set_clock_xi (const xi_status_t xiStatus) {
    set_clock_mode(false, 0, 0, 0, XI_IGNORED, CLK_SRC_XI);
}

static inline void reboot () {
    __asm__ volatile ("hubset %0" : : "r" (0x10000000));
}

/* Basic pin I/O functions */

#define io_asm(instruction, pinNumber) __asm__ __volatile__ (#instruction " #" #pinNumber)

static inline void direction_input (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_output (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_not_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_not_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_random (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void direction_invert (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_low (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_high (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_not_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_not_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_random (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void output_invert (const uint_fast8_t pinNumber) {
    __asm__ volatile ("outnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_low (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_high (const uint_fast8_t pinNumber) {
    __asm__ volatile ("flth %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_not_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_not_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_random (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void float_invert (const uint_fast8_t pinNumber) {
    __asm__ volatile ("fltnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_low (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvl %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_high (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_not_c (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvnc %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_not_z (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvnz %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_random (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvrnd %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

static inline void drive_invert (const uint_fast8_t pinNumber) {
    __asm__ volatile ("drvnot %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

/* Interrupt Configuration */

typedef enum {
    ISR_1,
    ISR_2,
    ISR_3
} isr_number_t;

typedef void (*isr_t) (void);

void set_isr (const isr_number_t isrNumber, const isr_t isr);

/* Smart Pin Functions */

void set_smartpin_raw_mode (const uint_fast8_t pinNumber, const uint32_t mode);

static inline void set_smartpin_x_reg (const uint_fast8_t pinNumber, const uint32_t value) {
    __asm__ volatile ("wxpin %0, %1" : : "r"(pinNumber), "r"(value));
}

static inline void set_smartpin_y_reg (const uint_fast8_t pinNumber, const uint32_t value) {
    __asm__ volatile ("wypin %0, %1" : : "r"(pinNumber), "r"(value));
}

static inline uint32_t read_smartpin (const uint_fast8_t pinNumber) {
    uint32_t result;
    __asm__ volatile ("rdpin %0, %1" : "+r"(result) : "r"(pinNumber));
    return result;
}

static inline uint32_t read_smartpin_quiet (const uint_fast8_t pinNumber) {
    uint32_t result;
    __asm__ volatile ("rqpin %0, %1" : "+r"(result) : "r"(pinNumber));
    return result;
}

static inline void ack_smartpin (const uint_fast8_t pinNumber) {
    __asm__ volatile ("akpin %0" : : "r"(pinNumber));
}
