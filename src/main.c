#include <stdio.h>
#include "common.h"  
#include "c_kv.h"

int main() {         
    print_banner();
    printf(">> System Initialized.\n");

   printf(">> 1. [Init] 初始化数据库...\n");
    KVS* db = kvs_create(0);
    if (!db) {
        printf("Failed to create DB\n");
        return 1;
    }

    printf(">> 2. [Put] 写入数据 (user:1 -> zhangsan)...\n");
    kvs_put(db, "user:1", "zhangsan");
    
    // 验证写入
    char* val = kvs_get(db, "user:1");
    printf("   -> Get user:1: %s\n", val ? val : "NULL");

    printf(">> 3. [Delete] 删除数据 (user:1)...\n");
    SYS_STATUS status = kvs_delete(db, "user:1");
    if (status == SS_SUCCESS) {
        printf("   -> 删除成功！\n");
    } else {
        printf("   -> 删除失败，错误码: %d\n", status);
    }

    // 验证删除结果（应该找不到）
    printf(">> 4. [Verify] 再次尝试获取 (user:1)...\n");
    val = kvs_get(db, "user:1");
    if (val == NULL) {
        printf("   -> 验证通过：数据已不存在。\n");
    } else {
        printf("   -> 验证失败：见鬼了，数据还在！\n");
    }

    printf(">> 5. [Destroy] 销毁数据库...\n");
    kvs_destroy(db);
    printf(">> System Shutdown.\n");

    return 0;
}