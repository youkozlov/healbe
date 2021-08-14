add_subdirectory(msgs)
add_subdirectory(utils)

find_library(PTHREAD_LIBRARY pthread)

add_executable(main
    main.c
    uart_controller.c
    stats_controller.c
    wifi_controller.c
    bt_controller.c
    handler.c
    bus_timer.c
    bus_thread.c
    bus_container.c
    bus_controller.c
    bus_msg_pool.c
)

target_link_libraries(main ${PTHREAD_LIBRARY} msgs utils)