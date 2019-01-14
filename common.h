#pragma once

#include <stdint.h>
#include <stdbool.h>

extern uint32_t CLOCK_FREQ;

/* COG Registers */

typedef enum {
    REG_ADDR_IJMP3 = 0x1F0,
    REG_ADDR_IRET3,
    REG_ADDR_IJMP2,
    REG_ADDR_IRET2,
    REG_ADDR_IJMP1,
    REG_ADDR_IRET1,
    REG_ADDR_PA,
    REG_ADDR_PB,
    REG_ADDR_PTRA,
    REG_ADDR_PTRB,
    REG_ADDR_DIRA,
    REG_ADDR_DIRB,
    REG_ADDR_OUTA,
    REG_ADDR_OUTB,
    REG_ADDR_INA,
    REG_ADDR_INB
}               cog_register_addr_t;

static volatile uint32_t *const IJMP3 = (volatile uint32_t *const) REG_ADDR_IJMP3;
static volatile uint32_t *const IRET3 = (volatile uint32_t *const) REG_ADDR_IRET3;
static volatile uint32_t *const IJMP2 = (volatile uint32_t *const) REG_ADDR_IJMP2;
static volatile uint32_t *const IRET2 = (volatile uint32_t *const) REG_ADDR_IRET2;
static volatile uint32_t *const IJMP1 = (volatile uint32_t *const) REG_ADDR_IJMP1;
static volatile uint32_t *const IRET1 = (volatile uint32_t *const) REG_ADDR_IRET1;
static volatile uint32_t *const PA    = (volatile uint32_t *const) REG_ADDR_PA;
static volatile uint32_t *const PB    = (volatile uint32_t *const) REG_ADDR_PB;
static volatile uint32_t *const PTRA  = (volatile uint32_t *const) REG_ADDR_PTRA;
static volatile uint32_t *const PTRB  = (volatile uint32_t *const) REG_ADDR_PTRB;
static volatile uint32_t *const DIRA  = (volatile uint32_t *const) REG_ADDR_DIRA;
static volatile uint32_t *const DIRB  = (volatile uint32_t *const) REG_ADDR_DIRB;
static volatile uint32_t *const OUTA  = (volatile uint32_t *const) REG_ADDR_OUTA;
static volatile uint32_t *const OUTB  = (volatile uint32_t *const) REG_ADDR_OUTB;
static volatile uint32_t *const INA   = (volatile uint32_t *const) REG_ADDR_INA;
static volatile uint32_t *const INB   = (volatile uint32_t *const) REG_ADDR_INB;

/* Timing and Sleep functions */

#define getct(destination) __asm__ volatile("getct %0" : "+r" (destination))

#define addct(counterEventNumber, d, s) __asm__ volatile("addct" #counterEventNumber " %0, %1" : "+r" (d) : "r" (s))

#define nop() __asm__ volatile("nop")

static inline void waitx (const uint32_t clockCycles) {
    __asm__ __volatile ("waitx %0" : : "r" (clockCycles));
}

void interruptable_wait (uint32_t clocks);

void interruptable_wait1 (const uint32_t clocks);

void interruptable_wait2 (const uint32_t clocks);

void interruptable_wait3 (const uint32_t clocks);

/* HUB Configuration */

#define RCFAST_FREQ 22600000

typedef enum {
    XI_IGNORED,
    XI_NO_LOADING_CAPS,
    XI_15PF,
    XI_30PF
}                        xi_status_t;

typedef enum {
    CLK_SRC_RC_FAST,
    CLK_SRC_RC_SLOW,
    CLK_SRC_XI,
    CLK_SRC_PLL
}                        clock_source_t;

typedef enum {
    NO_ERROR,
    INVALID_INPUT_DIVIDER,
    INVALID_VCO_MULTIPLIER,
    INVALID_FINAL_DIVIDER
}                        error_t;

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

#define direction_low direction_input

static inline void direction_output (const uint_fast8_t pinNumber) {
    __asm__ volatile ("dirh %[_pinNumber]" : :[_pinNumber] "r"(pinNumber));
}

#define direction_high direction_output

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

/* Event & Interrupt Configuration */

typedef enum {
    INT_1 = 1,
    INT_2,
    INT_3
}                        interrupt_number_t;

typedef enum {
    SEL_EVT_1 = 1,
    SEL_EVT_2,
    SEL_EVT_3,
    SEL_EVT_4
}                        sel_evt_number_t;

typedef enum {
    EVT_SRC_OFF,
    EVT_SRC_CT_PASSED_CT1,
    EVT_SRC_CT_PASSED_CT2,
    EVT_SRC_CT_PASSED_CT3,
    EVT_SRC_SE1,
    EVT_SRC_SE2,
    EVT_SRC_SE3,
    EVT_SRC_SE4,
    EVT_SRC_PIN_PATTERN,
    EVT_SRC_HUB_FIFO_WRAP_RELOAD,
    EVT_SRC_STREAMER_READY,
    EVT_SRC_STREAMER_OUT,
    EVT_SRC_STREAMER_NCO_ROLL,
    EVT_SRC_STREAMER_READ_1FF,
    EVT_SRC_ATN_REQ,
    EVT_SRC_CORDIC
}                        event_source_t;

typedef enum {
    SESP_COG_READ_LUT       = 0b000000000,
    SESP_COG_WRITE_LUT      = 0b000000100,
    SESP_COMP_COG_READ_LUT  = 0b000001000,
    SESP_COMP_COG_WRITE_LUT = 0b000001100,
    SESP_LOCK_RISES         = 0b000010000,
    SESP_LOCK_FALLS         = 0b000100000,
    SESP_LOCK_CHANGES       = 0b000110000,
    SESP_IN_RISES           = 0b001000000,
    SESP_IN_FALLS           = 0b010000000,
    SESP_IN_CHANGES         = 0b011000000,
    SESP_IN_LOW             = 0b100000000,
    SESP_IN_HIGH            = 0b110000000,
}                        sel_evt_src_prefix_t;

typedef void (*isr_t) (void);

#define ISR(name) __attribute__ ((naked)) void isr_ ## name (void)

#define interrupt_return(isrNumber) __asm__ volatile ("reti" #isrNumber)

#define interrupt_resume(isrNumber) __asm__ volatile ("resi" #isrNumber)

#define set_isr(interruptNumber, isr) __asm__ volatile ("mov ijmp" #interruptNumber ", ##_isr_" #isr)

#define get_isr(destination, interruptNumber) __asm__ volatile ("mov %0, ijmp" #interruptNumber : "+r" (destination))

#define assign_int_event_src(interruptNumber, interruptSource) \
    __asm__ volatile ("setint" #interruptNumber " %0" : : "I" (interruptSource))

#define trigger_interrupt(isrNumber) __asm__ volatile ("trgint" #isrNumber)

#define push_frame() __asm__ volatile ("call #__PUSH_FRAME")

#define pop_frame() __asm__ volatile ("call #__POP_FRAME")

/* Smart Pin Functions */

typedef enum {
    SPM_OFF,
    SPM_DAC_NOISE,
    SPM_DAC_PRNG_DITHER,
    SPM_DAC_PWM_DITHER,
    SPM_PULSE_CYCLE_OUTPUT,
    SPM_TRANSITION_OUTPUT,
    SPM_NCO_FREQUENCY,
    SPM_NCO_DUTY,
    SPM_PWM_TRIAGLE,
    SPM_PWM_SAWTOOTH,
    SPM_PWM_SWITCH_MODE_PWR,
    SPM_QUAD_ENCODER,
    SPM_WTF1,
    SPM_WTF2,
    SPM_TIME_A_STATES  = 0b10000,
    SPM_TIME_A_HIGH_STATES,
    SPM_TIME_X_A,
    SPM_SYNC_SERIAL_RX = 0b11101,
    SPM_ASYNC_SERIAL_TX,
    SPM_ASYNC_SERIAL_RX,
}            smartpin_mode_t;

typedef union {
    struct {
        unsigned int
                :1;
        unsigned int mode
                             :5;
        unsigned int tt
                             :2;
        unsigned int lowLevelControl
                             :13;
        unsigned int filter
                             :3;
        unsigned int inputSelectorB
                             :4;
        unsigned int inputSelectorA
                             :4;
    } __attribute__ ((packed)) fields;
    uint32_t                   raw;
}            smartpin_control_t;

static inline void set_smartpin_mode (const uint_fast8_t pinNumber, const smartpin_control_t mode) {
    __asm__ volatile ("wrpin %0, %1" : : "r" (mode.raw), "r" (pinNumber));
}

static inline void set_smartpin_x_reg (const uint_fast8_t pinNumber, const uint32_t value) {
    __asm__ volatile ("wxpin %0, %1" : : "r"(value), "r"(pinNumber));
}

static inline void set_smartpin_y_reg (const uint_fast8_t pinNumber, const uint32_t value) {
    __asm__ volatile ("wypin %0, %1" : : "r"(value), "r"(pinNumber));
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
