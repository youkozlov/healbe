#include "bus_controller.h"

#include <stdlib.h>
#include "logger.h"

int mutex_lock(bus_controller_t* this_)
{
    if (pthread_mutex_lock(&this_->mutex))
    {
        LM_ERR("pthread_mutex_lock return error");
    }
    return 0;
}

int mutex_unlock(bus_controller_t* this_)
{
    if (pthread_mutex_unlock(&this_->mutex))
    {
        LM_ERR("pthread_mutex_unlock return error");
    }
    return 0;
}

int cond_signal(bus_controller_t* this_)
{
    if (pthread_cond_signal(&this_->cv))
    {
        LM_ERR("pthread_cond_signal return error");
    }
    return 0;
}

int cond_signal_broadcast(bus_controller_t* this_)
{
    if (pthread_cond_broadcast(&this_->cv))
    {
        LM_ERR("pthread_cond_signal return error");
    }
    return 0;
}

bus_controller_t* bus_controller_init()
{
    bus_controller_t* this_ = (bus_controller_t*)malloc(sizeof(bus_controller_t));

    this_->busy_idxs = queue_init(MSG_CAPACITY);
    this_->free_idxs = queue_init(MSG_CAPACITY);
    this_->bus_msg_pool = bus_msg_pool_init(MSG_CAPACITY);
    for (int i = 0; i < MSG_CAPACITY; i += 1)
    {
        this_->msgs[i].queue_id = i;
        queue_push(this_->free_idxs, i);
    }
    if (pthread_mutex_init(&this_->mutex, NULL))
    {
        LM_ERR("Can not init mutex");
        exit(1);
    }
    if (pthread_cond_init(&this_->cv, NULL))
    {
        LM_ERR("Can not init cv");
        exit(1);
    }
    this_->is_stopped = 1;
    return this_;
}

int bus_controller_free(bus_controller_t* this_)
{
    LM_DBG("/");
    pthread_cond_destroy(&this_->cv);
    pthread_mutex_destroy(&this_->mutex);
    queue_free(this_->free_idxs);
    queue_free(this_->busy_idxs);
    bus_msg_pool_free(this_->bus_msg_pool);
    free(this_);
    LM_DBG("\\");
    return 0;
}

void bus_controller_start(bus_controller_t* this_)
{
    this_->is_stopped = 0;
}

void bus_controller_stop(bus_controller_t* this_)
{
    this_->is_stopped = 1;
    cond_signal_broadcast(this_);
}

bus_msg_t* bus_controller_pull_free(bus_controller_t* this_)
{
    mutex_lock(this_);
    if (queue_empty(this_->free_idxs))
    {
        mutex_unlock(this_);
        return 0;
    }
    uint32_t const bus_id = queue_front(this_->free_idxs);
    queue_pop(this_->free_idxs);
    bus_msg_t* msg = &this_->msgs[bus_id];
    msg->payload = bus_msg_pool_pull_buffer(this_->bus_msg_pool, &msg->pool_id);
    if (!msg->payload)
    {
        mutex_unlock(this_);
        return 0;
    }
    mutex_unlock(this_);
    return msg;
}

bus_msg_t* bus_controller_pull_busy(bus_controller_t* this_)
{
    mutex_lock(this_);
    while ((!this_->is_stopped) && (!this_->cv || queue_empty(this_->busy_idxs)))
    {
        pthread_cond_wait(&this_->cv, &this_->mutex);
    }
    if (this_->is_stopped)
    {
        mutex_unlock(this_);
        return 0;
    }
    int const id = queue_front(this_->busy_idxs);
    queue_pop(this_->busy_idxs);
    mutex_unlock(this_);
    return &this_->msgs[id];
}

void bus_controller_push_free(bus_controller_t* this_, bus_msg_t const* msg)
{
    mutex_lock(this_);
    bus_msg_pool_push_buffer(this_->bus_msg_pool, msg->pool_id);
    queue_push(this_->free_idxs, msg->queue_id);
    mutex_unlock(this_);
}

void bus_controller_push_busy(bus_controller_t* this_, bus_msg_t const* msg)
{
    mutex_lock(this_);
    queue_push(this_->busy_idxs, msg->queue_id);
    cond_signal(this_);
    mutex_unlock(this_);
}