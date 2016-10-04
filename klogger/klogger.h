#ifndef _KLOGGER_H_
#define _KLOGGER_H_

#include <stdlib.h>

void* klog_create(const char* filename);
void klog_destroy(void* log);
size_t klog_write(void* log, const void* buffer, size_t size);

#endif // _KLOGGER_H_
