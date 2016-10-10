#include <stdio.h>
#include <stdlib.h>

#include "ring_buffer.h"
#include "utils.h"

rb_t rb_create(const char* filename, size_t size)
{
	ring_buffer_t* rb;

	rb = alloc_memory(sizeof(ring_buffer_t));
	rb->size = size;
	rb->buffer = alloc_memory(size);
	rb->head = rb->buffer;
	rb->tail = rb->buffer;

	return rb;
}

void rb_destroy(rb_t rb)
{
	free_memory(rb->buffer);
	free_memory(rb);
}

size_t rb_write(rb_t rb, void* src, size_t size)
{
	if (rb->head + rb->size >= rb->tail + size)
	{
		uint8_t* tmp_tail = rb->tail;
		rb->tail += size;
		copy_memory(tmp_tail, src, size);
	}
	else
	{
		uint8_t* tmp_tail = rb->tail;
		size_t size_bottom = rb->size - (rb->tail - rb->buffer);
		rb->tail = rb->buffer + size - size_bottom;
		copy_memory(tmp_tail, src, size_bottom);
		copy_memory(rb->buffer, (char*)src + size_bottom, size - size_bottom);
	}
	return size;
}

size_t rb_read(rb_t rb, void* dst, size_t size)
{
	return 0;
}
