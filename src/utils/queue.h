#pragma once

#include "stdint.h"
#include "defs.h"

typedef struct queue
{
    uint32_t    begin;
    uint32_t    end;
    uint32_t    size;
    uint32_t    capacity;
    uint32_t*   idxs;
} queue_t;

queue_t* queue_init(uint32_t capacity);

int queue_free(queue_t*);

uint32_t queue_size(queue_t const*);

int queue_empty(queue_t const*);

int queue_full(queue_t const*);

void queue_push(queue_t*, uint32_t);

void queue_pop(queue_t*);

uint32_t queue_front(queue_t const*);