#ifndef _UTILS_H_
#define _UTILS_H_

#include <ntddk.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

HANDLE open_file(const char* name);
NTSTATUS close_file(void* handle);
size_t write_file(void* handle, void* src, size_t size);

void* alloc_memory(size_t size);
void free_memory(void* ptr);
void copy_memory(void* dst, void* src, size_t size);

long atomic_add(long* shared, long value);
long atomic_set(long* shared, long value);

#endif // _UTILS_H_
