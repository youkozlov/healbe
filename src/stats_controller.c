#include "stats_controller.h"

#include <stdio.h>
#include <unistd.h>
#include "logger.h"
#include "simple_filter.h"
#include "msg_bus.h"
#include "uart_data_ind.h"
#include "stats_ind.h"

void send_stats_ind(struct msg_bus_t* msg_bus, int32_t avg)
{
    struct msg_t* msg = msg_bus_pull_free(msg_bus);
    if (!msg)
    {
        LM_ERR("There are no free pools");
    }
    else
    {
        LM_DBG("Stats produced");
        struct stats_ind_t* payload = (struct stats_ind_t*)msg->payload;
        payload->average = avg;
        msg_bus_push_busy(msg_bus, msg);
    }
}

void* stats_controller(void* params_)
{
    LM_DBG("Stats started");

    struct filter_ctx_t filter_ctx;
    filter_ctx_init(&filter_ctx);

    struct stats_controller_params_t* params = (struct stats_controller_params_t*)params_;
    while (1)
    {
        struct msg_t* msg = msg_bus_pull_busy(params->msg_bus_uart);
        if (!msg)
        {
            LM_DBG("There are no msgs");
            continue;
        }
        LM_DBG("Stats consume");

        struct uart_data_ind_t* payload = (struct uart_data_ind_t*)msg->payload;
        filter_ctx_update(&filter_ctx, payload->data, UART_PAYLOAD);
        msg_bus_push_free(params->msg_bus_uart, msg);

        send_stats_ind(params->msg_bus_stats_wifi, filter_ctx_get_avg(&filter_ctx));
        send_stats_ind(params->msg_bus_stats_bt, filter_ctx_get_avg(&filter_ctx));
    }

    LM_DBG("Stats stopped");
    return 0;
}