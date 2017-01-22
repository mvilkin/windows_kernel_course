#include "klogger.h"
#include "ring_buffer.h"
#include "utils.h"

static const size_t LOGGER_SIZE = 1 << 22; // 4 MB

void* klog_create(const char* filename)
{
	klog_t klog = alloc_memory(sizeof(klogger_info_t));
	klog->file_handle = open_file(filename);
	klog->rb = rb_create(LOGGER_SIZE, NULL);
	klog->filled_size = 0;
	return (void*)klog;
}

void klog_destroy(void* log)
{
	klog_t klog = (klog_t)log;
	rb_destroy(klog->rb);
	close_file(klog->file_handle);
	free_memory(klog);
}

size_t klog_write(void* log, void* buffer, size_t size)
{
	klog_t klog = (klog_t)log;
	size_t wr_size = rb_write(klog->rb, buffer, size);
	klog->filled_size += wr_size;
	return wr_size;
}

size_t int_flush(void* log)
{
	klog_t klog = (klog_t)log;
	size_t size = klog->filled_size;
	void* buf = alloc_memory(size);
	rb_read(klog->rb, buf, size);
	write_file(klog->file_handle, buf, size);
	free_memory(buf);
	return size;
}
