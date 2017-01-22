#include "klogger.h"
#include "ring_buffer.h"
#include "utils.h"

static const size_t LOGGER_SIZE = 1 << 22; // 4 MB
static const size_t LOGGER_FLUSH_SIZE = 1 << 21; // 2 MB

void init_events(klog_t klog);
void destroy_events(klog_t klog);
void perform_flush(klog_t klog);
VOID flush_routine(PVOID context);
void init_thread_flush(klog_t klog);
void destroy_thread_flush(klog_t klog);

void* klog_create(const char* filename)
{
	klog_t klog = alloc_memory(sizeof(klogger_info_t));
	if (!klog) {
		DbgPrint("KLogger: init error - allocate memory for klogger\n");
		return NULL;
	}

	klog->rb = rb_create(LOGGER_SIZE, NULL);
	if (!klog->rb) {
		DbgPrint("KLogger: init error - create ring buffer\n");
		goto err_rbcreate;
	}

	klog->file_handle = open_file(filename);
	if (!klog->file_handle) {
		DbgPrint("KLogger: init error - open file for flush\n");
		goto err_openfile;
	}

	klog->buffer_flush = alloc_memory(LOGGER_FLUSH_SIZE);
	if (!klog->buffer_flush) {
		DbgPrint("KLogger: init error - allocate memory for flush buffer\n");
		goto err_buffalloc;
	}

	klog->stop_working = FALSE;

	init_events(klog);

	init_thread_flush(klog);
	if (!klog->thread_flush) {
		DbgPrint("KLogger: init error - create thread\n");
		goto err_thrcreate;
	}

	return (void*)klog;

err_thrcreate:
	free_memory(klog->buffer_flush);
err_buffalloc:
	close_file(klog->file_handle);
err_openfile:
	rb_destroy(klog->rb);
err_rbcreate:
	free_memory(klog);
	return NULL;
}

void klog_destroy(void* log)
{
	klog_t klog = (klog_t)log;
	BOOLEAN prev_stop;

	if (!klog)
		return;
	if (klog->stop_working)
		return;

	prev_stop = atomic_set(&klog->stop_working, TRUE);
	if (prev_stop)
		return;

	destroy_thread_flush(klog);
	destroy_events(klog);
	close_file(klog->file_handle);
	rb_destroy(klog->rb);
	free_memory(klog->buffer_flush);
	free_memory(klog);
}

size_t klog_write(void* log, void* buffer, size_t size)
{
	klog_t klog = (klog_t)log;
	DbgPrint("KLogger: write log message - 0x%x\n", size);

	if (!klog || !buffer || !size)
		return 0;

	if (klog->stop_working)
		return 0;

	return rb_write(klog->rb, buffer, size);;
}

void init_events(klog_t klog)
{
	if (!klog)
		return;

	KeInitializeEvent(
		&klog->event_flush,
		SynchronizationEvent, // TODO: NotificationEvent
		FALSE);
}

void destroy_events(klog_t klog)
{
	ZwClose(&klog->event_flush);
}

void perform_flush(klog_t klog)
{
	size_t size = 0;
	do {
		size = rb_read(klog->rb, klog->buffer_flush, LOGGER_FLUSH_SIZE);
		if (size)
			write_file(klog->file_handle, klog->buffer_flush, size);
	} while (size);
}


VOID flush_routine(PVOID context)
{
	klog_t klog = (klog_t)context;
	PVOID handles[1];
	LARGE_INTEGER timeout;

	DbgPrint("KLogger: flust thread - enter routine\n");

	handles[0] = (PVOID)&klog->event_flush;
	timeout.QuadPart = -10000000LL; // 1 sec, because time in 100ns format

	while (!klog->stop_working) {
		NTSTATUS status = KeWaitForMultipleObjects(
			1,
			handles,
			WaitAny,
			Executive,
			KernelMode,
			TRUE,
			&timeout,
			NULL);

		if (status == STATUS_TIMEOUT)
			DbgPrint("KLogger: flust thread - timer event\n");

		perform_flush(klog);
	}

	DbgPrint("KLogger: flust thread - exit routine");

	PsTerminateSystemThread(STATUS_SUCCESS);
}

void init_thread_flush(klog_t klog)
{
	HANDLE flushing_thread = NULL;
	NTSTATUS status;

	if (!klog)
		return;

	status = PsCreateSystemThread(
		&flushing_thread,
		THREAD_ALL_ACCESS,
		NULL,
		NULL,
		NULL,
		flush_routine,
		(PVOID)klog);

	if (!NT_SUCCESS(status)) {
		DbgPrint("KLogger: flush thread - error creating\n");
		return;
	}

	ObReferenceObjectByHandle(
		flushing_thread,
		THREAD_ALL_ACCESS,
		NULL,
		KernelMode,
		(PVOID *)&klog->thread_flush,
		NULL);

	ZwClose(flushing_thread);
}

void destroy_thread_flush(klog_t klog)
{
	KeWaitForSingleObject(
		klog->thread_flush,
		Executive,
		KernelMode,
		FALSE,
		NULL);

	ObDereferenceObject(klog->thread_flush);

	DbgPrint("KLogger: flush thread - exit\n");
}