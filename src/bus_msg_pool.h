#pragma once

#include "queue.h"

#define BUS_MSG_POOL_ITEM_SIZE 1024

typedef struct bus_msg_pool
{
    queue_t*    idxs;
    uint32_t    capacity;
    uint8_t*    buffers;
} bus_msg_pool_t;

bus_msg_pool_t* bus_msg_pool_init(uint32_t capacity);

int bus_msg_pool_free(bus_msg_pool_t*);

void* bus_msg_pool_pull_buffer(bus_msg_pool_t*, uint32_t*);

void bus_msg_pool_push_buffer(bus_msg_pool_t*, uint32_t);