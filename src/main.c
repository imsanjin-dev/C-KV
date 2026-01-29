#include <stdio.h>
#include "common.h"  
#include "c_kv.h"
#include "bst.h"

int main() {         
    printf("=== Phase 1: 写入数据并存盘 ===\n");
    KVS* db = kvs_create(0); // 默认容量
    
    kvs_put(db, "user:1", "Alice");
    kvs_put(db, "user:2", "Bob");
    kvs_put(db, "config:mode", "Debug");
    kvs_put(db, "fruit", "Apple");
    
    printf(">> 当前内存数据 (应按 Key 排序):\n");
    bst_print_keys(db->bstRoot); // 这里应该调用你的打印函数
    
    kvs_save(db, FILE_NAME);
    kvs_destroy(db); // 销毁内存，模拟程序退出
    printf(">> 数据库已关闭，内存已清空。\n\n");

    
    printf("=== Phase 2: 重启并加载数据 ===\n");
    KVS* new_db = kvs_create(0);
    kvs_load(new_db, FILE_NAME);
    
    printf(">> 恢复后的数据 (验证排序与完整性):\n");
    bst_print_keys(new_db->bstRoot);

    // 验证查询
    char* v1 = kvs_get(new_db, "user:1");
    char* v2 = kvs_get(new_db, "fruit");
    printf(">> Get check:\n");
    printf("   user:1 -> %s\n", v1 ? v1 : "NULL");
    printf("   fruit  -> %s\n", v2 ? v2 : "NULL");

    kvs_destroy(new_db);
    printf("=== Test Finished ===\n");
    return 0;
}