#include "bt_controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "msg_idxs.h"
#include "bt_stats_ind.h"
#include "handler.h"
#include "bus_container.h"
#include "logger.h"

void bt_controller_receive_bt_stats_ind(bt_controller_t* this_, bt_stats_ind_t const* msg)
{
    LM_DBG("uart_data_ind received(avg=%d)", msg->average);

    this_->num_rcvd_msg += 1;
}

bt_controller_t* bt_controller_init(bus_container_t* bus_container)
{
    bt_controller_t* this_ = (bt_controller_t*)malloc(sizeof(bt_controller_t));

    this_->bus_container = bus_container;

    bus_container_add_handler(
          bus_container
        , BT_STATS_IND
        , handler_msg(this_, (void(*)(void*, void const*))bt_controller_receive_bt_stats_ind)
    );
    return this_;
}

void bt_controller_free(bt_controller_t* this_)
{
    free(this_);
}

void bt_controller_start(bt_controller_t* this_)
{
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}

void bt_controller_stop(bt_controller_t* this_)
{
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}