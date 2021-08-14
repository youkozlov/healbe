#include "wifi_controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "msg_idxs.h"
#include "wifi_stats_ind.h"
#include "handler.h"
#include "bus_container.h"
#include "logger.h"

void wifi_controller_receive_wifi_stats_ind(wifi_controller_t* this_, wifi_stats_ind_t const* msg)
{
    LM_DBG("uart_data_ind received(avg=%d)", msg->average);

    this_->num_rcvd_msg += 1;
}

wifi_controller_t* wifi_controller_init(bus_container_t* bus_container)
{
    wifi_controller_t* this_ = (wifi_controller_t*)malloc(sizeof(wifi_controller_t));

    this_->bus_container = bus_container;

    bus_container_add_handler(
          bus_container
        , WIFI_STATS_IND
        , handler_msg(this_, (void(*)(void*, void const*))wifi_controller_receive_wifi_stats_ind)
    );
    return this_;
}

void wifi_controller_free(wifi_controller_t* this_)
{
    free(this_);
}

void wifi_controller_start(wifi_controller_t* this_)
{
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}

void wifi_controller_stop(wifi_controller_t* this_)
{
    LM_INF("num_rcvd_msg %u", this_->num_rcvd_msg);
}