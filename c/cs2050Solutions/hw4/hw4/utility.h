#ifndef _UTILITY_H_
#define _UTILITY_H_

#define MAX_BUFF_SIZE 50

typedef struct StringArray_ {
	char **strings;
	int size;
}StringArray;

StringArray* tokenizer (char *string, const char* delimiters);
void free_string_array(StringArray *sr);

#endif
