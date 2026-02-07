# =============================================================================
# C-Super-KV 构建脚本 (Day 4: 单元测试集成版)
# =============================================================================

cmake_minimum_required(VERSION 3.10)
project(C-Super-KV)

# 1. 基础设置
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 开启调试符号 (-g) 和严格警告 (-Wall -Wextra)
# 工业界标准：不要忽略任何 Warning
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra")

# 指定头文件路径
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/tests) # 让测试代码能找到 framework.h

# =============================================================================
# 2. 核心代码分离 (关键步骤)
# =============================================================================
# 这里列出所有的 .c 文件，但 **绝对不要** 包含 main.c
# 也不要包含 test_kv.c
set(CORE_SRC 
    src/c_kv.c
    src/utils.c
    src/bst.c
    src/heap.c
    src/stack.c
    src/queue.c
    src/cmd.c
)

# =============================================================================
# 3. 构建主程序 (Product)
# =============================================================================
# 只有主程序才需要 src/main.c
add_executable(c-kv src/main.c ${CORE_SRC})

# =============================================================================
# 4. 构建测试程序 (Unit Tests)
# =============================================================================
enable_testing() # 开启 CTest 功能

# 测试程序 = 测试入口(test_kv.c) + 核心逻辑(CORE_SRC)
add_executable(run_tests tests/test_kv.c ${CORE_SRC})
add_executable(benchmark_tests tests/test_benchmark.c ${CORE_SRC})
add_executable(heap_test tests/test_heap.c ${CORE_SRC})


# 注册到 CTest
# 这样你就可以在 build 目录下运行 `ctest` 命令了
add_test(NAME CoreKVTest COMMAND run_tests)

# =============================================================================
# 5. 辅助信息
# =============================================================================
message(STATUS "-------------------------------------------------")
message(STATUS "Build Configured:")
message(STATUS "   App  Target: c-kv")
message(STATUS "   Test Target: run_tests")
message(STATUS "-------------------------------------------------")