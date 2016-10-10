#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "utils.h"

void* open_file(const char* name)
{
	if (!name)
		return NULL;

	HANDLE hfile =  CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW | TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("Error opening file: 0x%x\n", GetLastError());
		return NULL;
	}
	return hfile;
}

void close_file(void* handle)
{
	if (!handle)
		return;
	CloseHandle(handle);
}

void* alloc_memory(size_t size)
{
	if (size <= 0)
		return NULL;
	return malloc(size);
}

void free_memory(void* ptr)
{
	if (!ptr)
		return;
	free(ptr);
}

void copy_memory(void* dst, void* src, size_t size)
{
	if (size <= 0 || !dst || !src)
		return;
	memcpy(dst, src, size);
}
