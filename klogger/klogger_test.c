#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "klogger.h"

int main()
{
	const char* test_string = "test string";
	void* log = klog_create("tst.log");
	size_t written = klog_write(log, test_string, strlen(test_string));
	assert(written == strlen(test_string));
	klog_destroy(log);
	return 0;
}
