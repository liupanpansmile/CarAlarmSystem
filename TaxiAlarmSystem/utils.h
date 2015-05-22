#pragma once
#include "zlib.h"
namespace oautils{
	#include "banner.h"
	char *oa_uncompress(Bytef *data, uInt len, size_t *clen);
	int oa_msleep(signed int millis);
	void oa_realfree(void **oldptr);
	void *oa_memcpy(void *dst, void *src, size_t size);
	void *oa_memmove(size_t n, void *src, void *des);
	size_t oa_strlen(const char *str);
	char *oa_strcat(char *des, const char *src);
	void *oa_strcpy(void *des, const char *src);
	int oa_strcmp(const char *a1, const char *a2);
	int oa_strnicmp(size_t n, const char *s1, const char *s2);
	void *oa_malloc(size_t size);
	void *oa_realloc(void *oldptr, int newsize);
	void *oa_zalloc(size_t size);
	void *oa_calloc(size_t nelem, size_t elsize);

	char *oa_convert(char *data);
}
