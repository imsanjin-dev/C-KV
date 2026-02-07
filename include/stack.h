#ifndef STACK_H
#define STACK_H

#include "common.h"
#include <stdbool.h>
#include <string.h>

typedef struct charStack{
    char data[1024];
    int top;
}charStack;

void stack_init(charStack* stack);
SYS_STATUS stack_push(charStack* stack,char c);
SYS_STATUS stack_pop(charStack* stack,char* out_c);
bool stack_check_quotes(const char* command_str);

#endif