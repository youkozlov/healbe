#include "stats_controller.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "msg_idxs.h"
#include "uart_data_ind.h"
#include "bt_stats_ind.h"
#include "wifi_stats_ind.h"
#include "handler.h"
#include "bus_container.h"
#include "logger.h"


void stats_controller_send_bt_stats_ind(stats_controller_t* this_, int32_t avg)
{
    LM_DBG("send_bt_stats_ind");

    bt_stats_ind_t msg;
    msg.average = avg;

    bus_container_send_msg(this_->bus_container, BT_STATS_IND, &msg, sizeof(bt_stats_ind_t));
}

void stats_controller_send_wifi_stats_ind(stats_controller_t* this_, int32_t avg)
{
    LM_DBG("send_wifi_stats_ind");

    wifi_stats_ind_t msg;
    msg.average = avg;

    bus_container_send_msg(this_->bus_container, WIFI_STATS_IND, &msg, sizeof(wifi_stats_ind_t));
}

void stats_controller_receive_uart_data_ind(stats_controller_t* this_, uart_data_ind_t const* msg)
{
    LM_DBG("uart_data_ind received");

    this_->num_rcvd_msg += 1;

    filter_ctx_update(&this_->filter, msg->data, UART_PAYLOAD);

    stats_controller_send_bt_stats_ind(this_, filter_ctx_get_avg(&this_->filter));
    stats_controller_send_wifi_stats_ind(this_, filter_ctx_get_avg(&this_->filter));
}

stats_controller_t* stats_controller_init(bus_container_t* bus_container)
{
    stats_controller_t* this_ = (stats_controller_t*)malloc(sizeof(stats_controller_t));

    this_->bus_container = bus_container;
    filter_ctx_init(&this_->filter);

    bus_container_add_handler(
          bus_container
        , UART_DATA_IND
        , handler_msg(this_, (void(*)(void*, void const*))stats_controller_receive_uart_data_ind)
    );
    return this_;
}

void stats_controller_free(stats_controller_t* this_)
{
    free(this_);
}

void stats_controller_start(stats_controller_t* this_)
{
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}

void stats_controller_stop(stats_controller_t* this_)
{
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}