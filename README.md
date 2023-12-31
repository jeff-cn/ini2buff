# ini2buff
## Parse INI format data in buffer. A very simple and easy to use util.

Receive INI format data from server, it can direct to parse in buffer, modify, then send back to server, is not nessisary save to local file.

## How to use ini2buff:

1. Load your ini file to buffer:

	`char* inibuf = load2buffer("test.ini");`

	or receive your ini format data from network

	`char* inibuf = ...`

2. Operation on your ini format data:

	1) Get value of key

	  `char* value = get_inival(inibuf, "common", "testkey");`

	3) Set value of key (if section or key not exist, will create it)

	  `set_inival(inibuf, "common", "server_port", "54321");`

	5) Del key and value

	  `del_key(inibuf, "device", "config_api");`

	7) Del the whole setion

	  `del_section(inibuf, "device");`

3. Write ini format data in buffer to file
   
	`size_t rs = write2file("test.ini", inibuf);  `

4. Free buffer of the ini format data
   
	`free_all();`

5. How to build the test file (test/test_main.c)
	> cd test/
 
	> gcc -c test_main.c ../ini2buff.c ../utils_string.c
 
	> gcc test_main.o ini2buff.o utils_string.o -o test
 
	> ./test

 ------------------------------
	The output data of test:

	----------- orig data -----------
	[common]
	server_addr = 192.168.1.100
	server_port = 8080

	[communication]
	token = 0123-3456-6789

	[device]
	dev_ip = 192.168.1.200
	status_api = /api/status
	config_api = /api/config (178 bytes)

	----- del_key: server_addr, config_api -----
	[common]
	server_port = 8080

	[communication]
	token = 0123-3456-6789

	[device]
	dev_ip = 192.168.1.200
	status_api = /api/status (125 bytes)

	----- del section: common, device -----
	[communication]
	token = 0123-3456-6789 (38 bytes)

	----- set value / create if not exist -----
	[communication]
	testkey = testval-2
	token = 0123-3456-6789

	[device]
	testkey = testval-3

	[common]
	server_port = 54321
	testkey = testval-1

	[test1]
	testkey = testval-4

	[test2]
	testkey = testval-5 (196 bytes)

	----------- get value -----------
	common testkey:                 testval-1
	communication testkey:          testval-2
	device testkey:                 testval-3
	test1 testkey:                  testval-4
	test2 testkey:                  testval-5
	
	write 196 bytes.
