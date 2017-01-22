#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

HANDLE open_file(const char* name)
{
	HANDLE hfile;
	UNICODE_STRING name_unicode;
	OBJECT_ATTRIBUTES attribs;
	NTSTATUS status;
	IO_STATUS_BLOCK status_block;

	if (!name)
		return NULL;

	RtlInitUnicodeString(&name_unicode, name);

	InitializeObjectAttributes(&attribs, &name_unicode,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	status = ZwCreateFile(&hfile,
		GENERIC_WRITE,
		&attribs, &status_block, NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OVERWRITE_IF,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL, 0);

	if (!NT_SUCCESS(status)) {
		DbgPrint("KLogger: open/create file error 0x%x\n", status);
		return NULL;
	}

	DbgPrint("KLogger: open/create file comleted\n");
	return hfile;
}

NTSTATUS close_file(void* handle)
{
	if (!handle)
		return STATUS_SUCCESS;
	return ZwClose(handle);
}

size_t write_file(void* handle, void* src, size_t size)
{
	NTSTATUS status;
	IO_STATUS_BLOCK status_block;

	if (!handle || !src || !size)
		return 0;

	status = ZwWriteFile(handle, NULL, NULL, NULL,
		&status_block, src, size, NULL, NULL);

	if (!NT_SUCCESS(status)) {
		DbgPrint("KLogger: write to file error\n");
		return 0;
	}

	DbgPrint("KLogger: write to file completed\n");
	return status_block.Information;
}

void* alloc_memory(size_t size)
{
	if (!size)
		return NULL;

	return ExAllocatePoolWithTag(NonPagedPool, size, 'Tag1');
}

void free_memory(void* ptr)
{
	if (!ptr)
		return;

	ExFreePoolWithTag(ptr, 'Tag1');
}

void copy_memory(void* dst, void* src, size_t size)
{
	if (!dst || !src || !size)
		return;

	RtlCopyMemory(dst, src, size);
}

long atomic_add(long* shared, long value)
{
	return InterlockedExchangeAdd(shared, value);
}

long atomic_set(long* shared, long value)
{
	return InterlockedExchange(shared, value);
}
