#ifndef COMMON_H
#define COMMON_H

#define PROJECT_NAME "C-KV"
#define VERSION "0.1"

void print_banner(void);
typedef enum {
    SS_SUCCESS = 0,     //SS_SUCCESS (成功)
    SS_INVALID_PARAM,   // SS_INVALID_PARAM (参数错误)
    SS_KEY_NOT_FOUND,   //SS_KEY_NOT_FOUND (查不到)
    SS_OUT_OF_MEMORY,    //SS_OUT_OF_MEMORY (内存不足)
    SS_OUT_OF_STACK,     //超出栈长
    SS_STACK_EMPTY       //栈空
} SYS_STATUS;

#endif