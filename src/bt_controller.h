#pragma once

#include "bus_container.h"
#include "bt_stats_ind.h"

typedef struct bt_controller
{
    bus_container_t*  bus_container;
    uint32_t          num_rcvd_msg;
} bt_controller_t;

bt_controller_t* bt_controller_init(bus_container_t*);
void bt_controller_free(bt_controller_t*);
void bt_controller_start(bt_controller_t*);
void bt_controller_stop(bt_controller_t*);