
add_library(utils STATIC
    queue.c
    simple_filter.c
)

target_include_directories(utils PUBLIC .)