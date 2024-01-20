#include "timespec.h"

#include <rbc/core/os.h>

std_timespec rbc_timespec_to_std_timespec(rbc_timespec self) {
	return (std_timespec){.tv_sec = self.tv_sec, .tv_nsec = self.tv_nsec};
}

rbc_timespec rbc_timespec_from_std_timespec(std_timespec ts) {
	return (rbc_timespec){.tv_sec = ts.tv_sec, .tv_nsec = ts.tv_nsec};
}

#ifdef RBC_OS_LINUX

rbc_timespec rbc_timespec_get(void) {
	std_timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return rbc_timespec_from_std_timespec(ts);
}

rbc_timespec rbc_timespec_getres(void) {
	std_timespec ts;
	clock_getres(CLOCK_REALTIME, &ts);
	return rbc_timespec_from_std_timespec(ts);
}

#endif
