#include "simple_filter.h"

void filter_ctx_init(struct filter_ctx_t* ctx)
{
    ctx->sum = 0;
    ctx->size = 0;
    ctx->begin = 0;
    ctx->end = 0;
}

void filter_ctx_update(struct filter_ctx_t* ctx, char const* data, uint16_t len)
{
    for(uint16_t i = 0; i < len; i += 1)
    {
        char const val = data[i];
        ctx->sum += val;
        if (ctx->size == WINDOW_SIZE)
        {
            ctx->sum -= ctx->items[ctx->begin];
            ctx->begin = (ctx->begin + 1) % WINDOW_SIZE;
            ctx->items[ctx->end] = val;
            ctx->end = (ctx->end + 1) % WINDOW_SIZE;
        }
        else
        {
            ctx->items[ctx->end] = val;
            ctx->end = (ctx->end + 1) % WINDOW_SIZE;
            ctx->size += 1;
        }
        ctx->avg = ctx->sum / ctx->size;
    }
}

int32_t filter_ctx_get_avg(struct filter_ctx_t* ctx)
{
    return ctx->avg;
}
