#include <stdint.h>
#include <stdbool.h>

/* Sleep functions */

void waitx (const uint32_t clockCycles);

/* HUB Configuration */

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
} errot_t;

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
errot_t set_clock_pll (uint32_t inputDivider, uint32_t vcoMultiplier, uint32_t finalDivider);

uint32_t compute_clock(const uint32_t xi, const uint32_t inputDivider, const uint32_t vcoMultiplier, const uint32_t finalDivider);

void set_clock_rcfast ();

void set_clock_rcslow ();

void set_clock_xi(const xi_status_t xiStatus);

void reboot ();

/* Pin I/O functions */

#define io_asm(instruction, pinNumber) __asm__ __volatile__ (#instruction " #" #pinNumber : :)

void direction_input (const uint_fast8_t pinNumber);

void direction_output (const uint_fast8_t pinNumber);

void direction_c (const uint_fast8_t pinNumber);

void direction_not_c (const uint_fast8_t pinNumber);

void direction_z (const uint_fast8_t pinNumber);

void direction_not_z (const uint_fast8_t pinNumber);

void direction_random (const uint_fast8_t pinNumber);

void direction_invert (const uint_fast8_t pinNumber);

void output_low (const uint_fast8_t pinNumber);

void output_high (const uint_fast8_t pinNumber);

void output_c (const uint_fast8_t pinNumber);

void output_not_c (const uint_fast8_t pinNumber);

void output_z (const uint_fast8_t pinNumber);

void output_not_z (const uint_fast8_t pinNumber);

void output_random (const uint_fast8_t pinNumber);

void output_invert (const uint_fast8_t pinNumber);

void float_low (const uint_fast8_t pinNumber);

void float_high (const uint_fast8_t pinNumber);

void float_c (const uint_fast8_t pinNumber);

void float_not_c (const uint_fast8_t pinNumber);

void float_z (const uint_fast8_t pinNumber);

void float_not_z (const uint_fast8_t pinNumber);

void float_random (const uint_fast8_t pinNumber);

void float_invert (const uint_fast8_t pinNumber);

void drive_output (const uint_fast8_t pinNumber);

void drive_c (const uint_fast8_t pinNumber);

void drive_not_c (const uint_fast8_t pinNumber);

void drive_z (const uint_fast8_t pinNumber);

void drive_not_z (const uint_fast8_t pinNumber);

void drive_random (const uint_fast8_t pinNumber);

void drive_invert (const uint_fast8_t pinNumber);
