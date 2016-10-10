#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <crtdbg.h>

#include "klogger.h"

int main()
{
	{
		char* test_string = "test string\n";
		void* log = klog_create("tst.log");
		size_t written = klog_write(log, test_string, strlen(test_string));
		assert(written == strlen(test_string));
		written = klog_write(log, test_string, strlen(test_string));
		assert(written == strlen(test_string));
		int_flush(log);
		klog_destroy(log);
	}

	if (_CrtDumpMemoryLeaks())
		printf("Memory leak detected\n");

	return 0;
}
