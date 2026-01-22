#include <stdio.h>
#include "common.h"  
#include "c_kv.h"

int main() {         
    print_banner();
    printf(">> System Initialized.\n");

    CKV* kv_db=kvs_create();
    if (kv_db==NULL)
    {
        printf("内存满了");
        return 1;
    }

    kvs_put(kv_db,"diyige","yiyiyi");
    kvs_get(kv_db,"diyige");
    kvs_delete(kv_db,"diyige");

    kv_db->count=999;

    kvs_destroy(kv_db);
    
    return 0;
}