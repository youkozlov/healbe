#include "uart_controller.h"

#include <unistd.h>
#include <stdlib.h>
#include "logger.h"
#include "msg_idxs.h"
#include "uart_data_ind.h"

#define TICK_TIME_MS 10

int receive_from_uart(char* data)
{
    for (int i = 0; i < UART_PAYLOAD; i += 1)
    {
        data[i] = rand();
    }
    return 0;
}

void send_uart_data_ind(uart_controller_t* this_, uart_data_ind_t* msg)
{
    LM_DBG("send_uart_data_ind");

    bus_container_send_msg(this_->bus_container, UART_DATA_IND, msg, sizeof(uart_data_ind_t));
}

void bus_timer_tick(uart_controller_t* this_)
{
    LM_DBG("bus_timer_tick");

    this_->num_rcvd_msg += 1;

    uart_data_ind_t ind;
    int result = receive_from_uart(ind.data);
    if (result)
    {
        LM_ERR("receive_from_uart return (%d)", result);
        return;
    }
    send_uart_data_ind(this_, &ind);
}

uart_controller_t* uart_controller_init(bus_container_t* bus_container)
{
    uart_controller_t* this_ = (uart_controller_t*)malloc(sizeof(uart_controller_t));

    this_->bus_container = bus_container;
    this_->bus_timer = bus_timer_init(TICK_TIME_MS, handler_get(this_, (void(*)(void*))bus_timer_tick));
    return this_;
}

void uart_controller_free(uart_controller_t* this_)
{
    bus_timer_free(this_->bus_timer);
    free(this_);
}

void uart_controller_start(uart_controller_t* this_)
{
    bus_timer_start(this_->bus_timer);
}

void uart_controller_stop(uart_controller_t* this_)
{
    bus_timer_stop(this_->bus_timer);
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}