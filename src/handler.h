#pragma once

typedef struct handler
{
    void* this_;
    void (*function)(void*);
} handler_t;

void handler_exec(handler_t* handler);
handler_t handler_get(void* this_, void (*function)(void*));

typedef struct handler_msg
{
    void* this_;
    void (*function)(void*, void const*);
} handler_msg_t;

void handler_exec_msg(handler_msg_t* handler, void const* msg);
handler_msg_t handler_msg(void* this_, void (*function)(void*, void const*));