#include "uart_controller.h"

#include <unistd.h>
#include <stdlib.h>
#include "logger.h"
#include "msg_bus.h"
#include "uart_data_ind.h"

#define TICK_TIME_MS 3000

int receive_from_uart(char* data)
{
    for (int i = 0; i < UART_PAYLOAD; i += 1)
    {
        data[i] = rand();
    }
    return 0;
}

void* uart_controller(void* params_)
{
    LM_DBG("UART started");
    struct uart_controller_params_t* params = (struct uart_controller_params_t*)params_;
    while(1)
    {
        struct msg_t* msg = msg_bus_pull_free(params->msg_queue);
        if (!msg)
        {
            LM_ERR("There are no free pools");
        }
        else
        {
            struct uart_data_ind_t* payload = (struct uart_data_ind_t*)msg->payload;
            char result = receive_from_uart(payload->data);
            if (result)
            {
                LM_ERR("UART return (%d)", result);
                msg_bus_push_free(params->msg_queue, msg);
            }
            else
            {
                LM_DBG("UART produce");
                msg_bus_push_busy(params->msg_queue, msg);
            }
        }
        usleep(1000 * TICK_TIME_MS);
    }
    LM_DBG("UART stopped");
    return 0;
}