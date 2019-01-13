#pragma once

#include "common.h"
#include <stdint.h>
#include <stddef.h>

#define UART_BUFFER_SIZE 256

typedef struct {
    interrupt_number_t interruptNumber;
    sel_evt_number_t   selectableEventNumber;
    uint8_t            pinNumber;
    uint32_t           buffer[UART_BUFFER_SIZE];
    uint32_t           *head;
    uint32_t           *tail;
} uart_tx_t;

void uart_configure (uart_tx_t * uart, uint32_t baudRate, uint8_t wordSize);

void uart_put(uart_tx_t * uart, uint32_t word);
