/*
 * Copyright (c) 2023 Jeff Huang
 *
 * License: The MIT License (MIT)
 */

#include "utils_string.h"

char* rtrim_chr(char *str, char c)
{
    if (str == NULL || *str == '\0')
    {
            return str;
    }
    int len = strlen(str);
    char *p = str + len - 1;
    while (p >= str && c == *p)
    {
        *p = '\0';
        --p;
    }
    return str;
}

char* ltrim_chr(char *str, char c)
{
    if (str == NULL || *str == '\0')
    {
        return str;
    }
    int len = 0;
    char *p = str;
    while (*p != '\0' && c == *p)
    {
        ++p;
        ++len;
    }
    memmove(str, p, strlen(str) - len + 1);
    return str;
}

char* trim_chr(char *str, char c)
{
    str = rtrim_chr(str, c);
    str = ltrim_chr(str, c);
    return str;
}

char* rtrim(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return str;
    }
    int len = strlen(str);
    char *p = str + len - 1;
    while (p >= str  && isspace(*p))
    {
        *p = '\0';
        --p;
    }
    return str;
}

char* ltrim(char *str)
{
    if (str == NULL || *str == '\0')
    {
        return str;
    }
    int len = 0;
    char *p = str;
    while (*p != '\0' && isspace(*p))
    {
        ++p;
        ++len;
    }
    memmove(str, p, strlen(str) - len + 1);
    return str;
}

char* trim(char *str)
{
    str = rtrim(str);
    str = ltrim(str);
    return str;
}

void delete_char(char* str, char c)
{
    int i, j;
    for(i = 0, j = 0; str[i] != '\0'; i++)
    {
        if(str[i] != c){
            str[j] = str[i]; j++;
        }
    }
    str[j] = '\0';
}

char* move_char(char* strbuf, size_t start, 
               int move_bytes, bool backward)
{
    size_t new_size;
    size_t org_size = strlen(strbuf);
    if(backward) {      
        new_size = strlen(strbuf) + move_bytes;

        for(size_t i=org_size; i>=start; i--) {
            strbuf[i + move_bytes] = strbuf[i];
            strbuf[i] = ' ';
        } 
        trim_chr(strbuf, '\n');
        return strbuf;
    }
    new_size = strlen(strbuf) - move_bytes;
    for(size_t i=start; i<org_size - start; i++) {
        strbuf[i] = strbuf[i + move_bytes];
        strbuf[i + move_bytes] = ' ';
    } 
    strbuf[new_size] = '\0';
    trim_chr(strbuf, '\n');

    return strbuf;
}
