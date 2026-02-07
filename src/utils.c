#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"common.h"
void print_banner(){
    printf("=====Welcome to %s(v%s)!Input 'help' for help=====\n",PROJECT_NAME,VERSION);
}
char* str_copy(const char* s){
    int lenth=strlen(s);
    char* sCopy=(char*)malloc((lenth+1)*sizeof(char));
    if (sCopy==NULL)
    {
        return NULL;
    }
    strcpy(sCopy,s);
    return sCopy;
}

unsigned long hash(const char* str){
    unsigned long hash=5381;
    while (*str!='\0')
    {
        char c=*str++;
        hash=hash*33+(unsigned long)c;
    }
    return hash;
}
