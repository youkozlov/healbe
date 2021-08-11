#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "msg_bus.h"
#include "uart_data_ind.h"
#include "stats_ind.h"
#include "uart_controller.h"
#include "stats_controller.h"
#include "wifi_controller.h"
#include "bt_controller.h"

int main(int argc, char** argv)
{
    printf("argc=%d argv[0]=%s\n", argc, argv[0]);

    struct msg_bus_t msg_bus_uart;
    msg_bus_init(&msg_bus_uart, sizeof(struct uart_data_ind_t));

    struct msg_bus_t msg_bus_stats_wifi;
    msg_bus_init(&msg_bus_stats_wifi, sizeof(struct stats_ind_t));

    struct msg_bus_t msg_bus_stats_bt;
    msg_bus_init(&msg_bus_stats_bt, sizeof(struct stats_ind_t));


    struct uart_controller_params_t uart_params;
    uart_params.msg_queue = &msg_bus_uart;
    pthread_t uart_thread;
    pthread_create(&uart_thread, NULL, uart_controller, &uart_params);

    struct stats_controller_params_t stats_params;
    stats_params.msg_bus_uart = &msg_bus_uart;
    stats_params.msg_bus_stats_wifi = &msg_bus_stats_wifi;
    stats_params.msg_bus_stats_bt = &msg_bus_stats_bt;
    pthread_t stats_thread;
    pthread_create(&stats_thread, NULL, stats_controller, &stats_params);

    struct wifi_controller_params_t wifi_params;
    wifi_params.msg_bus_stats = &msg_bus_stats_wifi;
    pthread_t wifi_thread;
    pthread_create(&wifi_thread, NULL, wifi_controller, &wifi_params);

    struct bt_controller_params_t bt_params;
    bt_params.msg_bus_stats = &msg_bus_stats_bt;
    pthread_t bt_thread;
    pthread_create(&bt_thread, NULL, bt_controller, &bt_params);

    pthread_join(uart_thread, NULL);
    pthread_join(stats_thread, NULL);
    pthread_join(wifi_thread, NULL);
    pthread_join(bt_thread, NULL);

    msg_bus_free(&msg_bus_uart);
    msg_bus_free(&msg_bus_stats_wifi);
    msg_bus_free(&msg_bus_stats_bt);

    return 0;
}