/*
 * Copyright (c) 2023 Jeff Huang
 *
 * License: The MIT License (MIT)
 */

#include "ini2buff.h"

#define BUF_LINE (128)
#define REDUNDANCE  BUF_LINE

char* ini_buffer = NULL;

void free_all()
{
	if(!ini_buffer) return;
    free(ini_buffer);
    ini_buffer = NULL;
}

static long get_file_size(char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if(!fp) return -1;

    fseek(fp, 0 , SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0 , SEEK_SET);
    fclose(fp);

    return fileSize;
}

int write2file(char* inifile, char *inibuf)  
{  
    FILE *fp;   

    if(NULL == (fp = fopen(inifile, "w"))) {  
        perror("fopen"); return -1;
    }
    size_t rs = fwrite(inibuf, sizeof(char), strlen(inibuf), fp);
    fclose(fp); 

    return rs;  
} 

char* load2buffer(char *inifile)  
{  
    free_all();
    FILE *fp;  
    char line[BUF_LINE] = {0};   

    if(NULL == (fp = fopen(inifile, "r"))) {  
        perror("fopen"); return NULL;
    }

    ini_buffer = calloc(sizeof(char), get_file_size(inifile) + 128);
    while (NULL != fgets(line, BUF_LINE, fp)) {  
    	delete_char(line, '\r');
        strcat(ini_buffer, line);
    } 
    fclose(fp); 

    return trim_chr(ini_buffer, '\n');  
} 

char inival[128];
char* get_inival(char* inibuf, char* section, char* key)
{
	size_t buf_len = strlen(inibuf); 
	size_t sec_len = strlen(section) + 2;

	size_t fkey_start = 0, fkey_end = 0;
	size_t fval_start = 0, fval_end = 0;

	char fsection[sec_len];
	sprintf(fsection, "[%s]", section);

	char *tmp = strstr(inibuf, fsection); 
    if(!tmp) return NULL;
    fkey_start = tmp - inibuf + sec_len;

    tmp = strchr(inibuf + fkey_start, '['); 
    if(!tmp) fkey_end = buf_len;
    else	 fkey_end = tmp - inibuf;

    tmp = strstr(inibuf + fkey_start, key); 
    if(!tmp) return NULL;   

    fval_start = tmp - inibuf + strlen(key); 
    if(fval_start > fkey_end) return NULL;    

    tmp = strchr(inibuf + fval_start, '\n');
    if(!tmp) fval_end = strlen(inibuf);    
    else fval_end = tmp - inibuf;

    int val_len = fval_end - fval_start;
    memset(inival, 0, 128);
    memcpy(inival, inibuf + fval_start, val_len);

    delete_char(inival, '=');
    trim(inival);

    return inival;
}

bool set_inival(char* inibuf, char* section, char* key, char* val)
{
	char newstr[BUF_LINE];

	size_t sec_len = strlen(section) + 3; 	// "[、]、\n"
	size_t key_len = strlen(key) + 1;		// "\n"
	size_t val_len = strlen(val) + 3;		// " = "
	size_t tol_len = sec_len + key_len + val_len;

	if(tol_len > REDUNDANCE) {
		printf("buffer size may be too big than current.\n");
        return false;
	}

	char fsection[sec_len];
	sprintf(fsection, "[%s]", section);

	char *tmp = strstr(inibuf, fsection); 
    if(!tmp) {
    	memset(newstr, 0, BUF_LINE);
    	sprintf(newstr,"\n\n%s\n%s = %s",fsection, key, val);

    	strcat(inibuf, newstr);    	
    	return true;
    }

    size_t val_sta = tmp - inibuf + sec_len;
    tmp = strchr(inibuf + val_sta, '[');
    size_t val_end = tmp - inibuf;

    tmp = strstr(inibuf + val_sta, key); 
    if(!tmp || (tmp - inibuf) > val_end) {
    	memset(newstr, 0, BUF_LINE);
    	sprintf(newstr,"%s = %s\n",key, val);
    	size_t nlen = strlen(newstr);

    	move_char(inibuf, val_sta, nlen, true);
    	memcpy(inibuf + val_sta, newstr, nlen);

    	return true;
    }

    val_sta = tmp - inibuf + strlen(key);

    tmp = strchr(inibuf + val_sta, '=');
    val_sta = tmp - inibuf + 1;

    tmp = strchr(inibuf + val_sta, '\n');
    if(!tmp) val_end = strlen(inibuf);
    else val_end = tmp - inibuf;

    val_len = val_end - val_sta;
    size_t new_len = strlen(val) + 1; // ' '

    int mbytes = new_len - val_len;
    
    for(int i=val_sta; i<val_end; i++) inibuf[i] = ' ';

  	memset(newstr, 0, BUF_LINE);
    sprintf(newstr, " %s", val);

    if(mbytes <= 0) {   	
    	memcpy(inibuf + val_sta, newstr, strlen(newstr));

    	return true;
    }
    
    move_char(inibuf, val_end, mbytes, true);    
    memcpy(inibuf + val_sta, newstr, strlen(newstr));

    return true;
}

bool del_section(char* inibuf, char* section)
{
	size_t sec_len = strlen(section) + 2; 	// "[、]"

	size_t sec_sta = 0, sec_end = 0;

	char fsection[sec_len];
	sprintf(fsection, "[%s]", section);

	char *tmp = strstr(inibuf, fsection); 
    if(!tmp) return true;

    sec_sta = tmp - inibuf;

    tmp = strchr(inibuf + sec_sta + sec_len, '['); 
    if(!tmp) {
        inibuf[sec_sta] = '\0';
        rtrim_chr(inibuf, '\n');
        return true;
    }
    else sec_end = tmp - inibuf;

    int mbytes = sec_end - sec_sta;
    move_char(inibuf, sec_sta, mbytes, false); 

    return true;
}

bool del_key(char* inibuf, char* section, char* key)
{
	size_t sec_len = strlen(section) + 3; 	// "[、]、\n"
	size_t key_len = strlen(key) + 1;		// "\n"
	
	size_t fval_sta = 0, fval_end = 0;

	char fsection[sec_len];
	sprintf(fsection, "[%s]", section);

	char *tmp = strstr(inibuf, fsection); 
    if(!tmp) return true;

    fval_sta = tmp - inibuf + sec_len;

    tmp = strchr(inibuf + fval_sta, '[');
    if(!tmp) fval_end = strlen(inibuf);
    else fval_end = tmp - inibuf;

    tmp = strstr(inibuf + fval_sta, key);
    if(!tmp) return true;

    size_t val_end = 0;
    size_t val_sta = tmp - inibuf;
    tmp = strchr(inibuf + val_sta, '\n');
    if(!tmp) val_end = strlen(inibuf);
    else val_end = tmp - inibuf;

    int mbytes = val_end - val_sta + 1;  // '\n'
    move_char(inibuf, val_sta, mbytes, false); 
  	
    return true;    
}
