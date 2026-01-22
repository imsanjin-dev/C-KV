#include<stdio.h>
#include<string.h>
#include"common.h"
void print_banner(){
    printf("=====Welcome to %s(v%s)=====\n",PROJECT_NAME,VERSION);
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