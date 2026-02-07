#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<string.h>
#include <strings.h>
#include <unistd.h>
#include"../include/cmd.h"

CommandType check_type(char* arg){
    if (strcasecmp(arg,"put")==0)
    {
        return CMD_PUT;
    }
    if (strcasecmp(arg,"get")==0)
    {
        return CMD_GET;
    }
    if (strcasecmp(arg,"put_expire")==0)
    {
        return CMD_PUT_EXPIRE;
    }
    if (strcasecmp(arg,"del")==0||strcasecmp(arg,"delete")==0)
    {
        return CMD_DEL;
    }
    if (strcasecmp(arg,"save")==0)
    {
        return CMD_SAVE;
    }
    if (strcasecmp(arg,"load")==0)
    {
        return CMD_LOAD;
    }
    if (strcasecmp(arg,"history")==0)
    {
        return CMD_HISTORY;
    }
    if (strcasecmp(arg,"help")==0)
    {
        return CMD_HELP;
    }
    if (strcasecmp(arg,"exit")==0)
    {
        return CMD_EXIT;
    }
    return CMD_UNKNOWN; 
}

CommandType parse_input(char* input,char** args,int* arg_count){
    int escape=0;
    int in_quotes = 0;
    char* current_args_start=NULL;
    char* read=input;
    char* write=input;
    while(*read==' '||*read=='\t')
    {
        read++;
    }
    while (*read!='\0')
    {
        char c=*read;
        if (current_args_start==NULL)
        {
            if (*arg_count<MAX_ARGS)
            {
                current_args_start=write;
                args[*arg_count]=current_args_start;
                (*arg_count)++;
            }else{
                return CMD_UNKNOWN;
            }
        }
        if (escape&&in_quotes)
        {
            *write=c;
            write++;
            read++;
            escape=0;
            continue;
        }
        if (c=='\\')
        {
            if (in_quotes)
            {
                read++;
                escape=1;
                continue;
            }else{
                *write=c;
                write++;
                read++;
                continue;
            }
        }
        if (c=='"')
        {
            in_quotes=in_quotes==0?1:0;
            read++;
            continue; 
        }
        if (c==' '||c=='\t')
        {
            if (in_quotes)
            {
                *write=c;
                write++;    
                read++;
                continue;
            }else{
                *write='\0';
                write++;
                current_args_start=NULL;
                read++;
                while (*read==' '||*read=='\t')
                {
                    read++;
                }
                continue;
            }
        }
        *write=c;
        write++;
        read++;
    }
    if (current_args_start!=NULL)
    {
        *write='\0';
    }
    if (*arg_count==0)
    {
        return CMD_UNKNOWN;
    }
    CommandType type=check_type(args[0]);
    switch (type) {
        case CMD_PUT:
            if (*arg_count != 3) {
                printf("Usage: put <key> <value>\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_PUT_EXPIRE:
            if (*arg_count != 4) {
                printf("Usage: put_expire <key> <value> <ttl>\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_GET:
            if (*arg_count != 2) {
                printf("Usage: get <key>\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_DEL:
            if (*arg_count != 2) {
                printf("Usage: del <key>\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_SAVE:
            if (*arg_count != 1 && *arg_count != 2) {
                printf("Usage: save [filename]\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_LOAD:
            if (*arg_count != 1 && *arg_count != 2) {
                printf("Usage: load [filename]\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_HISTORY:
            if (*arg_count != 1) {
                printf("Usage: history\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_HELP:
            if (*arg_count != 1) {
                printf("Usage: help\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_EXIT:
            if (*arg_count != 1) {
                printf("Usage: exit\n");
                return CMD_UNKNOWN;
            }
            break;

        case CMD_UNKNOWN:
        default:
            return CMD_UNKNOWN;
    }
    return type;
}
void cmd_main_loop(KVS* db){
    historyQueue history;
    queue_init(&history);
    print_banner();
    int running=1;
    while (running)
    {
        printf("\nCKV>>  ");
        char input[MAX_INPUT];
        if (fgets(input,MAX_INPUT,stdin)==NULL)
        {
            printf("bye!");
            sleep(1);
            break;
        }
        input[strcspn(input,"\n")]='\0';
        if (strlen(input)==0)
        {
            continue;
        }
        queue_add(&history,input);
        if (!stack_check_quotes(input))
        {
            printf("Input is not compliant");
            continue;
        }
        char* args[MAX_ARGS];
        int arg_count = 0;
        CommandType cmd=parse_input(input,args,&arg_count); 
        switch (cmd) {
            case CMD_HELP:
                printf("Commands:\n");
                printf("  put <key> <value>\n");
                printf("  put_expire <key> <value> <ttl>\n");
                printf("  get <key>\n");
                printf("  del <key>\n");
                printf("  save [filename]\n");
                printf("  load [filename]\n");
                printf("  history\n");
                printf("  help\n");
                printf("  exit\n");
                break;

            case CMD_HISTORY:
                queue_print(&history);
                break;

            case CMD_PUT: {
                SYS_STATUS st = kvs_put(db, args[1], args[2]);
                if (st == SS_SUCCESS) printf("OK\n");
                else if (st == SS_OUT_OF_MEMORY) printf("Error: out of memory\n");
                else if (st == SS_INVALID_PARAM) printf("Error: invalid param\n");
                else printf("Error: put failed (%d)\n", st);
                break;
            }

            case CMD_GET: {
                char* val = kvs_get(db, args[1]);
                if (val) printf("%s\n", val);
                else printf("NOT FOUND\n");
                break;
            }

            case CMD_DEL: {
                SYS_STATUS st = kvs_delete(db, args[1]);
                if (st == SS_SUCCESS) printf("OK\n");
                else if (st == SS_KEY_NOT_FOUND) printf("NOT FOUND\n");
                else if (st == SS_INVALID_PARAM) printf("Error: invalid param\n");
                else printf("Error: del failed (%d)\n", st);
                break;
            }

            case CMD_SAVE: {
                const char* filename = (arg_count == 2) ? args[1] : FILE_NAME;
                kvs_save(db, filename);
                printf("Saved to %s\n", filename);
                break;
            }

            case CMD_LOAD: {
                const char* filename = (arg_count == 2) ? args[1] : FILE_NAME;
                kvs_load(db, filename);
                printf("Loaded from %s\n", filename);
                break;
            }

            case CMD_PUT_EXPIRE: {
                char* end = NULL;
                long ttl = strtol(args[3], &end, 10);

                if (end == args[3] || *end != '\0' || ttl < 0 || ttl > INT_MAX) {
                    printf("Error: ttl must be a non-negative integer\n");
                    break;
                }

                SYS_STATUS st = kvs_put_expire(db, args[1], args[2], (int)ttl);
                if (st == SS_SUCCESS) printf("OK\n");
                else if (st == SS_OUT_OF_MEMORY) printf("Error: out of memory\n");
                else if (st == SS_INVALID_PARAM) printf("Error: invalid param\n");
                else printf("Error: put_expire failed (%d)\n", st);
                break;
            }

            case CMD_EXIT:
                printf("bye!\n");
                running=0;
                break;

            case CMD_UNKNOWN:
            default:
                printf("Unknown command. Type 'help' for usage.\n");
                break;
        }
    }
    queue_destroy(&history);
}