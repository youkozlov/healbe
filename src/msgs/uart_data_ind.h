#pragma once

#define UART_PAYLOAD 18

typedef struct uart_data_ind
{
    char data[UART_PAYLOAD];
} uart_data_ind_t;
