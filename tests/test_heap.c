#include "test_framework.h"
#include "../include/heap.h"
#include "../include/c_kv.h"
#include <unistd.h> // for sleep()
#include <stdlib.h>

// --- Part 1: 纯堆逻辑测试 ---
void test_heap_logic() {
    printf(">> Testing Pure Heap Logic...\n");
    MinHeap* h = heap_create(10);
    
    // 乱序插入: 5s, 1s, 3s
    // 假设当前时间是 base
    time_t base = 100000; 
    heap_insert(h, "Key_5s", base + 5);
    heap_insert(h, "Key_1s", base + 1);
    heap_insert(h, "Key_3s", base + 3);

    // 验证堆顶是否是最小的 (Key_1s)
    HeapNode* min = heap_get_min(h);
    CHECK_STR("Key_1s", min->key);

    // 依次弹出，顺序应该是: 1s -> 3s -> 5s
    char* k;
    
    // Pop 1
    heap_remove_min(h, &k);
    CHECK_STR("Key_1s", k);
    free(k);

    // Pop 2
    heap_remove_min(h, &k);
    CHECK_STR("Key_3s", k); // 应该是 3，不是 5
    free(k);

    // Pop 3
    heap_remove_min(h, &k);
    CHECK_STR("Key_5s", k);
    free(k);

    heap_destroy(h);
}

// --- Part 2: KVS 集成 TTL 测试 ---
void test_kvs_ttl() {
    printf(">> Testing KVS TTL Integration...\n");
    KVS* db = kvs_create(0);

    // 1. 插入一个 1秒后过期的 Key
    kvs_put_expire(db, "quick_die", "RIP", 1);
    
    // 2. 插入一个 5秒后过期的 Key (长寿)
    kvs_put_expire(db, "long_live", "Alive", 5);

    // 3. 立即查询 (应该都能查到)
    CHECK_STR("RIP", kvs_get(db, "quick_die"));
    CHECK_STR("Alive", kvs_get(db, "long_live"));

    // 4. 等待 2 秒
    printf("   Sleeping for 2 seconds...\n");
    sleep(2);

    // 5. 再次查询
    // quick_die 应该过期了 (惰性删除触发) -> 返回 NULL
    char* val1 = kvs_get(db, "quick_die");
    if (val1 != NULL) {
        printf(COLOR_RED "[FAIL] quick_die should be expired!\n" COLOR_RESET);
        tests_failed++;
    } else {
        printf(COLOR_GREEN "." COLOR_RESET);
    }

    // long_live 应该还在
    CHECK_STR("Alive", kvs_get(db, "long_live"));

    kvs_destroy(db);
}

int main() {
    test_heap_logic();
    test_kvs_ttl();
    PRINT_REPORT();
    return tests_failed;
}