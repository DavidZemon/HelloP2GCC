#include "uart.h"
#include "common.h"

uart_tx_t *uart1;
uart_tx_t *uart2;
uart_tx_t *uart3;

ISR(uart_tx_isr2) {
    push_frame();
    if (uart2->head != uart2->tail) {
        set_smartpin_y_reg(uart2->pinNumber, *uart2->head);
        ++uart2->head;
        if (uart2->head == &uart2->buffer[UART_BUFFER_SIZE]) {
            uart2->head = &uart2->buffer[0];
        }
    }
    pop_frame();
    interrupt_return(2);
}

void uart_configure (uart_tx_t * const uart, const uint32_t baudRate, const uint8_t wordSize) {
    direction_low(uart->pinNumber);

    // Initialize the UART struct
    uart->head = uart->tail = uart->buffer;

    // Set the smart pin mode
    smartpin_control_t mode;
    mode.fields.mode = SPM_ASYNC_SERIAL_TX;
    mode.fields.tt = 0b01;
    set_smartpin_mode(uart->pinNumber, mode);

    // Configure baud rate
    const uint16_t baudClockTicks = CLOCK_FREQ / baudRate;
    set_smartpin_x_reg(uart->pinNumber, (baudClockTicks << 16) | ((wordSize - 1) & 0b11111));

    // Assign the ISR and interrupt event
    switch (uart->interruptNumber) {
        case INT_1:
            set_isr(1, uart_tx_isr1);
            assign_int_event_src(1, EVT_SRC_SE1);
            break;
        case INT_2:
            set_isr(2, uart_tx_isr2);
            assign_int_event_src(2, EVT_SRC_SE1);
            break;
        case INT_3:
            set_isr(3, uart_tx_isr3);
            assign_int_event_src(3, EVT_SRC_SE1);
            break;
    }

    direction_high(uart->pinNumber);
}

void uart1_put(uart_tx_t * const uart, const uint32_t word) {

    set_smartpin_y_reg(uart1->pinNumber, *uart1->head);
    ++uart1->head;
    if (uart1->head == &uart1->buffer[UART_BUFFER_SIZE]) {
        uart1->head = &uart1->buffer[0];
    }
}
