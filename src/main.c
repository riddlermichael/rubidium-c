#include <stdio.h>

#include <rbc/core/package.h>
#include <rbc/sync/package.h>

#define RBC_PC() __builtin_extract_return_addr(__builtin_return_address(0))

static int thread_func(void* arg) {
	RBC_UNUSED(arg);
	rbc_thread this_thread = rbc_thread_current();
	rbc_error error = rbc_thread_wait_for(this_thread, rbc_duration_s(1));
	printf("error %d: %s\n", error, rbc_error_to_string(error));
	return error;
}

#define RBC_DUMP(expr)                                                 \
	do {                                                               \
		rbc_error _error = expr;                                       \
		printf("error %d: %s\n", _error, rbc_error_to_string(_error)); \
	} while (0)

int main(void) {
	printf("%p\n", RBC_PC());

	rbc_thread this_thread = rbc_thread_current();
	printf("main thread %zu\n", rbc_thread_id(this_thread));

	rbc_thread RBC_CLEANUP(rbc_thread_destroy) thread;
	rbc_thread_init(&thread);
	usize stack_size = 1024UL * 1024UL;
	RBC_DUMP(rbc_thread_set_stack_size(thread, stack_size));
	RBC_DUMP(rbc_thread_start(thread, thread_func, NULL));
	int exit_code = 0;
	rbc_thread_join_with_code(thread, &exit_code);
	printf("Exit code: %d\n", exit_code);
	return 0;
}
