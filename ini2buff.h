/*
 * Copyright (c) 2023 Jeff Huang
 *
 * License: The MIT License (MIT)
 */

#ifndef INI2BUFF_H_
#define INI2BUFF_H_

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "utils_string.h"

char* load2buffer(char *inifile);
int   write2file(char* inifile, char *inibuf);

char* get_inival(char* inibuf, char* section, char* key);
bool  set_inival(char* inibuf, char* section, char* key, char* val);

bool del_section(char* inibuf, char* section);
bool del_key(char* inibuf, char* section, char* key);

void free_all();

#endif /* INI2BUFF_H_ */