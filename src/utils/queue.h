#pragma once

#include "stdint.h"
#include "defs.h"

struct queue_t
{
    uint8_t begin;
    uint8_t end;
    uint8_t size;
    uint8_t idxs[MSG_CAPACITY];
};

void queue_init(struct queue_t*);

int queue_size(struct queue_t const*);

int queue_empty(struct queue_t const*);

int queue_full(struct queue_t const*);

void queue_push(struct queue_t*, uint8_t);

void queue_pop(struct queue_t*);

int queue_front(struct queue_t const*);