#include <ntddk.h>
#include "klogger.h"
#include"utils.h"

#define TEST_TIMERS_NUMBER 1

static void* klog = NULL;
static PKTIMER timer_test[TEST_TIMERS_NUMBER];
static PRKDPC dpc_obj[TEST_TIMERS_NUMBER];

VOID KloggerTestUnload(PDRIVER_OBJECT DriverObject);
void start_klogger_test();
void stop_klogger_test();
VOID timer_dpc_test_routine(PKDPC Dpc, PVOID DeferredContext,
	PVOID SystemArgument1, PVOID SystemArgument2);


NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT   DriverObject,
	IN PUNICODE_STRING  RegistryPath)
{
	DbgPrint("KLogger: start DriverEntry\n");

	DriverObject->DriverUnload = KloggerTestUnload;

	klog = klog_create(L"\\??\\C:\\Data\\KloggerTest.log");
	if (!klog) {
		DbgPrint("KLogger: Driver Enrty - error creating klogger\n");
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	start_klogger_test();

	DbgPrint("KLogger: DriverEntry completed\n");
	return STATUS_SUCCESS;
}

VOID KloggerTestUnload(
	IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("KLogger: start DriverUnload\n");
	stop_klogger_test();
	klog_destroy(klog);
	DbgPrint("KLogger: DriverUnload completed\n");
}

void start_klogger_test()
{
	LARGE_INTEGER timeout;
	LONG period;

	DbgPrint("KLogger: start test\n");

	for (unsigned i = 0; i < TEST_TIMERS_NUMBER; i++) {
		timer_test[i] = alloc_memory(sizeof(KTIMER));
		if (!timer_test[i]) {
			for (unsigned j = 0; j < i; j++) {
				free_memory(timer_test[j]);
				timer_test[j] = NULL;
			}
			DbgPrint("KLogger: start test - error allocate memory for test timer\n");
			return;
		}
	}

	for (unsigned i = 0; i < TEST_TIMERS_NUMBER; i++) {
		dpc_obj[i] = alloc_memory(sizeof(KDPC));
		if (!dpc_obj[i]) {
			for (unsigned j = 0; j < i; j++) {
				free_memory(dpc_obj[j]);
				dpc_obj[j] = NULL;
			}
			for (unsigned j = 0; j < TEST_TIMERS_NUMBER; j++) {
				free_memory(timer_test[j]);
				timer_test[j] = NULL;
			}
			DbgPrint("KLogger: start test - error allocate memory for dpc object\n");
			return;
		}
	}

	timeout.QuadPart = -1000000LL;	// 100ms, because time in 100ns format
	period = 1000; // 1s, because time in 1ms format

	for (unsigned i = 0; i < TEST_TIMERS_NUMBER; i++) {
		KeInitializeTimer(timer_test[i]);
		KeInitializeDpc(dpc_obj[i], timer_dpc_test_routine, klog);
		KeSetTimerEx(timer_test[i], timeout, period, dpc_obj[i]);
	}

	DbgPrint("KLogger: start test completed\n");
	return;
}

void stop_klogger_test()
{
	DbgPrint("KLogger: stop test\n");

	if (!dpc_obj[0] || !timer_test[0]) {
		DbgPrint("KLogger: start was failed\n");
		return;
	}

	KeFlushQueuedDpcs();

	for (unsigned i = 0; i < TEST_TIMERS_NUMBER; i++) {
		KeCancelTimer(timer_test[i]);
		free_memory(timer_test[i]);
		free_memory(dpc_obj[i]);
	}

	DbgPrint("KLogger: stop test complete\n");
}

VOID timer_dpc_test_routine(
	_In_     PKDPC	Dpc,
	_In_opt_ PVOID  DeferredContext,
	_In_opt_ PVOID  SystemArgument1,
	_In_opt_ PVOID  SystemArgument2)
{
	klog_t klog = (klog_t)DeferredContext;
	static unsigned counter = 0;

	if (counter % 2) {
		KIRQL cur_irql;
		static const char* msg = "message from high level\r\n";
		size_t msg_size = strlen(msg);
		size_t ret_size;

		KeRaiseIrql(HIGH_LEVEL, &cur_irql);
		ret_size = klog_write(klog, msg, msg_size);
		DbgPrint("KLogger: test write high - 0x%x out of 0x%x\n", ret_size, msg_size);
		KeLowerIrql(cur_irql);
	}
	else {
		static const char* msg = "message from dpc level\r\n";
		size_t msg_size = strlen(msg);
		size_t ret_size;

		ret_size = klog_write(klog, msg, msg_size);
		DbgPrint("KLogger: test write dpc - 0x%x out of 0x%x\n", ret_size, msg_size);
	}

	counter++;
}
