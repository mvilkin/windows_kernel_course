#ifndef _KLOGGER_H_
#define _KLOGGER_H_

#include <stdlib.h>
#include "ring_buffer.h"

typedef struct _klogger_info_t
{
	rb_t rb;
	void* file_handle;
	size_t filled_size;
} klogger_info_t, *klog_t;

void* klog_create(const char* filename);
void klog_destroy(void* log);
size_t klog_write(void* log, void* buffer, size_t size);

// exported for debug only
size_t int_flush(void* log);

#endif // _KLOGGER_H_
