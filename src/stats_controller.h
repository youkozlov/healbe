#pragma once

struct stats_controller_params_t
{
    struct msg_bus_t* msg_bus_uart;
    struct msg_bus_t* msg_bus_stats_wifi;
    struct msg_bus_t* msg_bus_stats_bt;
};

void* stats_controller(void* params_);