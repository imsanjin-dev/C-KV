#include"../include/utils.h"
#include"../include/common.h"
#include"../include/c_kv.h"
#include<time.h>
#include<stdio.h>
#include<string.h>

int main(){
    KVS* db=kvs_create();
    if (db==NULL)
    {
        return 1;
    }

    clock_t put_start=clock();
    for (int i = 0; i < 100000; i++)
    {
        char testKey[12];
        char testValue[14];
        sprintf(testKey,"key-%d",i);
        sprintf(testValue,"value-%d",i);
        kvs_put(db,testKey,testValue);
    }
    clock_t put_end=clock();
    double put_time_spent=(double)(put_end-put_start)/CLOCKS_PER_SEC;
    printf("put运行耗时: %lf 秒\n",put_time_spent);

    int get_failed=0;
    clock_t get_start=clock();
    for (int i = 0; i < 100000; i++)
    {
        char testKey[12];
        char testValue[14];
        sprintf(testKey,"key-%d",i);
        sprintf(testValue,"value-%d",i);
        char* actual=kvs_get(db,testKey);
        if (actual!=NULL)
        {
            if (strcmp(actual,testValue)!=0)
            {
                get_failed++;
                continue;
            }else{
                continue;
            }
        }
        get_failed++;
        continue;
    }
    clock_t get_end=clock();
    double get_time_spent=(double)(get_end-get_start)/CLOCKS_PER_SEC;
    printf("get运行耗时: %lf 秒，错误 %d 次\n",get_time_spent,get_failed);

    kvs_destroy(db); 
}