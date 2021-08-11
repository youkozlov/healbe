add_subdirectory(msgs)
add_subdirectory(utils)

find_library(PTHREAD_LIBRARY pthread)

add_executable(main
    main.c
    msg_bus.c
    uart_controller.c
    stats_controller.c
    wifi_controller.c
    bt_controller.c
)

target_link_libraries(main ${PTHREAD_LIBRARY} msgs utils)