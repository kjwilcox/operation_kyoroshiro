
#ifndef INI_PARSER_H
#define INI_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INI_KEYSIZE 50
#define INI_VALSIZE 50
#define INI_DICTMAXSIZE 100

struct {
	char key[INI_KEYSIZE];
	char value[INI_VALSIZE];
} typedef pair;

struct {
	pair data[INI_DICTMAXSIZE];
	int size;
} typedef dict;

#define EMPTY_DICT {'\0', 0} 

dict settings;

dict load_ini(const char* filename, int* fail);

int ini_key_index(const char* k);

void ini_print(void);

int ini_val_i(const char* k);
double ini_val_d(const char* k);
char* ini_val_c(const char* k);
int ini_val_b(const char *k);



#endif