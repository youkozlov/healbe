#pragma once

#include "msg_bus.h"

struct wifi_controller_params_t
{
    struct msg_bus_t* msg_bus_stats;
};

void* wifi_controller(void* params_);
