#define memset(str,c,len2) { \
	unsigned char *st = (unsigned char *)str; \
	int len = (int)len2; \
	while (len-- > 0) \
	*st++ = c; \
}

#define memcpy(destaddr,srcaddr,len2) { \
unsigned char *dest = (unsigned char *)destaddr; \
unsigned char *src = (unsigned char *)srcaddr; \
int len = (int)len2; \
while (len-- > 0) \
*dest++ = *src++; \
}
