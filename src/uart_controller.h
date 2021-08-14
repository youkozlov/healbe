#pragma once

#include "bus_container.h"
#include "bus_timer.h"

typedef struct uart_controller
{
    bus_container_t*    bus_container;
    bus_timer_t*        bus_timer;
    uint32_t            num_rcvd_msg;

} uart_controller_t;

uart_controller_t* uart_controller_init(bus_container_t*);
void uart_controller_free(uart_controller_t*);
void uart_controller_start(uart_controller_t*);
void uart_controller_stop(uart_controller_t*);