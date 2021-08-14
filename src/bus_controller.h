#pragma once

#include <pthread.h>

#include "bus_msg.h"
#include "queue.h"
#include "bus_msg_pool.h"

#define BUS_CONTROLLER_MSGS_CAPACITY 16

typedef struct bus_controller
{
    queue_t*        busy_idxs;
    queue_t*        free_idxs;
    bus_msg_t       msgs[BUS_CONTROLLER_MSGS_CAPACITY];
    bus_msg_pool_t* bus_msg_pool;
    pthread_mutex_t mutex;
    pthread_cond_t  cv;
    volatile int    is_stopped;
} bus_controller_t;

bus_controller_t* bus_controller_init();

int bus_controller_free(bus_controller_t*);

bus_msg_t* bus_controller_pull_free(bus_controller_t*);

void bus_controller_start(bus_controller_t*);

void bus_controller_stop(bus_controller_t*);

bus_msg_t* bus_controller_pull_busy(bus_controller_t*);

void bus_controller_push_free(bus_controller_t*, bus_msg_t const*);

void bus_controller_push_busy(bus_controller_t*, bus_msg_t const*);