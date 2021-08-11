#pragma once

#include <pthread.h>
#include "defs.h"
#include "queue.h"

struct msg_t
{
    uint8_t id;
    void*   payload;
};

struct msg_bus_t
{
    struct queue_t  busy_idxs;
    struct queue_t  free_idxs;
    struct msg_t    msgs[MSG_CAPACITY];
    pthread_mutex_t mutex;
    pthread_cond_t  cv;
    void*           mempool;
};

void msg_bus_init(struct msg_bus_t*, size_t);

void msg_bus_free(struct msg_bus_t*);

struct msg_t* msg_bus_pull_free(struct msg_bus_t*);

struct msg_t* msg_bus_pull_busy(struct msg_bus_t*);

void msg_bus_push_free(struct msg_bus_t*, struct msg_t*);

void msg_bus_push_busy(struct msg_bus_t*, struct msg_t*);