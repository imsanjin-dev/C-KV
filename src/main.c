#include <stdio.h>
#include "common.h"  // 这里也会有红线，先不用管

int main() {         // 必须有这个 main 函数！
    print_banner();
    printf(">> System Initialized.\n");
    return 0;
}