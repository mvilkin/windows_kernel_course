#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "ring_buffer.h"
#include "utils.h"

rb_t rb_create(size_t size, rb_overflow_callback_t cb)
{
	ring_buffer_t* rb;

	rb = alloc_memory(sizeof(ring_buffer_t));
	rb->size = size;
	rb->buffer = alloc_memory(size);
	rb->ofw_callback = cb;
	rb->uHead = 0;
	rb->uTail = 0;
	rb->uWritten = 0;

	return rb;
}

void rb_destroy(rb_t rb)
{
	free_memory(rb->buffer);
	free_memory(rb);
}

size_t rb_write(rb_t rb, void* src, size_t size)
{
	uint32_t oldTail;

	/*
	TODO: Writing overflow message
	*/
	if (rb->uTail - rb->uHead + size >= rb->size){
		//overflowing
		return 0;
	}
	oldTail = atomic_add(rb->uTail, size);
	if (oldTail - rb->uHead + size >= rb->size){
		//overflowing
		//dec tail
		rb->uTail -= size;
		return 0;
	}

	if (rb->size - (oldTail % rb->size) < size){
		//overlapping
		size_t partSize = rb->size - (oldTail % rb->size);
		copy_memory(&rb->buffer[oldTail % rb->size], src, partSize);
		copy_memory(&rb->buffer[0
		], (uint8_t*)src + partSize, size - partSize);
	}
	else {
		copy_memory(&rb->buffer[oldTail % rb->size], src, size);
	}

	// sync with read thread
	// busywait for logs before this
	while (rb->uWritten != oldTail)
		;
	rb->uWritten += size;

	return size;
}

size_t rb_read(rb_t rb, void* dst, size_t size)
{
	uint32_t oldWritten;

	if (rb->uTail == rb->uHead)
		return 0;
	oldWritten = rb->uWritten;
	size = oldWritten - rb->uHead;

	if (rb->size - (rb->uHead % rb->size) < size){
		//overlapping
		size_t partSize = rb->size - (rb->uHead % rb->size);
		copy_memory(dst, &rb->buffer[rb->uHead % rb->size], partSize);
		copy_memory((uint8_t*)dst + partSize, &rb->buffer[0], size - partSize);
	}
	else {
		copy_memory(dst, &rb->buffer[rb->uHead % rb->size], size);
	}
	rb->uHead += oldWritten;
	return size;
}
