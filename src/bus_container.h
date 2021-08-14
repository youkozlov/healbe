#pragma once

#include "bus_msg.h"
#include "bus_controller.h"
#include "bus_thread.h"

#define BUS_CONTAINER_MAX_NUM_HANDLERS 16
#define BUS_CONTAINER_MAX_NUM_THREADS 16

typedef struct bus_container
{
    bus_controller_t*   bus_controller;
    handler_msg_t       bus_container_handlers[BUS_CONTAINER_MAX_NUM_HANDLERS];
    bus_thread_t*       bus_threads[BUS_CONTAINER_MAX_NUM_THREADS];
    uint16_t            num_bus_threads;
    volatile int        is_stopped;
} bus_container_t;

bus_container_t* bus_container_init(uint16_t num_threads);
void bus_container_free(bus_container_t*);
void bus_container_add_handler(bus_container_t*, uint32_t msg_id, handler_msg_t);
void bus_container_start(bus_container_t*);
void bus_container_stop(bus_container_t*);
void bus_container_send_msg(bus_container_t*, uint32_t msg_id, void const* payload, uint32_t size);
void bus_container_receive_msg(bus_container_t*);