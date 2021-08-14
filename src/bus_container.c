#include "bus_container.h"

#include <stdlib.h>
#include <string.h>
#include "logger.h"


bus_container_t* bus_container_init(uint16_t num_threads)
{
    LM_DBG("/");
    bus_container_t* this_ = (bus_container_t*)malloc(sizeof(bus_container_t));

    this_->bus_controller   = bus_controller_init();
    this_->num_bus_threads  = num_threads;
    for (size_t i = 0; i < num_threads; i += 1)
    {
        this_->bus_threads[i] = bus_thread_init(
            handler_get(this_, (void(*)(void*)) bus_container_receive_msg)
        );
    }

    this_->is_stopped = 1;
    LM_DBG("\\");
    return this_;
}

void bus_container_free(bus_container_t* this_)
{
    LM_DBG("/");
    bus_controller_free(this_->bus_controller);

    for (size_t i = 0; i < this_->num_bus_threads; i += 1)
    {
        bus_thread_free(this_->bus_threads[i]);
    }

    free(this_);
    LM_DBG("\\");
}

void bus_container_add_handler(bus_container_t* this_, uint32_t msg_id, handler_msg_t handler)
{
    if (msg_id >= BUS_CONTAINER_MAX_NUM_HANDLERS)
    {
        return;
    }
    this_->bus_container_handlers[msg_id] = handler;
}

void bus_container_start(bus_container_t* this_)
{
    this_->is_stopped = 0;
    bus_controller_start(this_->bus_controller);
    for (size_t i = 0; i < this_->num_bus_threads; i += 1)
    {
        bus_thread_start(this_->bus_threads[i]);
    }
}

void bus_container_stop(bus_container_t* this_)
{
    this_->is_stopped = 1;
    for (size_t i = 0; i < this_->num_bus_threads; i += 1)
    {
        bus_controller_stop(this_->bus_controller);
        bus_thread_stop(this_->bus_threads[i]);
    }
}

void bus_container_send_msg(bus_container_t* this_, uint32_t msg_id, void const* payload, uint32_t size)
{
    if (this_->is_stopped)
    {
        return;
    }
    bus_msg_t* bus_msg = bus_controller_pull_free(this_->bus_controller);
    if (!bus_msg)
    {
        LM_ERR("Can not send msg");
        return;
    }
    bus_msg->msg_id = msg_id;
    memcpy(bus_msg->payload, payload, size);
    bus_msg->size = size;
    bus_controller_push_busy(this_->bus_controller, bus_msg);
}

void bus_container_receive_msg(bus_container_t* this_)
{
    uint8_t tmp_buf[1024];

    if (this_->is_stopped)
    {
        return;
    }
    bus_msg_t* bus_msg = bus_controller_pull_busy(this_->bus_controller);
    if (!bus_msg)
    {
        return;
    }
    memcpy(tmp_buf, bus_msg->payload, bus_msg->size);
    bus_controller_push_free(this_->bus_controller, bus_msg);
    handler_exec_msg(&this_->bus_container_handlers[bus_msg->msg_id], tmp_buf);
}