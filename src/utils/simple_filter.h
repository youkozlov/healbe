#pragma once

#include <stdint.h>

#define WINDOW_SIZE 32

struct filter_ctx_t
{
    int32_t  avg;
    int32_t  sum;
    uint16_t size;
    uint16_t begin;
    uint16_t end;
    int16_t  items[WINDOW_SIZE];
};

void filter_ctx_init(struct filter_ctx_t*);

void filter_ctx_update(struct filter_ctx_t*, char const* data, uint16_t len);

int32_t filter_ctx_get_avg(struct filter_ctx_t*);
