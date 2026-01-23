#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <string.h>

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_RESET "\x1b[0m"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK_NUM(expect,actual) do{\
    tests_run++;\
    if ((expect)!=(actual))\
    {\
        printf(COLOR_RED"[ERROR] %s:%d: Want %d, but got %d\n"COLOR_RESET,\
            __FILE__,__LINE__,(expect),(int)(actual));\
        tests_failed++;\
    }else{\
        printf(COLOR_GREEN"."COLOR_RESET);\
    }\
}while(0)

#define CHECK_STR(expect,actual) do{\
    tests_run++;\
    if ((expect)==NULL||(actual)==NULL)\
    {\
        if ((expect)!=(actual))\
        {\
            printf(COLOR_RED"[ERROR] %s:%d: String is NULL!\n"COLOR_RESET, __FILE__, __LINE__); \
            tests_failed++; \
        }\
    }\
    else if (strcmp((expect),(actual))!=0)\
    {\
        printf(COLOR_RED"[ERROR] %s:%d: Want %s, but got %s\n"COLOR_RESET,\
            __FILE__,__LINE__,(expect),(actual));\
        tests_failed++;\
    }\
    else{\
        printf(COLOR_GREEN"."COLOR_RESET);\
    }\
}while(0)

// 打印测试报告
#define PRINT_REPORT() do { \
    printf("\n\n=== Test Report ===\n"); \
    printf("Total: %d\n", tests_run); \
    printf("Fails: %d\n", tests_failed); \
    \
    if (tests_failed == 0) { \
        printf(COLOR_GREEN "SUCCESS! All tests passed.\n" COLOR_RESET); \
    } else { \
        printf(COLOR_RED "FAIL! Fix the bugs.\n" COLOR_RESET); \
    } \
} while(0)

#endif