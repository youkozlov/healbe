#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "uart_controller.h"
#include "stats_controller.h"
#include "wifi_controller.h"
#include "bt_controller.h"
#include "bus_container.h"

static volatile int done = 0;
void intHandler(int rv)
{
    printf("exit(%d)\n", rv);
    done = 1;
}

int main(int argc, char** argv)
{
    signal(SIGINT, intHandler);

    printf("argc=%d argv[0]=%s\n", argc, argv[0]);

    uint32_t const num_threads = 4;
    bus_container_t* bus_container = bus_container_init(num_threads);

    uart_controller_t* uart_controller = uart_controller_init(bus_container);
    stats_controller_t* stats_controller = stats_controller_init(bus_container);
    wifi_controller_t* wifi_controller = wifi_controller_init(bus_container);
    bt_controller_t* bt_controller = bt_controller_init(bus_container);

    bus_container_start(bus_container);
    uart_controller_start(uart_controller);
    stats_controller_start(stats_controller);
    wifi_controller_start(wifi_controller);
    bt_controller_start(bt_controller);

    while (!done)
    {
    }

    bt_controller_stop(bt_controller);
    wifi_controller_stop(wifi_controller);
    stats_controller_stop(stats_controller);
    uart_controller_stop(uart_controller);
    bus_container_stop(bus_container);

    bt_controller_free(bt_controller);
    wifi_controller_free(wifi_controller);
    uart_controller_free(uart_controller);
    stats_controller_free(stats_controller);

    bus_container_free(bus_container);

    return 0;
}