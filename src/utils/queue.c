#include "queue.h"

#include <stdlib.h>
#include "logger.h"

queue_t* queue_init(uint32_t capacity)
{
    queue_t* this_ = (queue_t*)malloc(sizeof(queue_t));
    this_->begin = 0;
    this_->end = 0;
    this_->size = 0;
    this_->capacity = capacity;
    this_->idxs = (uint32_t*)malloc(this_->capacity * sizeof(uint32_t));
    return this_;
}

int queue_free(queue_t* this_)
{
    free(this_->idxs);
    free(this_);
    return 0;
}

uint32_t queue_size(queue_t const* this_)
{
    return this_->size;
}

int queue_empty(queue_t const* this_)
{
    return this_->size == 0;
}

int queue_full(queue_t const* this_)
{
    return this_->size == this_->capacity;
}

void queue_push(queue_t* this_, uint32_t idx)
{
    this_->idxs[this_->end] = idx;
    this_->end = (this_->end + 1) % this_->capacity;
    this_->size += 1;
}

void queue_pop(queue_t* this_)
{
    this_->begin = (this_->begin + 1) % MSG_CAPACITY;
    this_->size -= 1;
}

uint32_t queue_front(queue_t const* this_)
{
    return this_->idxs[this_->begin];
}
