#include "ntddk.h"

NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT   DriverObject,
	IN PUNICODE_STRING  RegistryPath)
{
	DbgPrint("MafMouse: Entering DriverEntry()\n");

	return STATUS_SUCCESS;
}