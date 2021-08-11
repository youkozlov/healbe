#pragma once

struct bt_controller_params_t
{
    struct msg_bus_t* msg_bus_stats;
};

void* bt_controller(void* params_);
