#pragma once

#define UART_PAYLOAD 18

struct uart_data_ind_t
{
    char data[UART_PAYLOAD];
};
