# ==========================================
# C-KV-Store 构建脚本
# ==========================================

# 1. 指定 CMake 最低版本 (3.10 是 Ubuntu 18.04 默认版本，兼容性好)
cmake_minimum_required(VERSION 3.10)

# 2. 定义项目名称
project(C-Super-KV)

# 3. 设置 C 语言标准
# 使用 C99 标准 (允许 // 注释，允许在 for 循环里定义变量)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 4. 开启调试模式 & 编译器警告
# Debug 模式会包含调试符号，方便后续用 GDB/VSCode 调试
# -Wall -Wextra: 打开所有警告，强迫你写出规范的代码
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# 5. 指定头文件搜索路径
# 这样你在代码里写 #include "kv.h" 时，编译器知道去 include/ 文件夹找
include_directories(${CMAKE_SOURCE_DIR}/include)

# 6. 自动发现源文件
# 扫描 src 目录下所有的 .c 文件，并将列表赋值给变量 SOURCES
file(GLOB SOURCES "src/*.c")

# 7. 生成可执行文件
# 最终生成的可执行文件名字叫 c-kv
add_executable(c-kv ${SOURCES})

# 打印一条消息，让你知道 CMake 跑起来了
message(STATUS "Build setup ready for sources: ${SOURCES}")