#pragma once

#include <stdint.h>

#include "bus_thread.h"

typedef struct bus_timer
{
    uint32_t      period;
    handler_t     handler;
    bus_thread_t* bus_thread;
} bus_timer_t;

bus_timer_t* bus_timer_init(uint32_t, handler_t);
void bus_timer_free(bus_timer_t*);
void bus_timer_start(bus_timer_t*);
void bus_timer_stop(bus_timer_t*);
