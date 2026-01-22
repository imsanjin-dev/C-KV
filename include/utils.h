#ifndef UTILS_H
#define UTILS_H

// 深拷贝字符串：内部会自动 malloc，记得 free
char* str_copy(const char* s);

void print_banner();

#endif