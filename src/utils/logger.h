#pragma once

#include <stdio.h>
#include <string.h>

#define LOG_LVL 1
#define LM_ERR(...) if(LOG_LVL >= 3) { char const* filePart = strrchr(__FILE__, '/'); char logger_msg_buf[256]; sprintf(logger_msg_buf, __VA_ARGS__); printf("ERR %s %d: %s\n", filePart ? filePart + 1 : __FILE__, __LINE__, logger_msg_buf);}
#define LM_INF(...) if(LOG_LVL >= 1) { char const* filePart = strrchr(__FILE__, '/'); char logger_msg_buf[256]; sprintf(logger_msg_buf, __VA_ARGS__); printf("INF %s %d: %s\n", filePart ? filePart + 1 : __FILE__, __LINE__, logger_msg_buf);}
#define LM_DBG(...) if(LOG_LVL == 0) { char const* filePart = strrchr(__FILE__, '/'); char logger_msg_buf[256]; sprintf(logger_msg_buf, __VA_ARGS__); printf("DBG %s %d: %s\n", filePart ? filePart + 1 : __FILE__, __LINE__, logger_msg_buf);}