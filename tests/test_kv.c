#include "test_framework.h"
#include "../include/c_kv.h"
#include "../include/common.h"
#include "../include/utils.h"
#include<time.h>
#include<stdio.h>
#include<string.h>

void test_database_init(){
        KVS* db=kvs_create();
        if (db!=NULL)
        {
            CHECK_NUM(kvs_get_count(db),0);
        }else{
            tests_failed++;
            printf(COLOR_RED"[ERROR] %s:%d: DataBase is NULL!\n"COLOR_RESET, __FILE__, __LINE__); \
        }
        kvs_destroy(db);
    }

void test_put_get(){
    KVS* db=kvs_create();
    CHECK_NUM(kvs_put(db,"key1","value1"),SS_SUCCESS);
    CHECK_NUM(kvs_get_count(db),1);
    char* val = kvs_get(db, "key1");
    CHECK_STR(val,"value1");
    kvs_destroy(db);
}

void test_delete(){
    KVS* db=kvs_create();
    CHECK_NUM(kvs_put(db,"key1","value1"),SS_SUCCESS);
    CHECK_NUM(kvs_put(db,"key2","value2"),SS_SUCCESS);
    CHECK_NUM(kvs_delete(db,"key1"),SS_SUCCESS);
    CHECK_STR(kvs_get(db,"key1"),NULL);
    CHECK_NUM(kvs_get_count(db),1);
    CHECK_NUM(kvs_put(db,"key1","value1"),SS_SUCCESS);
    CHECK_NUM(kvs_delete(db,"key1"),SS_SUCCESS);
    CHECK_STR(kvs_get(db,"key1"),NULL);
    CHECK_NUM(kvs_get_count(db),1);
    CHECK_NUM(kvs_delete(db,"key2"),SS_SUCCESS);
    CHECK_STR(kvs_get(db,"key2"),NULL);
    CHECK_NUM(kvs_get_count(db),0);
    kvs_destroy(db);
}

void test_hash_func(){
    char* key1="test_key";
    unsigned long h1=hash(key1);
    unsigned long h2=hash("test_key");
    CHECK_LONG_ISSAME1(h1,h2);
    unsigned long h3=hash("abc");
    unsigned long h4=hash("acb");
    CHECK_LONG_ISSAME2(h3,h4);
    printf("\n%lu,%lu,%lu,%lu\n",h1,h2,h3,h4);
}

//测试前将TABLE_SIZE改为128，注释掉test_lru_eviction
void test_resize(){
    KVS* db=kvs_create();
    for (int i = 0; i < 100; i++)
    {
        char testKey[12];
        char testValue[14];
        sprintf(testKey,"key-%d",i);
        sprintf(testValue,"value-%d",i);
        kvs_put(db,testKey,testValue);
    }
    CHECK_NUM(kvs_get_count(db),100);
    CHECK_NUM(kvs_get_bucket_count(db),256);
    kvs_destroy(db);
}

//测试前将maxCapacity改为5,注释掉test_resize
void test_lru_eviction(){
    KVS* db=kvs_create();
    for (int i = 0; i <=5 ; i++)
    {
        char testKey[12];
        char testValue[14];
        sprintf(testKey,"key-%d",i);
        sprintf(testValue,"value-%d",i);
        kvs_put(db,testKey,testValue);
    }
    CHECK_STR(NULL,kvs_get(db,"key-0"));
    CHECK_STR("value-5",kvs_get(db,"key-5"));
    kvs_get(db,"key-1");
    kvs_put(db,"key-6","value-6");
    CHECK_STR(NULL,kvs_get(db,"key-2"));
    CHECK_STR("value-6",kvs_get(db,"key-6"));
    kvs_destroy(db);
}

int main() {
    test_database_init();
    test_put_get();
    test_delete();
    test_hash_func();
    test_resize();
    PRINT_REPORT();
    return tests_failed; // 如果 fails 是 0，返回 0 (成功)
}