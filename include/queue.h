#ifndef QUEUE_H
#define QUEUE_H

#define HISTORY_SIZE 10

typedef struct historyQueue{
    char* historyCommand[HISTORY_SIZE];
    int front;
    int rear;
    int count;
}historyQueue;

void queue_init(historyQueue* queue);
void queue_add(historyQueue* queue,const char* command);
void queue_print(historyQueue* queue);
void queue_destroy(historyQueue* queue);

#endif