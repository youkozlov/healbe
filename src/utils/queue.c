#include "queue.h"

void queue_init(struct queue_t* queue)
{
    queue->begin = 0;
    queue->end = 0;
    queue->size = 0;
}

int queue_size(struct queue_t const* queue)
{
    return queue->size;
}

int queue_empty(struct queue_t const* queue)
{
    return queue->size == 0;
}

int queue_full(struct queue_t const* queue)
{
    return queue->size == MSG_CAPACITY;
}

void queue_push(struct queue_t* queue, uint8_t idx)
{
    queue->idxs[queue->end] = idx;
    queue->end = (queue->end + 1) % MSG_CAPACITY;
    queue->size += 1;
}

void queue_pop(struct queue_t* queue)
{
    queue->begin = (queue->begin + 1) % MSG_CAPACITY;
    queue->size -= 1;
}

int queue_front(struct queue_t const* queue)
{
    return queue->idxs[queue->begin];
}
