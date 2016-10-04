#include <stdio.h>
#include <stdlib.h>

#include "klogger.h"

int main()
{
	void* log = klog_create("test.log");
	klog_destroy(log);
	return 0;
}
