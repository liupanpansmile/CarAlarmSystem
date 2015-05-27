#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "zlib.h"
#include "comm.h"

#ifdef __cplusplus
extern "C" {
#include "log.h"
}
#endif

namespace oautils{
const int MAX_COMPRESS_BUF_SIZE = 65536;

int oa_msleep(signed int millis)
{
	struct timespec ts;
	struct timespec remaining;

	ts.tv_sec = millis / 1000;
	ts.tv_nsec = 1000000 * millis % 1000;
	nanosleep(&ts, &remaining);	

	return true;
}
void oa_realfree(void **oldptr)
{
	if ( oldptr )
	{
		if ( *oldptr )
		{
			free(*oldptr);
			*oldptr = 0;
		}
	}
}
char *oa_uncompress(Bytef *data, uInt len, size_t *clen)
{
	int retlen;
	char *ret; 
	z_stream strm = {0}; 
	Bytef membuf[MAX_COMPRESS_BUF_SIZE] = {0};

	if ( data )
	{
		if ( *data )
		{
			if ( clen )
			{
				if ( len > 0 )
				{
					*clen = 0;
					memset(&strm, 0, sizeof(strm));
					strm.zalloc = (alloc_func)0;
					strm.zfree = (free_func)0;
					if ( Z_OK == inflateInit(&strm) )
					{
						ret = 0;
						retlen = 0;
						strm.avail_in = len;

						for ( strm.next_in = data; ; strm.next_in = data)
						{
							while ( 1 )
							{
								strm.avail_out = MAX_COMPRESS_BUF_SIZE;
								strm.next_out = membuf;
								int instatus = (inflate)(&strm, Z_FINISH);
								if ( instatus > Z_STREAM_END )
									break;
								if ( instatus )
								{
									int v5 = MAX_COMPRESS_BUF_SIZE - strm.avail_out + retlen;
									int v6 = MAX_COMPRESS_BUF_SIZE - strm.avail_out;
									ret = (char*)realloc(ret, MAX_COMPRESS_BUF_SIZE - strm.avail_out + retlen + 1);
									memcpy(((char*)ret + retlen), membuf, v6);
									(inflateEnd)(&strm);
									*clen = v5;
									*((char*)ret + v5) = 0;
									return ret;
								}
								ret = (char*)realloc(ret, retlen + MAX_COMPRESS_BUF_SIZE);
								ret = (char*)ret + retlen;
								if ( ret )
									memcpy(ret, membuf, MAX_COMPRESS_BUF_SIZE);
								retlen += MAX_COMPRESS_BUF_SIZE;
							}
							(inflateEnd)(&strm);
							if ( inflateInit2_(&strm, -15, ZLIB_VERSION, sizeof(strm)))
								break;
							strm.avail_in = len;
							data += retlen;
							if (retlen >= len)
							{
								break;
							}
						}
						if ( ret )
							free(ret);
					}
				}
			}
		}
	}
	return 0;
}

void *oa_memcpy(void *dst, void *src, size_t size)
{

	if ( dst )
	{
		if ( size > 0 )
		{
			if ( src )
				memcpy(dst, src, size);
		}
	}
	return dst;
}
void *oa_memmove(size_t n, void *src, void *des)
{
	if ( des )
	{
		if ( n > 0 )
		{
			if ( src )
				memmove(des, src, n);
		}
	}
	return des;
}
size_t oa_strlen(const char *str)
{
	size_t result; // eax@3

	if ( str && *str )
		result = strlen(str);
	else
		result = 0;
	return result;
}

char *oa_strcat(char *des, const char *src)
{
	char *result = NULL;

	if ( src && des )
	{
		int vdeslen = strlen(des);
		int vsrclen = strlen(src);
		vsrclen++;
		if ( &des[vdeslen] )
		{
			if ( vsrclen > 0 )
			{
				memcpy(&des[vdeslen], src, vsrclen);
			}
		}
		result = &des[vdeslen+vsrclen-1];
	}

	return result;
}
void *oa_strcpy(void *des, const char *src)
{
	size_t vlen = 0;
	void *result = des;
	if ( src && des )
	{
		vlen = strlen(src);
		if ( vlen > 0 )
			memcpy(des, src, vlen+1);
	}
	else
	{
		if ( des )
			*(char*)des = 0;
	}
	return (void *)((char*)result + vlen);
}
int oa_strcmp(const char *a1, const char *a2)
{
	int result = -2; 

	if ( a2 && a1 )
	{
		result = strcmp(a1, a2);
	}
	else
	{
		if ( a1 || a2 )
		{
			if ( a1 == 0)
				result = 1;
			if (a2 ==0)
				result = -1;
		}
	}
	return result;
}
int oa_strnicmp(size_t n, const char *s1, const char *s2)
{
	int result = 0;

	if ( s1 && s2 && n > 0 )
	{
		result = strncasecmp(s2, s1, n);
	}
	else
	{
		result = 0;
		if ( n )
		{

			if ( s1 || s2 )
			{
				if ( s1 == 0)
					result = 1;
				if (s2 ==0)
					result = -1;
			}
		}
	}
	return result;
}

void *oa_malloc(size_t size)
{
	size_t vsize; // ebx@1
	void *result; // eax@3

	vsize = size;
	if ( size <= 0 )
		vsize = 8;
	result = malloc(vsize);
	if ( !result )
	{
		log_error("failed to allocate %d byte(s)", vsize);
		exit(3);
	}
	return result;
}
void *oa_realloc(void *oldptr, int newsize)
{
	size_t vnewsize; // ebx@1
	void *result; // eax@4

	vnewsize = newsize;
	if ( oldptr )
	{
		if ( !newsize )
			vnewsize = 8;
		result = realloc(oldptr, vnewsize);
		if ( !result )
		{
			log_error("failed to reallocate %p for %d bytes", oldptr, vnewsize);
			exit(3);
		}
	}
	else
	{
		result = oa_malloc(newsize);
	}
	return result;
}
void *oa_zalloc(size_t size)
{
	size_t vsize;
	void *result;

	vsize = size;
	if ( size <= 0 )
		vsize = 8;
	result = calloc(1, vsize);
	if ( !result )
	{
		log_error("failed to allocate %d byte(s)", vsize);
		exit(3);
	}
	return result;
}
void *oa_calloc(size_t nelem, size_t elsize)
{
	size_t vnelem; 
	size_t velsize; 
	void *result;

	vnelem = nelem;
	velsize = elsize;
	if ( nelem <= 0 )
		vnelem = 1;
	if ( elsize <= 0 )
		velsize = 1;
	result = calloc(vnelem, velsize);
	if ( !result )
	{
		log_error("failed to allocate %d x %d bytes", vnelem, velsize);
		exit(3);
	}
	return result;
}

char * oa_convert( char *data )
{
	if (!data || !data[0]) return NULL;
	
	int datalen = oa_strlen(data);

	datalen *= 2;
	
	char *newdata = (char *)oa_malloc( sizeof(char) * datalen);
	if (!newdata) return NULL;

	int i = 0, j = 0;
	for (; i < datalen; i++)
	{
		if (data[i] == '\"')
		{
			newdata[j++] = '\\';
		}
		newdata[j++] = data[i];
	}
	return newdata;
}

} //namespace oautils

