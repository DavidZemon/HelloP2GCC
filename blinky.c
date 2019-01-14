#include "uart.h"
#include "common.h"
#include <propeller.h>
#include <stdio.h>

static const uint32_t XI             = 20000000;
static const uint32_t INPUT_DIVIDER  = 1;
static const uint32_t VCO_MULTIPLIER = 4;
static const uint32_t FINAL_DIVIDER  = 1;
uint32_t              CLOCK_FREQ;

void do_magic ();

ISR(my_fancy_isr) {
    push_frame();
    io_asm(drvnot, 56);
    pop_frame();
    interrupt_return(1);
}

void main () {
    int i;

    // Set the clock
    CLOCK_FREQ = compute_clock(XI, INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);
    const error_t err = set_clock_pll(INPUT_DIVIDER, VCO_MULTIPLIER, FINAL_DIVIDER);

    // Configure UART
    uart_tx_t uart;
    uart.interruptNumber       = INT_1;
    uart.selectableEventNumber = 1;
    uart.pinNumber             = 24;
    uart_configure(&uart, 115200, 8);

    // Configure smart pin wave
    direction_low(59);
    smartpin_control_t mode;
    mode.fields.mode = SPM_TRANSITION_OUTPUT;
    mode.fields.tt   = 0b01;
    set_smartpin_mode(59, mode);
    set_smartpin_x_reg(59, UINT16_MAX);
    direction_high(59);
    set_smartpin_y_reg(59, 0);

    // Configure manual interrupt
    set_isr(1, my_fancy_isr);

    if (err) {
        printf("Error! %d\n", err);
    } else {
        printf("Running at %d Hz\n", CLOCK_FREQ);
        do_magic();
    }
}

void do_magic () {
    int32_t i;
    while (1) {
        i = 20;
        do {
            drive_invert(58);
            interruptable_wait1(CLOCK_FREQ / 20);
        } while (--i);
        drive_invert(57);
        trigger_interrupt(1);
    }
}
