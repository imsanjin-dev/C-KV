#include "../include/queue.h"
#include "../include/utils.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void queue_init(historyQueue* queue){
    memset(queue->historyCommand,0,sizeof(queue->historyCommand));
    queue->count=0;
    queue->front=0;
    queue->rear=0;
}
void queue_add(historyQueue* queue,const char* command){
    char* newCommand=str_copy(command);
    if (!newCommand)
    {
        return;
    }
    if (queue->count==HISTORY_SIZE)
    {
        free(queue->historyCommand[queue->front]);
        queue->front=(queue->front+1)%HISTORY_SIZE;
        queue->count--;
    }
    queue->historyCommand[queue->rear]=newCommand;
    queue->rear=(queue->rear+1)%HISTORY_SIZE;
    queue->count++;
}
void queue_print(historyQueue* queue){
    for (int i = 0; i < queue->count; i++)
    {
        printf("%d:%s\n",i+1,queue->historyCommand[(queue->front+i)%HISTORY_SIZE]);
    }
}
void queue_destroy(historyQueue* queue){
    for (int i = 0; i < queue->count; i++)
    {
        if (queue->historyCommand[(queue->front+i)%HISTORY_SIZE]!=NULL)
        {
            free(queue->historyCommand[(queue->front+i)%HISTORY_SIZE]);
        }
    }
}