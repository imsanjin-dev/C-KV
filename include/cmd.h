#ifndef CMD_H
#define CMD_H

#include"c_kv.h"
#include"avl.h"
#include"queue.h"
#include"stack.h"

#define CMD_MAX_INPUT 1024
#define CMD_MAX_ARGS 5

typedef enum {
    CMD_UNKNOWN = 0,  // 未知命令
    CMD_PUT,          // put命令
    CMD_PUT_EXPIRE,   // put_expire命令
    CMD_GET,          // get命令  
    CMD_DEL,          // delete命令
    CMD_KEYS,         // 打印所有 key（avl 中序遍历）
    CMD_SAVE,         // save命令   
    CMD_LOAD,         // load命令
    CMD_HISTORY,      // 显示历史命令
    CMD_HELP,         // 提示信息
    CMD_EXIT          // 退出命令
} CommandType;

void cmd_main_loop(KVS* db);
CommandType parse_input(char* input, char** args, int* arg_count);

#endif