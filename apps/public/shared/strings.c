/*

	Tomato Firmware
	Copyright (C) 2006-2009 Jonathan Zarate

*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>

// #include <bcmnvram.h>
#include "shutils.h"
#include "shared.h"


/* Transfer Char to ASCII */
int char_to_ascii_safe(const char *output, const char *input, int outsize)
{
	char *src = (char *)input;
	char *dst = (char *)output;
	char *end = (char *)output + outsize - 1;
	char *escape = "[]"; // shouldn't be more?

	if (src == NULL || dst == NULL || outsize <= 0)
		return 0;

	for ( ; *src && dst < end; src++) {
		if ((*src >='0' && *src <='9') ||
		    (*src >='A' && *src <='Z') ||
		    (*src >='a' && *src <='z')) {
			*dst++ = *src;
		} else if (strchr(escape, *src)) {
			if (dst + 2 > end)
				break;
			*dst++ = '\\';
			*dst++ = *src;
		} else {
			if (dst + 3 > end)
				break;
			dst += sprintf(dst, "%%%.02X", *src);
		}
	}
	if (dst <= end)
		*dst = '\0';

	return dst - output;
}

void char_to_ascii(const char *output, const char *input)
{
	int outlen = strlen(input)*3 + 1;
	char_to_ascii_safe(output, input, outlen);
}

/* Transfer ASCII to Char */
int ascii_to_char_safe(const char *output, const char *input, int outsize){
	char *src = (char *)input;
	char *dst = (char *)output;
	char *end = (char *)output+outsize-1;
	char char_array[3];
	unsigned int char_code;

	if(src == NULL || dst == NULL || outsize <= 0)
		return 0;

	for(; *src && dst < end; ++src, ++dst){
		if((*src >= '0' && *src <= '9')
				|| (*src >= 'A' && *src <= 'Z')
				|| (*src >= 'a' && *src <= 'z')
				){
			*dst = *src;
		}
		else if(*src == '\\'){
			++src;
			if(!(*src))
				break;

			*dst = *src;
		}
		else{
			++src;
			if(!(*src))
				break;
			memset(char_array, 0, 3);
			strncpy(char_array, src, 2);
			++src;

			char_code = strtol(char_array, NULL, 16);

			*dst = (char)char_code;
		}
	}

	if(dst <= end)
		*dst = '\0';

	return (dst-output);
}

void ascii_to_char(const char *output, const char *input){
	int outlen = strlen(input)+1;
	ascii_to_char_safe(output, input, outlen);
}

const char *find_word(const char *buffer, const char *word)
{
	const char *p, *q;
	int n;

	n = strlen(word);
	p = buffer;
	while ((p = strstr(p, word)) != NULL) {
		if ((p == buffer) || (*(p - 1) == ' ') || (*(p - 1) == ',')) {
			q = p + n;
			if ((*q == ' ') || (*q == ',') || (*q == 0)) {
				return p;
			}
		}
		++p;
	}
	return NULL;
}

/*
static void add_word(char *buffer, const char *word, int max)
{
	if ((*buffer != 0) && (buffer[strlen(buffer) - 1] != ' '))
		strlcat(buffer, " ", max);
	strlcat(buffer, word, max);
}
*/

int remove_word(char *buffer, const char *word)
{
	char *p;
	char *q;

	if ((p = strstr(buffer, word)) == NULL) return 0;
	q = p;
	p += strlen(word);
	while (*p == ' ') ++p;
	while ((q > buffer) && (*(q - 1) == ' ')) --q;
	if (*q != 0) *q++ = ' ';
	strcpy(q, p);

	return 1;
}

int replce_str(char* pchrsource,char* pchrfind,char* pchrrep)
{
	char *pchrresult;
	char* pchrformer;
	char* pchrlocation;
	int intrep;
	int intfind;
	int intlength;
	int intgap=0;
	
	intfind=strlen(pchrfind);
	intrep=strlen(pchrrep);
	intlength=strlen(pchrsource)+1;
	
	pchrresult = (char*)malloc(sizeof(char) * intlength);
	strcpy(pchrresult, pchrsource);
	
	pchrformer=pchrsource;
	pchrlocation= strstr(pchrformer, pchrfind);
	
	while(pchrlocation!=NULL) {
	
		intgap+=(pchrlocation - pchrformer);
		pchrresult[intgap]='\0';
		
		intlength+=(intrep-intfind);
		pchrresult = (char*)realloc(pchrresult, intlength * sizeof(char));
		strcat(pchrresult, pchrrep);
		intgap+=intrep;
		
		pchrformer=pchrlocation+intfind;
		strcat(pchrresult, pchrformer);
		
		
		pchrlocation= strstr(pchrformer, pchrfind);
	}
	
	pchrresult[strlen(pchrresult)]='\0';
	
	strcpy(pchrsource,pchrresult);
	free(pchrresult);
	return 0;
}
