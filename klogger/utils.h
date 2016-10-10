#ifndef _UTILS_H_
#define _UTILS_H_

void* open_file(const char* name);
void close_file(void* handle);
void* alloc_memory(size_t size);
void free_memory(void* ptr);
void copy_memory(void* dst, void* src, size_t size);

#endif // _UTILS_H_
