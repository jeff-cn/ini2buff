/*
 * Copyright (c) 2023 Jeff Huang
 *
 * License: The MIT License (MIT)
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "../ini2buff.h"

int main(int argc, char const *argv[])
{
	char* inibuf = load2buffer("test.ini");
    printf("\n----------- orig data -----------\n");

	printf("%s (%ld bytes)\n", inibuf, strlen(inibuf));
	printf("\n----- del_key: server_addr, config_api -----\n");

	del_key(inibuf, "common", "server_addr");
	del_key(inibuf, "device", "config_api");

	printf("%s (%ld bytes)\n", inibuf, strlen(inibuf));
	printf("\n----- del section: common, device -----\n");

	del_section(inibuf, "common");
	del_section(inibuf, "device");

	printf("%s (%ld bytes)\n", inibuf, strlen(inibuf));
	printf("\n----- set value / create if not exist -----\n");

	set_inival(inibuf, "device", "testkey", "testval-3");
	set_inival(inibuf, "common", "testkey", "testval-1");
	set_inival(inibuf, "communication", "testkey", "testval-2");
	set_inival(inibuf, "test1", "testkey", "testval-4");
	set_inival(inibuf, "test2", "testkey", "testval-5");
	set_inival(inibuf, "common", "server_port", "54321");

	printf("%s (%ld bytes)\n", inibuf, strlen(inibuf));
    printf("\n----------- get value -----------\n");

    printf("common testkey: \t\t%s\n", 
            get_inival(inibuf, "common", "testkey"));
    printf("communication testkey: \t\t%s\n",
            get_inival(inibuf, "communication", "testkey"));
    printf("device testkey: \t\t%s\n",
            get_inival(inibuf, "device", "testkey"));
	printf("test1 testkey: \t\t\t%s\n",
			get_inival(inibuf, "test1", "testkey"));
	printf("test2 testkey: \t\t\t%s\n",
			get_inival(inibuf, "test2", "testkey"));

	
	size_t rs = write2file("test.ini", inibuf);  
	printf("write %ld bytes.\n", rs);

	free_all();
	
	return 0;
}
