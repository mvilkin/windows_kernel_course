#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void* open_file(const char* name)
{
	/*if (!name)
		return NULL;

	HANDLE hfile = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error opening file: 0x%x\n", GetLastError());
		return NULL;
	}
	return hfile;*/
}

void close_file(void* handle)
{
	/*if (!handle)
		return;
	CloseHandle(handle);*/
}

size_t write_file(void* handle, void* src, size_t size)
{
	/*size_t wr_size = 0;
	BOOL err = WriteFile(handle, src, size, &wr_size, NULL);
	if (err == FALSE)
	{
		printf("Error writing to file: 0x%x\n", GetLastError());
		return 0;
	}
	return wr_size;*/
}

void* alloc_memory(size_t size)
{
	/*if (size <= 0)
		return NULL;
	return malloc(size);*/
}

void free_memory(void* ptr)
{
	/*if (!ptr)
		return;
	free(ptr);*/
}

void copy_memory(void* dst, void* src, size_t size)
{
	/*if (size <= 0 || !dst || !src)
		return;
	memcpy(dst, src, size);*/
}

long atomic_add(long* shared, long value)
{
	//return InterlockedExchangeAdd(shared, value);
}
