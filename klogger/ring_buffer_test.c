#include <stdio.h>
#include <stdlib.h>

#include "ring_buffer.h"

int main()
{
	rb_t rb = rb_create(1024);
	rb_destroy(rb);
	return 0;
}