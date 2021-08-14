#include "bus_thread.h"

#include <stdlib.h>
#include "bus_container.h"
#include "logger.h"

void* bus_thread_run(void* params)
{
    bus_thread_t* this_ = (bus_thread_t*)params;
    this_->is_stopped = 0;
    while(!this_->is_stopped)
    {
        handler_exec(&this_->handler);
    }
    LM_INF("\\");
    return 0;
}

bus_thread_t* bus_thread_init(handler_t handler)
{
    bus_thread_t* this_ = (bus_thread_t*)malloc(sizeof(bus_thread_t));

    this_->handler      = handler;
    this_->is_stopped   = 1;
    return this_;
}

int bus_thread_free(bus_thread_t* this_)
{
    LM_INF("/");
    free(this_);
    LM_INF("\\");
    return 0;
}

int bus_thread_start(bus_thread_t* this_)
{
    pthread_create(&this_->thread, NULL, bus_thread_run, this_);
    return 0;
}

int bus_thread_stop(bus_thread_t* this_)
{
    LM_INF("/");
    this_->is_stopped = 1;
    pthread_join(this_->thread, NULL);
    LM_INF("\\");
    return 0;
}