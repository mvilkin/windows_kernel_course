#include <stdio.h>
#include <stdlib.h>

#include "ring_buffer.h"

static char* buffer;

rb_t rb_create(const char* filename, size_t size)
{
	return NULL;
}

void rb_destroy(rb_t rb)
{
	
}

size_t rb_write(rb_t rb, const void* src, size_t size)
{
	return 11;
}

size_t rb_read(rb_t rb, void* dst, size_t size)
{
	return 0;
}
