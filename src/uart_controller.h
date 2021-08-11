#pragma once

#include "msg_bus.h"

struct uart_controller_params_t
{
    struct msg_bus_t* msg_queue;
};

void* uart_controller(void*);