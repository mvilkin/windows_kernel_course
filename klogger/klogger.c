#include "klogger.h"
#include "ring_buffer.h"

static const size_t LOGGER_SIZE = 1 << 22; // 4 MB

void* klog_create(const char* filename)
{
	return (void*)rb_create(filename, LOGGER_SIZE);
}

void klog_destroy(void* log)
{
	rb_destroy((rb_t)log);
}

size_t klog_write(void* log, const void* buffer, size_t size)
{
	return rb_write((rb_t)log, buffer, size);
}
