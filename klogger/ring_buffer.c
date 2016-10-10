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
	if (rb->buffer + rb->size >= rb->tail + size)
	{
		copy_memory(rb->tail, src, size);
		rb->tail += size;
	}
	else
	{
		size_t bottom_size = rb->size - (rb->tail - rb->buffer);
		copy_memory(rb->tail, src, bottom_size);
		copy_memory(rb->buffer, (uint8_t*)src + bottom_size, size - bottom_size);
		rb->tail = rb->buffer + size - bottom_size;
	}
	return size;
}

size_t rb_read(rb_t rb, void* dst, size_t size)
{
	size_t read_size = 0;

	if (rb->head <= rb->tail)
	{
		read_size = MIN(rb->tail - rb->head, size);
		copy_memory(dst, rb->head, read_size);
		rb->head += read_size;
	}
	else
	{
		size_t bottom_size = rb->size - (rb->head - rb->buffer);
		size_t top_size = 0;
		if (size > bottom_size)
		{
			top_size = MIN(rb->size - bottom_size, size - bottom_size);
			read_size = bottom_size + top_size;
		}
		else
		{
			bottom_size = MIN(size, bottom_size);
			read_size = bottom_size;
		}
		copy_memory(dst, rb->head, bottom_size);
		copy_memory((uint8_t*)dst + bottom_size, rb->buffer, top_size);
		rb->head = rb->buffer + top_size;
	}

	return read_size;
}
