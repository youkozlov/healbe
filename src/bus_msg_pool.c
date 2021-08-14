#include "bus_msg_pool.h"

#include <stdlib.h>
#include "logger.h"

bus_msg_pool_t* bus_msg_pool_init(uint32_t capacity)
{
    bus_msg_pool_t* this_ = (bus_msg_pool_t*)malloc(sizeof(bus_msg_pool_t));

    this_->capacity = capacity;
    this_->idxs = queue_init(this_->capacity);
    this_->buffers = (uint8_t*)malloc(this_->capacity * BUS_MSG_POOL_ITEM_SIZE);
    for (uint32_t i = 0; i < this_->capacity; i += 1)
    {
        queue_push(this_->idxs, i);
    }
    return this_;
}

int bus_msg_pool_free(bus_msg_pool_t* this_)
{
    LM_DBG("/");
    queue_free(this_->idxs);
    free(this_->buffers);
    free(this_);
    LM_DBG("\\");
    return 0;
}

void* bus_msg_pool_pull_buffer(bus_msg_pool_t* this_, uint32_t* id)
{
    if (queue_empty(this_->idxs))
    {
        return 0;
    }
    *id = queue_front(this_->idxs);
    queue_pop(this_->idxs);
    return this_->buffers + *id * BUS_MSG_POOL_ITEM_SIZE;
}

void bus_msg_pool_push_buffer(bus_msg_pool_t* this_, uint32_t id)
{
    queue_push(this_->idxs, id);
}