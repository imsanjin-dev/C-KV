#include <stdio.h>
#include "common.h"  
#include "c_kv.h"
#include "bst.h"
#include "cmd.h"

int main(void) {
    KVS* db = kvs_create(0);
    if (!db) {
        fprintf(stderr, "Error: failed to create KVS\n");
        return 1;
    }
    cmd_main_loop(db);

    kvs_destroy(db);

    return 0;
}