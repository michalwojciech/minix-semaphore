#include "protocolo_sem.h"

char buf[MAX_MSG];
int pos;

void
clear_buffer(void)
{
	pos = 0;
}

char *
get_buffer(void)
{
	return buf;
}

int
get_buffer_size(void)
{
	return pos;
}

void
put_long(long n)
{
	long aux = htonl(n);
	memcpy (&buf[pos], &aux , sizeof(long));
	pos += sizeof(long);
}

long
get_long(void)
{
	long aux;
	memcpy (&aux, &buf[pos], sizeof(long));
	pos += sizeof(long);
	return ntohl(aux);
}

void
put_char(int c)
{
	buf[pos++] = c;
}

int
get_char(void)
{
	return buf[pos++];
}

void
put_string(char * str, int size)
{
	buf[pos++] = size;
	memcpy(&buf[pos], str, size);
	pos += size;
}

int
get_string(char * str, int size)
{
	int len = buf[pos++];
	int cpy_size = size<len?size:len;
	memcpy(str, &buf[pos], cpy_size);
	pos += len;
	str[len] = 0;
	return cpy_size;
}


