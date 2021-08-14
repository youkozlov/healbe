#include "bus_timer.h"

#include <stdlib.h>
#include <unistd.h>

void bus_timer_run(bus_timer_t* this_)
{
    usleep(1000 * this_->period);
    handler_exec(&this_->handler);
}

bus_timer_t* bus_timer_init(uint32_t period, handler_t handler)
{
    bus_timer_t* this_ = (bus_timer_t*)malloc(sizeof(bus_timer_t));

    this_->handler      = handler;
    this_->bus_thread   = bus_thread_init(handler_get(this_, (void(*)(void*)) bus_timer_run));
    this_->period       = period;
    return this_;
}

void bus_timer_free(bus_timer_t* this_)
{
    bus_thread_free(this_->bus_thread);
    free(this_);
}

void bus_timer_start(bus_timer_t* this_)
{
    bus_thread_start(this_->bus_thread);
}

void bus_timer_stop(bus_timer_t* this_)
{
    bus_thread_stop(this_->bus_thread);
}