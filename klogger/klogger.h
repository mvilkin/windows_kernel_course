#ifndef _KLOGGER_H_
#define _KLOGGER_H_

#include <stdlib.h>
#include <ntddk.h>
#include "ring_buffer.h"

typedef struct _klogger_info_t
{
	rb_t rb;
	HANDLE file_handle;
	void* buffer_flush;
	PKTHREAD thread_flush;
	KEVENT event_flush;
	PKTIMER timer_flush;
	BOOLEAN stop_working;
} klogger_info_t, *klog_t;

void* klog_create(const char* filename);
void klog_destroy(void* log);
size_t klog_write(void* log, const void* buffer, size_t size);

#endif // _KLOGGER_H_
