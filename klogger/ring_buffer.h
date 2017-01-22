#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct _ring_buffer_t;

typedef (*rb_overflow_callback_t)(struct _ring_buffer_t* rb);

typedef struct _ring_buffer_t
{
	uint8_t* buffer;
	size_t   size;
	rb_overflow_callback_t ofw_callback;
	uint32_t uHead;
	uint32_t uTail;
	uint32_t uWritten;
} ring_buffer_t, *rb_t;

rb_t rb_create(size_t size, rb_overflow_callback_t cb);
void rb_destroy(rb_t rb);
size_t rb_write(rb_t rb, const void* src, size_t size);
size_t rb_read(rb_t rb, void* dst, size_t size);

#endif // _RING_BUFFER_H_
