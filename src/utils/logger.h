#pragma once

#include <stdio.h>
#include <string.h>

#define LM_ERR(...) { /*char const* filePart = strrchr(__FILE__, '/'); char msg[256]; sprintf(msg, __VA_ARGS__); printf("ERR %s %d: %s\n", filePart ? filePart + 1 : __FILE__, __LINE__, msg);*/}
#define LM_DBG(...) { /*char const* filePart = strrchr(__FILE__, '/'); char msg[256]; sprintf(msg, __VA_ARGS__); printf("DBG %s %d: %s\n", filePart ? filePart + 1 : __FILE__, __LINE__, msg);*/}