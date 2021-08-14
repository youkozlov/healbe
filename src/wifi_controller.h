#pragma once

#include "bus_container.h"
#include "wifi_stats_ind.h"

typedef struct wifi_controller
{
    bus_container_t*  bus_container;
    uint32_t          num_rcvd_msg;
} wifi_controller_t;

wifi_controller_t* wifi_controller_init(bus_container_t*);
void wifi_controller_free(wifi_controller_t*);
void wifi_controller_start(wifi_controller_t*);
void wifi_controller_stop(wifi_controller_t*);