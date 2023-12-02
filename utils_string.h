/*
 * Copyright (c) 2023 Jeff Huang
 *
 * License: The MIT License (MIT)
 */

#ifndef UTILS_STRING_H_
#define UTILS_STRING_H_

#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char* rtrim(char *str);
char* ltrim(char *str);
char* trim(char *str);

char* rtrim_chr(char *str, char c);
char* ltrim_chr(char *str, char c);
char* trim_chr(char *str, char c);

void delete_char(char* str, char c);

/* move_char 移动字符串中某个位之后的所有字符
   char* strbuf 要操作的字符串 
   size_t start 开始移动的位置
   int move_bytes 移动的步数(byte)
   bool backward true:向后移动 false：向前移动
   例如： 
   hello,world -> (6,3,true) -> hello,   world
   hello,   world -> (6,3,false) -> hello,world
*/
char* move_char(char* strbuf, size_t start, 
			   int move_bytes, bool backward);

#endif /* UTILS_STRING_H_ */
