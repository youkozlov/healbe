#include "wifi_controller.h"

#include "logger.h"
#include "msg_bus.h"
#include "stats_ind.h"
/*
void send_to_wifi(int8_t avg)
{
    LM_DBG("Send avg...%d", avg);
}
*/
void* wifi_controller(void* params_)
{
    LM_DBG("WIFI started");
    struct wifi_controller_params_t* params = (struct wifi_controller_params_t*)params_;
    while(1)
    {
        struct msg_t* msg = msg_bus_pull_busy(params->msg_bus_stats);
        if (!msg)
        {
            LM_DBG("There are no msgs");
            continue;
        }
        LM_DBG("WIFI consume");
        //struct stats_ind_t* payload = (struct stats_ind_t*)msg->payload;
        //send_to_wifi(payload->average);
        msg_bus_push_free(params->msg_bus_stats, msg);
    }
    LM_DBG("WIFI stopped");
    return 0;
}