#pragma once

#include "stdint.h"

typedef struct bus_msg
{
    uint32_t    queue_id;
    uint32_t    pool_id;
    uint32_t    msg_id;
    void*       payload;
    uint32_t    size;
} bus_msg_t;