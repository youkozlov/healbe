#include "msg_bus.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

void msg_bus_init(struct msg_bus_t* bus, size_t msg_size)
{
    queue_init(&bus->busy_idxs);
    queue_init(&bus->free_idxs);
    bus->mempool = malloc(msg_size * MSG_CAPACITY);
    if (!bus->mempool)
    {
        LM_ERR("Can not allocate memory");
        exit(1);
    }
    void* ptr = bus->mempool;
    for (int i = 0; i < MSG_CAPACITY; i += 1)
    {
        bus->msgs[i].id = i;
        bus->msgs[i].payload = ptr;
        ptr += msg_size;
        queue_push(&bus->free_idxs, i);
    }
    if (pthread_mutex_init(&bus->mutex, NULL))
    {
        LM_ERR("Can not init mutex");
        exit(1);
    }
    if (pthread_cond_init(&bus->cv, NULL))
    {
        LM_ERR("Can not init cv");
        exit(1);
    }
}

void msg_bus_free(struct msg_bus_t* bus)
{
    free(bus->mempool);
    pthread_cond_destroy(&bus->cv);
    pthread_mutex_destroy(&bus->mutex);
}

struct msg_t* msg_bus_pull_free(struct msg_bus_t* bus)
{
    if (pthread_mutex_lock(&bus->mutex))
    {
        LM_ERR("Can not lock mutex");
        return 0;
    }
    if (queue_empty(&bus->free_idxs))
    {
        if (pthread_mutex_unlock(&bus->mutex))
        {
            LM_ERR("pthread_mutex_unlock return error");
        }
        return 0;
    }
    int id = queue_front(&bus->free_idxs);
    queue_pop(&bus->free_idxs);
    if (pthread_mutex_unlock(&bus->mutex))
    {
        LM_ERR("pthread_mutex_unlock return error");
    }
    return &bus->msgs[id];
}

struct msg_t* msg_bus_pull_busy(struct msg_bus_t* bus)
{
    if (pthread_mutex_lock(&bus->mutex))
    {
        LM_ERR("Can not lock mutex");
        return 0;
    }
    while (!bus->cv || queue_empty(&bus->busy_idxs))
    {
        pthread_cond_wait(&bus->cv, &bus->mutex);
    }
    if (queue_empty(&bus->busy_idxs))
    {
        if (pthread_mutex_unlock(&bus->mutex))
        {
            LM_ERR("pthread_mutex_unlock return error");
        }
        return 0;
    }
    int id = queue_front(&bus->busy_idxs);
    queue_pop(&bus->busy_idxs);
    if (pthread_mutex_unlock(&bus->mutex))
    {
        LM_ERR("pthread_mutex_unlock return error");
    }
    return &bus->msgs[id];
}

void msg_bus_push_free(struct msg_bus_t* bus, struct msg_t* msg)
{
    if (pthread_mutex_lock(&bus->mutex))
    {
        LM_ERR("Can not lock mutex");
        return;
    }
    if (queue_full(&bus->free_idxs))
    {
        pthread_mutex_unlock(&bus->mutex);
        LM_ERR("Queue is full");
        return;
    }
    queue_push(&bus->free_idxs, msg->id);
    pthread_mutex_unlock(&bus->mutex);
}

void msg_bus_push_busy(struct msg_bus_t* bus, struct msg_t* msg)
{
    if (pthread_mutex_lock(&bus->mutex))
    {
        LM_ERR("Can not lock mutex");
        return;
    }
    if (queue_full(&bus->busy_idxs))
    {
        pthread_mutex_unlock(&bus->mutex);
        LM_ERR("Queue is full");
        return;
    }
    queue_push(&bus->busy_idxs, msg->id);
    if (pthread_cond_signal(&bus->cv))
    {
        LM_ERR("pthread_cond_signal return error");
    }
    pthread_mutex_unlock(&bus->mutex);
}