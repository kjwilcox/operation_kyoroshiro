#include "ini.h"

dict load_ini(const char* filename, int* fail)
{
	FILE* infile = NULL;
	dict ini = EMPTY_DICT;
	int dicti = 0;
	enum mode_type {KEY, SEPARATOR, VAL, COMMENT, WHITESPACE};
	enum mode_type mode;
	char ch;
	char tempkey[INI_KEYSIZE] = {0};
	int tempkeyi = 0;
	char tempval[INI_VALSIZE] = {0};
	int tempvali = 0;
	mode = WHITESPACE;

	if (NULL != (infile = fopen(filename, "r")))
	{
		while (!feof(infile))
		{
			ch = fgetc(infile);

			// what we do next depends on the mode

			switch (mode)
			{
			case COMMENT:
				if (ch == '\n' || ch == '\r')
					mode = WHITESPACE;
				break;

			case KEY:
				if (ch == ' ' || ch == '=')
				{
					mode = SEPARATOR;
				}
				else
				{
					tempkey[tempkeyi] = ch;
					tempkeyi += 1;
				}
				break;

			case SEPARATOR:
				if (ch != ' ' && ch != '\t' && ch != '=')
				{
					ungetc(ch, infile);
					mode = VAL;
				}
				break;

			case VAL:
				if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t' || ch == ';')
				{
					// val has ended. time to write pair to dictionary
					strcat(ini.data[dicti].key, tempkey);
					strcat(ini.data[dicti].value, tempval);

					for (tempkeyi = 0; tempkeyi < INI_KEYSIZE; tempkeyi += 1)
						tempkey[tempkeyi] = '\0';

					for (tempvali = 0; tempvali < INI_VALSIZE; tempvali += 1)
						tempval[tempvali] = '\0';


					dicti += 1;
					ini.size += 1;
					tempkeyi = 0;
					tempvali = 0;

					if (ch == ';' || ch == '[')
						mode = COMMENT;
					else
						mode = WHITESPACE;
				}
				else
				{
					tempval[tempvali] = ch;
					tempvali += 1;
				}
				break;

			case WHITESPACE:
				if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n')
				{
					if (ch == '[' || ch == ';')
					{
						mode = COMMENT;
					}
					else
					{
						ungetc(ch, infile);
						mode = KEY;
					}
				}
				break;
			}
		}

		*fail = 0;
		return ini;

	}
	else
	{
		*fail = 1;
		return ini;
	}
}

int ini_key_index(const char* k)
{
	int i = 0;

	for (i = 0; i < settings.size; i += 1)
	{
		if (!strcmp(k, settings.data[i].key))
			return i;
	}

	return -1;
}

int ini_val_i(const char* k)
{
	int index = 0;
	extern dict settings;

	index = ini_key_index(k);
	if (index < 0)
		return -1;
	
	return atoi(settings.data[index].value);
}

double ini_val_d(const char* k)
{
	int index = 0;
	extern dict settings;

	index = ini_key_index(k);
	if (index < 0)
		return -1;
	
	return atof(settings.data[index].value);
}

char* ini_val_c(const char* k)
{
	int index = 0;
	extern dict settings;

	index = ini_key_index(k);
	if (index < 0)
		return "\0";
	
	return settings.data[index].value;
}

int ini_val_b(const char *k)
{
	int index;
	extern dict settings;

	index = ini_key_index(k);
	if (index < 0)
		return 0; // FALSE

	if (settings.data[index].value[0] == 'T' ||
		settings.data[index].value[0] == 't' ||
		settings.data[index].value[0] == '1' ||
		settings.data[index].value[0] == 'Y' ||
		settings.data[index].value[0] == 'y')
		return 1; // TRUE

	return 0; // FALSE
}

void ini_print(void)
{
	extern dict settings;
	int i = 0;
	for (i = 0; i < settings.size; i += 1)
		printf("%s: %s\n", settings.data[i].key, settings.data[i].value);
}