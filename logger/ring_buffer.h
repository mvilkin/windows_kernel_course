#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

typedef unsigned char uint8_t;

struct ring_buffer_t
{
	uint8_t *buffer;
	uint8_t *head;
	uint8_t *tail;
	size_t   size;
};
typedef struct ring_buffer_t *rb_t;

rb_t rb_create(size_t size);
void rb_destroy(rb_t rb);
size_t rb_read(rb_t rb, void *dst, size_t size);
size_t rb_write(rb_t rb, void *src, size_t size);

#endif // _RING_BUFFER_H_
