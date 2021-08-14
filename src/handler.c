#include "handler.h"

void handler_exec(handler_t* handler)
{
    handler->function(handler->this_);
}

handler_t handler_get(void* this_, void (*function)(void*))
{
    handler_t handler = {this_, function};
    return handler;
}

void handler_exec_msg(handler_msg_t* handler, void const* msg)
{
    handler->function(handler->this_, msg);
}

handler_msg_t handler_msg(void* this_, void (*function)(void*, void const*))
{
    handler_msg_t handler;
    handler.this_       = this_;
    handler.function    = function;
    return handler;
}