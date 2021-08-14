#pragma once

#include "bus_container.h"
#include "uart_data_ind.h"
#include "simple_filter.h"

typedef struct stats_controller
{
    bus_container_t*  bus_container;
    filter_ctx_t      filter;
    uint32_t          num_rcvd_msg;
} stats_controller_t;

stats_controller_t* stats_controller_init(bus_container_t*);
void stats_controller_free(stats_controller_t*);
void stats_controller_start(stats_controller_t*);
void stats_controller_stop(stats_controller_t*);