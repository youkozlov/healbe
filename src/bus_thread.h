#pragma once

#include <pthread.h>
#include "bus_msg.h"
#include "handler.h"

typedef void(*function)(void*);

typedef struct bus_thread
{
    handler_t           handler;
    pthread_t           thread;
    volatile int        is_stopped;
} bus_thread_t;

bus_thread_t* bus_thread_init(handler_t);
int bus_thread_free(bus_thread_t*);
int bus_thread_start(bus_thread_t*);
int bus_thread_stop(bus_thread_t*);