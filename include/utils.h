#ifndef UTILS_H
#define UTILS_H

// 深拷贝字符串：内部会自动 malloc，记得 free
char* str_copy(const char* s);

//djb2哈希算法：将字符串映射为无符号长整型
unsigned long hash(const char* str);

void print_banner();

#endif