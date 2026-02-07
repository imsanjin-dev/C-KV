#include"../include/stack.h"

void stack_init(charStack* stack){
    stack->top=-1;
}
SYS_STATUS stack_push(charStack* stack,char c){
    if (!stack)
    {
        return SS_INVALID_PARAM;
    }
    if (stack->top==1023)
    {
        return SS_OUT_OF_STACK;
    }
    stack->data[++(stack->top)]=c;  
    return SS_SUCCESS;
}
SYS_STATUS stack_pop(charStack* stack,char* out_c){
    if (!stack||!out_c)
    {
        return SS_INVALID_PARAM;
    }
    if (stack->top==-1)
    {
        return SS_STACK_EMPTY;
    }
    *out_c=stack->data[(stack->top)--];
    return SS_SUCCESS;
}
bool stack_check_quotes(const char* command_str){
    charStack s;
    char a; //没什么用的，接收废值
    stack_init(&s);
    while (*command_str!='\0')
    {
        if (*command_str=='"')
        {
            if (s.top==-1)
            {
                stack_push(&s,'"');
            }else{
                stack_pop(&s,&a);
            } 
        }
        if (*command_str=='\\')
        {
            command_str++;
            if (*command_str=='\0')
            {
                return false;
            }   
        }
        command_str++;
    }
    if (s.top!=-1)
    {
        return false;
    }
    return true;
}