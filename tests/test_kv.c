#include "test_framework.h"
#include "../include/c_kv.h"
#include "../include/common.h"
#include "../include/utils.h"

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
}

int main() {
    test_database_init();
    test_put_get();
    test_delete();
    PRINT_REPORT();
    return tests_failed; // 如果 fails 是 0，返回 0 (成功)
}